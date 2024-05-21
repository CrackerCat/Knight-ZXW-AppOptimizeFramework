//
// Created by Knight-ZXW on 2024/5/14.
//

#include "opt/suspend_time_out_fix.h"
#include "art_runtime.h"
#include "shadowhook.h"
#include "kbart_jni.h"
namespace kbArt {
using namespace art;
#define TARGET_ART_LIB "libart.so"
#define LOG_TAG "thread_suspend_hook"
// Function signatures updated for readability
#define SYMBOL_THREAD_SUSPEND_BY_PEER_WARNING_14 "_ZN3artL26ThreadSuspendByPeerWarningERNS_18ScopedObjectAccessEN7android4base11LogSeverityEPKcP8_jobject"
#define SYMBOL_THREAD_SUSPEND_BY_PEER_WARNING_8_13 "_ZN3artL26ThreadSuspendByPeerWarningEPNS_6ThreadEN7android4base11LogSeverityEPKcP8_jobject"
#define SYMBOL_THREAD_SUSPEND_BY_PEER_WARNING_6_7 "_ZN3artL26ThreadSuspendByPeerWarningEPNS_6ThreadENS_11LogSeverityEPKcP8_jobject"
#define SYMBOL_THREAD_SUSPEND_BY_PEER_WARNING_5 "_ZN3artL26ThreadSuspendByPeerWarningEPNS_6ThreadEiPKcP8_jobject"

jobject callbackObj = nullptr; // 全局引用，指向Java层的监听回调对象
void *originalFunction = nullptr; // 指向原始函数的指针
void *stubFunction = nullptr; // 指向存根函数的指针
std::atomic<LogSeverity> m_severity{WARNING}; // 日志严重性级别的原子变量，默认为INFO

typedef void (*ThreadSuspendByPeerWarning)(void *self, LogSeverity severity,
                                           const char *message, jobject peer); // 函数指针类型定义

void triggerSuspendTimeout();
//通知Java层观察者，出现suspendTimeout异常
void triggerSuspendTimeout() {
  // 触发挂起超时处理
  JNIEnv *pEnv = getJNIEnv();
  if (pEnv == nullptr) {
    return;
  }
  jclass jThreadHookClass = pEnv->FindClass(
      "com/knightboost/sliver/HookSuspendThreadTimeoutCallback");
  if (jThreadHookClass != nullptr) {
    jmethodID jMethodId = pEnv->GetMethodID(jThreadHookClass, "triggerSuspendTimeout",
                                            "()V");
    if (jMethodId != nullptr) {
      pEnv->CallVoidMethod(callbackObj, jMethodId);
    }
  }
}

void proxyThreadSuspendTimeoutWarning(void *self, LogSeverity severity, const char *message, jobject peer) {

  if (severity == FATAL && strcmp(message, SUSPEND_LOG_MSG) == 0) {
    // 如果当前是 FATAL 并且 message 是 Thread suspend timeout 则不调用原始函数,直接返回
    __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Fatal threadSuspendTimeout catch success : %s", message);
    triggerSuspendTimeout();
  } else {
    if (SHADOWHOOK_IS_SHARED_MODE) {
      SHADOWHOOK_CALL_PREV(proxyThreadSuspendTimeoutWarning, self, severity, message, peer);
    } else {
      reinterpret_cast<ThreadSuspendByPeerWarning>(originalFunction)(
          self, severity, message, peer
      );
    }
  }
  SHADOWHOOK_POP_STACK();
}

LogSeverity ToLogSeverity(int logLevel) {
  switch (logLevel) {
    case 0:return VERBOSE;
    case 1:return DEBUG;
    case 2:return INFO;
    case 3:return WARNING;
    case 4:return ERROR;
    case 5:return FATAL_WITHOUT_ABORT;
    case 6:return FATAL;
    default:return INFO;
  }
}

const char *getThreadSuspendByPeerWarningFunctionName() {
  int apiLevel = android_get_device_api_level();
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

/**
 * 防止目标线程调用suspend异常
 */
extern "C"
JNIEXPORT void JNICALL
Java_com_knightboost_sliver_Sliver_preventThreadSuspendTimeoutFatalLog(JNIEnv *env,
                                                                       jclass clazz, jobject callback) {
  if (callbackObj != nullptr) {
    env->DeleteGlobalRef(callbackObj);
  }
  callbackObj = env->NewGlobalRef(callback);

  // 如果已经调用Hook过 取消之前的hook
  if (stubFunction != nullptr) {
    shadowhook_unhook(stubFunction);
    stubFunction = nullptr;
  }
  stubFunction = shadowhook_hook_sym_name("libart.so",
                                          getThreadSuspendByPeerWarningFunctionName(),
                                          (void *) proxyThreadSuspendTimeoutWarning,
                                          (void **) &originalFunction);
  if (stubFunction == nullptr){ //在线下测试中发现，部分Android 13的设备，实际符号不同,和Android 14的符号一样,待确认原因
    int apiLevel = android_get_device_api_level();
    if (apiLevel == 33) {
      //SYMBOL_THREAD_SUSPEND_BY_PEER_WARNING_14
      stubFunction = shadowhook_hook_sym_name("libart.so",
                                              SYMBOL_THREAD_SUSPEND_BY_PEER_WARNING_14,
                                              (void *) proxyThreadSuspendTimeoutWarning,
                                              (void **) &originalFunction);
    }

  }
  if (stubFunction == nullptr) {
    const int err_num = shadowhook_get_errno();
    const char *errMsg = shadowhook_to_errmsg(err_num);
    if (errMsg == nullptr || callbackObj == nullptr) {
      return;
    }
    __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Hook setup failed: %s", errMsg);

    // 处理钩子设置失败的情况
    jclass jThreadHookClass = env->FindClass(
        "com/knightboost/sliver/HookSuspendThreadTimeoutCallback");
    if (jThreadHookClass != nullptr) {
      jmethodID jMethodId = env->GetMethodID(jThreadHookClass, "onError",
                                             "(Ljava/lang/String;)V");
      if (jMethodId != nullptr) {
        env->CallVoidMethod(callbackObj, jMethodId, env->NewStringUTF(errMsg));
      }
    }
    //cleanup
    if (callbackObj) {
      env->DeleteGlobalRef(callbackObj);
      callbackObj = nullptr;
    }
  } else {
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Hook setup success");
  }
}

}
