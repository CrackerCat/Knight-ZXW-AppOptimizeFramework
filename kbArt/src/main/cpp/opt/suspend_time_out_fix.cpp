//
// Created by Knight-ZXW on 2024/5/14.
//

#include "opt/suspend_time_out_fix.h"
#include "art_runtime.h"
#include "shadowhook.h"
#include "kbart_jni.h"
#include "logger.h"
#include <vector>
#include "unordered_set"
#include <thread>
#include <mutex>
#include "shared_mutex"
#include "art_def.h"
namespace kbArt {
using namespace art;
#define TARGET_ART_LIB "libart.so"
#define LOG_TAG "thread_suspend_hook"
// Function signatures updated for readability
#define SYMBOL_THREAD_SUSPEND_BY_PEER_WARNING_14 "_ZN3artL26ThreadSuspendByPeerWarningERNS_18ScopedObjectAccessEN7android4base11LogSeverityEPKcP8_jobject"
#define SYMBOL_THREAD_SUSPEND_BY_PEER_WARNING_8_13 "_ZN3artL26ThreadSuspendByPeerWarningEPNS_6ThreadEN7android4base11LogSeverityEPKcP8_jobject"
#define SYMBOL_THREAD_SUSPEND_BY_PEER_WARNING_6_7 "_ZN3artL26ThreadSuspendByPeerWarningEPNS_6ThreadENS_11LogSeverityEPKcP8_jobject"
#define SYMBOL_THREAD_SUSPEND_BY_PEER_WARNING_5 "_ZN3artL26ThreadSuspendByPeerWarningEPNS_6ThreadEiPKcP8_jobject"


#define SYMBOL_THREAD_SUSPEND_BY_PEER_API_31 "_ZN3art10ThreadList19SuspendThreadByPeerEP8_jobjectbNS_13SuspendReasonEPb"

#define SYMBOL_THREAD_SUSPEND_BY_PEER_API_32_34 "_ZN3art10ThreadList19SuspendThreadByPeerEP8_jobjectNS_13SuspendReasonEPb"

//8.1~14
#define SYMBOL_THREAD_SUSPEND_BY_THREAD_ID_API_26_34 "_ZN3art10ThreadList23SuspendThreadByThreadIdEjNS_13SuspendReasonEPb"
//5.0~8.0 Thread* ThreadList::SuspendThreadByThreadId(uint32_t thread_id, bool debug_suspension,bool* timed_out)
#define SYMBOL_THREAD_SUSPEND_BY_THREAD_ID_API_21_25 "_ZN3art10ThreadList23SuspendThreadByThreadIdEjbPb"
typedef void (*ThreadSuspendByPeerWarning)(void *self, LogSeverity severity,
                                           const char *message, jobject peer); // 函数指针类型定义

typedef void *(*ThreadSuspendByThreadId)(void *threadList, uint32_t,
                                         SuspendReason,
                                         bool *); // 函数指针类型定义

static std::unordered_set<uint32_t> protectedThreadIdSet; // 共享的数据
std::shared_mutex  idVectorMutext;
static bool replaceSuspendMethodForAll = false;

jobject javaCallbackObject = nullptr; // 全局引用，指向Java层的监听回调对象
void *originalThreadSuspendByPeerWarning = nullptr; // 指向原始函数的指针
void *threadSuspendByPeerWarningHookStub = nullptr; // 指向存根函数的指针

ThreadSuspendByThreadId thread_suspend_by_thread_id = nullptr;

static jfieldID nativePeerFieldId = nullptr;

void *originalSuspendThreadByPeer = nullptr; // 指向原始函数的指针
void *proxySuspendThreadByPeerStub = nullptr;

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
  // 31和32 其实不一样
  if (apiLevel >=32 && apiLevel<=34){
    return SYMBOL_THREAD_SUSPEND_BY_PEER_API_32_34;
  } else {
    return SYMBOL_THREAD_SUSPEND_BY_PEER_API_31;
  }
}

const char *getThreadSuspendThreadByThreadIdSymbol() {
  if (apiLevel <= 34 && apiLevel > 26) { //12.1~14
    return SYMBOL_THREAD_SUSPEND_BY_THREAD_ID_API_26_34;
  } else {
    return SYMBOL_THREAD_SUSPEND_BY_THREAD_ID_API_21_25;
  }
}

//通知Java层观察者，出现suspendTimeout异常
void triggerSuspendTimeout() {
  // 触发挂起超时处理
  JNIEnv *pEnv = getJNIEnv();
  if (pEnv == nullptr) {
    return;
  }
  jclass jThreadHookClass = pEnv->FindClass(
      "com/knightboost/optimize/FixSuspendThreadTimeoutCallback");
  if (jThreadHookClass != nullptr) {
    jmethodID jMethodId = pEnv->GetMethodID(jThreadHookClass, "triggerSuspendTimeout",
                                            "()V");
    if (jMethodId != nullptr) {
      pEnv->CallVoidMethod(javaCallbackObject, jMethodId);
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
void *proxySuspendThreadByPeer31(void *thread_list,
                               jobject peer,
                                 bool request_suspension,
                               SuspendReason suspendReason,
                               bool *timed_out){
  SHADOWHOOK_STACK_SCOPE();
  //调用原函数
  if (replaceSuspendMethodForAll){
    //直接替换
    jlong nativePeer = getJNIEnv()->GetLongField(peer, nativePeerFieldId);
    uint32_t targetThreadId = ((Thread *) nativePeer)->GetThreadId();
    return thread_suspend_by_thread_id(thread_list, targetThreadId, suspendReason, timed_out);
  } else {
    uint32_t currentThreadId = Thread::Current()->GetThreadId();
    std::shared_lock<std::shared_mutex> lock(idVectorMutext);
    if (protectedThreadIdSet.find(currentThreadId) != protectedThreadIdSet.end()){
      lock.unlock();
      jlong nativePeer = getJNIEnv()->GetLongField(peer, nativePeerFieldId);
      uint32_t targetThreadId = ((Thread *) nativePeer)->GetThreadId();
//    LOGT("sliver", "替换线程  suspendThreadByThreadId,函数指针为 %p ,thread_list 为%p, 目标线程threadId为 %d, ",
//         thread_suspend_by_thread_id,
//         thread_list,
//         targetThreadId);
      return thread_suspend_by_thread_id(thread_list, targetThreadId, suspendReason, timed_out);
    } else{
      lock.unlock();
      //调用原函数
      void *thread = SHADOWHOOK_CALL_PREV(proxySuspendThreadByPeer31, thread_list, peer,request_suspension, suspendReason, timed_out);
      return thread;
    }
  }
}

void *proxySuspendThreadByPeer32_34(void *thread_list,
                                 jobject peer,
                                 SuspendReason suspendReason,
                                 bool *timed_out){
  SHADOWHOOK_STACK_SCOPE();
  //调用原函数
  if (replaceSuspendMethodForAll){
    //直接替换
    jlong nativePeer = getJNIEnv()->GetLongField(peer, nativePeerFieldId);
    uint32_t targetThreadId = ((Thread *) nativePeer)->GetThreadId();
//    LOGT("sliver", "替换线程  suspendThreadByThreadId,函数指针为 %p ,thread_list 为%p, 目标线程threadId为 %d, ",
//         thread_suspend_by_thread_id,
//         thread_list,
//         targetThreadId);
    return thread_suspend_by_thread_id(thread_list, targetThreadId, suspendReason, timed_out);
  } else {
    uint32_t currentThreadId = Thread::Current()->GetThreadId();
    std::shared_lock<std::shared_mutex> lock(idVectorMutext);
    if (protectedThreadIdSet.find(currentThreadId) != protectedThreadIdSet.end()){
      lock.unlock();
      jlong nativePeer = getJNIEnv()->GetLongField(peer, nativePeerFieldId);
      uint32_t targetThreadId = ((Thread *) nativePeer)->GetThreadId();
//      LOGT("sliver", "替换 目标线程 suspendThreadByThreadId,目标线程threadId为 %d", targetThreadId);

      return thread_suspend_by_thread_id(thread_list, targetThreadId, suspendReason, timed_out);
    } else{
      lock.unlock();
      //调用原函数
      void *thread = SHADOWHOOK_CALL_PREV(proxySuspendThreadByPeer32_34, thread_list, peer, suspendReason, timed_out);
      return thread;
    }
  }
}





using namespace kbArt;
extern "C"
JNIEXPORT void JNICALL
Java_com_knightboost_optimize_SuspendTimeoutFixer_preventThreadSuspendTimeoutFatalLog(JNIEnv *env,
                                                                                      jobject thiz,
                                                                                      jboolean replace_suspend_by_peer_to_thread_id_for_all_thread,
                                                                                      jobject callback) {

  // 处理钩子设置失败的情况
  jclass jThreadHookClass = env->FindClass(
      "com/knightboost/optimize/FixSuspendThreadTimeoutCallback");
  jmethodID onErrorMethodId = env->GetMethodID(jThreadHookClass, "onError",
                                               "(Ljava/lang/String;)V");

  jclass threadClass = env->FindClass("java/lang/Thread");  // Find the Java Thread class.
  nativePeerFieldId = env->GetFieldID(threadClass,"nativePeer","J");

  if (javaCallbackObject != nullptr) {
    env->DeleteGlobalRef(javaCallbackObject);
    //调用过，只在开发环境中会出现，进行资源清理操作
    replaceSuspendMethodForAll = false;
    if (proxySuspendThreadByPeerStub!= nullptr){
      shadowhook_unhook(proxySuspendThreadByPeerStub);
    }

    if (threadSuspendByPeerWarningHookStub!= nullptr){
      shadowhook_unhook(threadSuspendByPeerWarningHookStub);
    }
  }
  javaCallbackObject = env->NewGlobalRef(callback);

  if (apiLevel>34){
    env->CallVoidMethod(callback,onErrorMethodId,env->NewStringUTF("api 版本 >暂未兼容"));
  }else if (apiLevel >= 31 && apiLevel <= 34) {
    //31以上 使用替换 suspendThreadByPeer 的方式
    replaceSuspendMethodForAll = replace_suspend_by_peer_to_thread_id_for_all_thread;

    void *handle = shadowhook_dlopen("libart.so");
    thread_suspend_by_thread_id = (ThreadSuspendByThreadId) shadowhook_dlsym(handle, getThreadSuspendThreadByThreadIdSymbol());
    shadowhook_dlclose(handle);
    if (thread_suspend_by_thread_id == nullptr) {
      env->CallVoidMethod(callback, onErrorMethodId, env->NewStringUTF("thread_suspend_by_thread_id 符号未找到"));
      return;
    }

    if (apiLevel == 31) {
      proxySuspendThreadByPeerStub = shadowhook_hook_sym_name("libart.so", SYMBOL_THREAD_SUSPEND_BY_PEER_API_31,
                                                              (void *) proxySuspendThreadByPeer31,
                                                              &originalSuspendThreadByPeer);

    }else{
      proxySuspendThreadByPeerStub = shadowhook_hook_sym_name("libart.so", SYMBOL_THREAD_SUSPEND_BY_PEER_API_32_34,
                                                              (void *) proxySuspendThreadByPeer32_34,
                                                              &originalSuspendThreadByPeer);
    }

    if (proxySuspendThreadByPeerStub == nullptr) {
      const int err_num = shadowhook_get_errno();
      const char *errMsg = shadowhook_to_errmsg(err_num);
      env->CallVoidMethod(callback, onErrorMethodId, env->NewStringUTF(errMsg));
      return;
    } else {
      //Hook成功
      jmethodID jMethodId = env->GetMethodID(jThreadHookClass, "hookSuspendThreadByIdSuccess",
                                             "()V");
      if (jMethodId != nullptr) {
        env->CallVoidMethod(callback, jMethodId);
      }
      return;
    }
  } else {
    //31以下替换 suspendThreadByPeerWarning日志级别的方式
    // 如果已经调用Hook过 取消之前的hook
    if (threadSuspendByPeerWarningHookStub != nullptr) {
      shadowhook_unhook(threadSuspendByPeerWarningHookStub);
      threadSuspendByPeerWarningHookStub = nullptr;
    }
    threadSuspendByPeerWarningHookStub = shadowhook_hook_sym_name("libart.so",
                                                                  getThreadSuspendByPeerWarningFunctionName(),
                                                                  (void *) proxyThreadSuspendTimeoutWarning,
                                                                  (void **) &originalThreadSuspendByPeerWarning);
    if (threadSuspendByPeerWarningHookStub == nullptr) {
      const int err_num = shadowhook_get_errno();
      const char *errMsg = shadowhook_to_errmsg(err_num);
      if (errMsg == nullptr || javaCallbackObject == nullptr) {
        //should never happen
        return;
      }
      __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Hook setup failed: %s", errMsg);
      env->CallVoidMethod(javaCallbackObject, onErrorMethodId, env->NewStringUTF(errMsg));
      //cleanup
      if (javaCallbackObject) {
        env->DeleteGlobalRef(javaCallbackObject);
        javaCallbackObject = nullptr;
      }
    } else {
      __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Hook setup success");
      jmethodID hookSuspendThreadByPeerWarningSuccessMethodId = env->GetMethodID(jThreadHookClass,
                                                                                 "hookSuspendThreadByPeerWarningSuccess", "()V");
      env->CallVoidMethod(callback, hookSuspendThreadByPeerWarningSuccessMethodId);
    }
  }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_knightboost_optimize_SuspendTimeoutFixer_addThread(JNIEnv *env, jobject thiz, jobject targetThread) {
  std::unique_lock<std::shared_mutex> lock(idVectorMutext);
  // getThreadId
  if (nativePeerFieldId == nullptr){
    jclass threadClass = env->FindClass("java/lang/Thread");  // Find the Java Thread class.
    nativePeerFieldId = env->GetFieldID(threadClass,"nativePeer","J");
  }

  jlong nativePeerValue = env->GetLongField(targetThread, nativePeerFieldId);
  auto *thread = reinterpret_cast<Thread *>(nativePeerValue);
  uint32_t threadId = thread->GetThreadId();
  protectedThreadIdSet.insert(threadId);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_knightboost_optimize_SuspendTimeoutFixer_removeThread(JNIEnv *env, jobject thiz, jobject targetThread) {
  if (nativePeerFieldId == nullptr){
    jclass threadClass = env->FindClass("java/lang/Thread");  // Find the Java Thread class.
    nativePeerFieldId = env->GetFieldID(threadClass,"nativePeer","J");
  }
  std::unique_lock<std::shared_mutex> lock(idVectorMutext);
  jlong nativePeerValue = env->GetLongField(targetThread, nativePeerFieldId);
  auto *thread = reinterpret_cast<Thread *>(nativePeerValue);
//  LOGE("sliver","目标线程 nativePeerValue = %ld",nativePeerValue);
  uint32_t threadId = thread->GetThreadId();
  protectedThreadIdSet.erase(threadId);
}
}

