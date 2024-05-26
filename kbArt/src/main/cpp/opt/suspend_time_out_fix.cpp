//
// Created by Knight-ZXW on 2024/5/14.
//

#include "opt/suspend_time_out_fix.h"
#include "art_runtime.h"
#include "shadowhook.h"
#include "kbart_jni.h"
#include "logger.h"
namespace kbArt {
using namespace art;
#define TARGET_ART_LIB "libart.so"
#define LOG_TAG "thread_suspend_hook"
// Function signatures updated for readability
#define SYMBOL_THREAD_SUSPEND_BY_PEER_WARNING_14 "_ZN3artL26ThreadSuspendByPeerWarningERNS_18ScopedObjectAccessEN7android4base11LogSeverityEPKcP8_jobject"
#define SYMBOL_THREAD_SUSPEND_BY_PEER_WARNING_8_13 "_ZN3artL26ThreadSuspendByPeerWarningEPNS_6ThreadEN7android4base11LogSeverityEPKcP8_jobject"
#define SYMBOL_THREAD_SUSPEND_BY_PEER_WARNING_6_7 "_ZN3artL26ThreadSuspendByPeerWarningEPNS_6ThreadENS_11LogSeverityEPKcP8_jobject"
#define SYMBOL_THREAD_SUSPEND_BY_PEER_WARNING_5 "_ZN3artL26ThreadSuspendByPeerWarningEPNS_6ThreadEiPKcP8_jobject"

#define SYMBOL_THREAD_SUSPEND_BY_PEER_API_32_34 "_ZN3art10ThreadList19SuspendThreadByPeerEP8_jobjectNS_13SuspendReasonEPb"

//8.1~14
#define SYMBOL_THREAD_SUSPEND_BY_THREAD_ID_API_26_34 "_ZN3art10ThreadList23SuspendThreadByThreadIdEjNS_13SuspendReasonEPb"
//5.0~8.0 Thread* ThreadList::SuspendThreadByThreadId(uint32_t thread_id, bool debug_suspension,bool* timed_out)
#define SYMBOL_THREAD_SUSPEND_BY_THREAD_ID_API_21_25 "_ZN3art10ThreadList23SuspendThreadByThreadIdEjbPb"
typedef void (*ThreadSuspendByPeerWarning)(void *self, LogSeverity severity,
                                           const char *message, jobject peer); // 函数指针类型定义

typedef void* (*ThreadSuspendByThreadId)(void *threadList,  uint32_t,
                                        SuspendReason,
                                        bool *); // 函数指针类型定义

static int apiLevel = android_get_device_api_level();
static uint32_t replaceSuspendTargetThreadId = -1;


jobject hookThreadSuspendByPeerWarningCallbackObj = nullptr; // 全局引用，指向Java层的监听回调对象
void *originalThreadSuspendByPeerWarning = nullptr; // 指向原始函数的指针
void *threadSuspendByPeerWarningHookStub = nullptr; // 指向存根函数的指针

ThreadSuspendByThreadId thread_suspend_by_thread_id = nullptr;


const char *getThreadSuspendByPeerWarningFunctionName() {
  // Simplified logic based on Android API levels
  if (apiLevel < 23) {
    return SYMBOL_THREAD_SUSPEND_BY_PEER_WARNING_5;
  } else if (apiLevel < 26) {
    // below android 8
    return SYMBOL_THREAD_SUSPEND_BY_PEER_WARNING_6_7;
  } else if (apiLevel < 34) {
    // above android 8 and below android 14
    return SYMBOL_THREAD_SUSPEND_BY_PEER_WARNING_8_13;
  } else if (apiLevel == 34) {
    // android 14+
    return SYMBOL_THREAD_SUSPEND_BY_PEER_WARNING_14;
  }
}

const char *getThreadSuspendByPeerSymbol() {
//  if (apiLevel <= 34 && apiLevel >= 32) { //12.1~14
//    return SYMBOL_THREAD_SUSPEND_BY_PEER_API_32_34;
//  }

  return SYMBOL_THREAD_SUSPEND_BY_PEER_API_32_34;

}

const char* getThreadSuspendThreadByThreadIdSymbol(){
  if (apiLevel <= 34 && apiLevel > 26) { //12.1~14
    return SYMBOL_THREAD_SUSPEND_BY_THREAD_ID_API_26_34;
  } else {
    return SYMBOL_THREAD_SUSPEND_BY_THREAD_ID_API_21_25;
  }
}

void triggerSuspendTimeout();
//通知Java层观察者，出现suspendTimeout异常
void triggerSuspendTimeout() {
  // 触发挂起超时处理
  JNIEnv *pEnv = getJNIEnv();
  if (pEnv == nullptr) {
    return;
  }
  jclass jThreadHookClass = pEnv->FindClass(
      "com/knightboost/sliver/FixSuspendThreadTimeoutCallback");
  if (jThreadHookClass != nullptr) {
    jmethodID jMethodId = pEnv->GetMethodID(jThreadHookClass, "triggerSuspendTimeout",
                                            "()V");
    if (jMethodId != nullptr) {
      pEnv->CallVoidMethod(hookThreadSuspendByPeerWarningCallbackObj, jMethodId);
    }
  }
}

void proxyThreadSuspendTimeoutWarning(void *self, LogSeverity severity, const char *message, jobject peer) {
  SHADOWHOOK_STACK_SCOPE();
  if (severity == FATAL && strcmp(message, SUSPEND_LOG_MSG) == 0) {

    // 如果当前是 FATAL 并且 message 是 Thread suspend timeout 则不调用原始函数,直接返回
    __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Fatal threadSuspendTimeout catch success : %s", message);
    triggerSuspendTimeout();
    //测试验证
    //该行日志 验证 是不是回调Java层打印日志导致的崩溃问题
    __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "triggerSuspendTimeout callback success");
  } else {
    if (SHADOWHOOK_IS_SHARED_MODE) {
      SHADOWHOOK_CALL_PREV(proxyThreadSuspendTimeoutWarning, self, severity, message, peer);
    } else {
      reinterpret_cast<ThreadSuspendByPeerWarning>(originalThreadSuspendByPeerWarning)(
          self, severity, message, peer
      );
    }
  }
}




static jfieldID nativePeerFieldId = nullptr;

void *originalSuspendThreadByPeer = nullptr; // 指向原始函数的指针
void *proxySuspendThreadByPeerStub = nullptr;
void *proxySuspendThreadByPeer(void *thread_list,
                               jobject peer,
                               SuspendReason suspendReason,
                               bool *timed_out) {
  SHADOWHOOK_STACK_SCOPE();
  LOGE("sliver", "proxySuspendThreadByPeer, currentThread is %p ,peer is %p  suspendReason= %hhb",Thread::Current(), peer, suspendReason);
  //调用原函数
  uint32_t currentThreadId = Thread::Current()->GetThreadId();
  if (currentThreadId == replaceSuspendTargetThreadId){ //是目标线程替换为suspendThreadById
    jlong nativePeer = getJNIEnv()->GetLongField(peer, nativePeerFieldId);
    uint32_t targetThreadId = ((Thread *) nativePeer)->GetThreadId();
    LOGT("sliver","目标线程threadId为 %d",targetThreadId);
    return thread_suspend_by_thread_id(thread_list,targetThreadId,suspendReason,timed_out);
  } else{ //非目标线程，直接调用原函数
    void *thread  =SHADOWHOOK_CALL_PREV(proxySuspendThreadByPeer, thread_list, peer, suspendReason, timed_out);
    LOGT("sliver", "当前线程为非目标保护线程， Hook 返回thread指针地址 %p,当前线程threadId 为%d",
         thread, currentThreadId);
    return thread;
  }

}



/**
 * 防止目标线程调用suspend异常
 */
extern "C"
JNIEXPORT void JNICALL
Java_com_knightboost_sliver_Sliver_preventThreadSuspendTimeoutFatalLog(JNIEnv *env,
                                                                       jclass clazz, jobject callback) {
  // 只支持Android API 31以下 ,由于Android 12.1 在Fatal 日志后添加了  UNREACHABLE ，因此只进行该函数Hook没有作用
  if (apiLevel >= 31) {
    LOGW("sliver", "preventThreadSuspendTimeoutFatalLog doesn't support on android api %d", apiLevel);
    return;
  }
  if (hookThreadSuspendByPeerWarningCallbackObj != nullptr) {
    env->DeleteGlobalRef(hookThreadSuspendByPeerWarningCallbackObj);
  }
  hookThreadSuspendByPeerWarningCallbackObj = env->NewGlobalRef(callback);

  // 如果已经调用Hook过 取消之前的hook
  if (threadSuspendByPeerWarningHookStub != nullptr) {
    shadowhook_unhook(threadSuspendByPeerWarningHookStub);
    threadSuspendByPeerWarningHookStub = nullptr;
  }
  threadSuspendByPeerWarningHookStub = shadowhook_hook_sym_name("libart.so",
                                                                getThreadSuspendByPeerWarningFunctionName(),
                                                                (void *) proxyThreadSuspendTimeoutWarning,
                                                                (void **) &originalThreadSuspendByPeerWarning);
  if (threadSuspendByPeerWarningHookStub == nullptr) { //在线下测试中发现，部分Android 13的设备，实际符号不同,和Android 14的符号一样,待确认原因
    int apiLevel = android_get_device_api_level();
    if (apiLevel == 33) {
      //SYMBOL_THREAD_SUSPEND_BY_PEER_WARNING_14
      threadSuspendByPeerWarningHookStub = shadowhook_hook_sym_name("libart.so",
                                                                    SYMBOL_THREAD_SUSPEND_BY_PEER_WARNING_14,
                                                                    (void *) proxyThreadSuspendTimeoutWarning,
                                                                    (void **) &originalThreadSuspendByPeerWarning);
    }

  }
  if (threadSuspendByPeerWarningHookStub == nullptr) {
    const int err_num = shadowhook_get_errno();
    const char *errMsg = shadowhook_to_errmsg(err_num);
    if (errMsg == nullptr || hookThreadSuspendByPeerWarningCallbackObj == nullptr) {
      return;
    }
    __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Hook setup failed: %s", errMsg);

    // 处理钩子设置失败的情况
    jclass jThreadHookClass = env->FindClass(
        "com/knightboost/sliver/FixSuspendThreadTimeoutCallback");
    if (jThreadHookClass != nullptr) {
      jmethodID jMethodId = env->GetMethodID(jThreadHookClass, "onError",
                                             "(Ljava/lang/String;)V");
      if (jMethodId != nullptr) {
        env->CallVoidMethod(hookThreadSuspendByPeerWarningCallbackObj, jMethodId, env->NewStringUTF(errMsg));
      }
    }
    //cleanup
    if (hookThreadSuspendByPeerWarningCallbackObj) {
      env->DeleteGlobalRef(hookThreadSuspendByPeerWarningCallbackObj);
      hookThreadSuspendByPeerWarningCallbackObj = nullptr;
    }
  } else {
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Hook setup success");
  }
}


extern "C"
JNIEXPORT void JNICALL
Java_com_knightboost_sliver_Sliver_replaceThreadByPeerToById(JNIEnv *env, jclass clazz, jobject target_thread, jobject callback) {
  if (nativePeerFieldId == nullptr) {
    jclass threadClass = env->FindClass("java/lang/Thread");  // Find the Java Thread class.
    if (threadClass == nullptr) {
      //never happen
      return;
    }
    nativePeerFieldId = env->GetFieldID(threadClass, "nativePeer", "J");
  }

  jclass jThreadHookClass = env->FindClass(
      "com/knightboost/sliver/FixSuspendThreadTimeoutCallback");

  
  void *handle = shadowhook_dlopen("libart.so");
  thread_suspend_by_thread_id =  (ThreadSuspendByThreadId)shadowhook_dlsym(handle, getThreadSuspendThreadByThreadIdSymbol());
  if (thread_suspend_by_thread_id == nullptr){
    jmethodID jMethodId = env->GetMethodID(jThreadHookClass, "onError",
                                           "(Ljava/lang/String;)V");
    if (jMethodId != nullptr) {
      env->CallVoidMethod(callback, jMethodId, env->NewStringUTF("thread_suspend_by_thread_id 符号未找到"));
    }

  }

  jlong nativePeerValue = env->GetLongField(target_thread, nativePeerFieldId);

  auto* thread = reinterpret_cast<Thread *>(nativePeerValue);
  uint32_t threadId = thread->GetThreadId();

  if (proxySuspendThreadByPeerStub != nullptr) {
    //已经替换过了，直接记录目标线程
    //TODO 添加数组
    jmethodID jMethodId = env->GetMethodID(jThreadHookClass, "hookSuspendThreadByIdSuccess",
                                           "()V");
    if (jMethodId != nullptr) {
      env->CallVoidMethod(callback, jMethodId);
    }
  } else {
    //TODO 添加数组
    proxySuspendThreadByPeerStub = shadowhook_hook_sym_name("libart.so", getThreadSuspendByPeerSymbol(),
                                                            (void *) proxySuspendThreadByPeer,
                                                            &originalSuspendThreadByPeer);
    if (proxySuspendThreadByPeerStub == nullptr) {
      const int err_num = shadowhook_get_errno();
      const char *errMsg = shadowhook_to_errmsg(err_num);
      jmethodID jMethodId = env->GetMethodID(jThreadHookClass, "onError",
                                             "(Ljava/lang/String;)V");
      if (jMethodId != nullptr) {
        env->CallVoidMethod(callback, jMethodId, env->NewStringUTF(errMsg));
      }
    } else {
      //Hook成功
      replaceSuspendTargetThreadId = threadId;
      jmethodID jMethodId = env->GetMethodID(jThreadHookClass, "hookSuspendThreadByIdSuccess",
                                             "()V");
      if (jMethodId != nullptr) {
        env->CallVoidMethod(callback, jMethodId);
      }
    }

  }

}

extern "C"
JNIEXPORT void JNICALL
Java_com_knightboost_sliver_Sliver_replaceThreadByPeerToByIdAll(JNIEnv *env, jclass clazz, jobject callback) {
}

}
