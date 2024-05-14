//
// Created by Knight-ZXW on 2024/5/14.
//

#include "suspend_time_out_fix.h"
#include "art_runtime.h"
#include "shadowhook.h"
#include "jni_util.h"
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
std::atomic<LogSeverity> m_severity{INFO}; // 日志严重性级别的原子变量，默认为INFO


typedef void (*ThreadSuspendByPeerWarning)(void *self, LogSeverity severity,
                                           const char *message, jobject peer); // 函数指针类型定义

void triggerSuspendTimeout();

void hookPointFailed(const char *msg); // 钩子设置失败时的处理函数

void releaseHook(); // 释放钩子的函数

void cleanup(JNIEnv *env); // 清理资源的函数



void hookPointFailed(const char *errMsg) {
  // 处理钩子设置失败的情况
  JNIEnv *pEnv = getJNIEnv();
  if (pEnv == nullptr) {
    return;
  }
  jclass jThreadHookClass = pEnv->FindClass(
      "com/knightboost/sliver/HookSuspendThreadTimeoutCallback");
  if (jThreadHookClass != nullptr) {
    jmethodID jMethodId = pEnv->GetMethodID(jThreadHookClass, "onError",
                                            "(Ljava/lang/String;)V");
    if (jMethodId != nullptr) {
      pEnv->CallVoidMethod(callbackObj, jMethodId, pEnv->NewStringUTF(errMsg));
    }
  }
  cleanup(pEnv);
}


//通知原始函数异常
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


// Hook 函数实现，替换原始函数
void threadSuspendByPeerWarning(void *self, LogSeverity severity, const char *message,
                                jobject peer) {
  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Hooked point success : %s", message);
  if (severity == FATAL && strcmp(message, SUSPEND_LOG_MSG) == 0) {
    // 如果当前是 FATAL 并且 message 是 Thread suspend timeout 则设置一个非FATAL级别的。
    severity = m_severity.load();
    triggerSuspendTimeout();
  }
  ((ThreadSuspendByPeerWarning) originalFunction)(self, severity, message, peer);
}

void maskThreadSuspendTimeout(void *self, LogSeverity severity, const char *message, jobject peer) {
  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Hooked point success : %s", message);
  if (severity == FATAL && strcmp(message, SUSPEND_LOG_MSG) == 0) {
    // 如果当前是 FATAL 并且 message 是 Thread suspend timeout 则不调用原始函数,直接返回
    triggerSuspendTimeout();
  }
}


void preparedMaskThreadTimeoutAbort() {
  releaseHook();
  stubFunction = shadowhook_hook_sym_name(TARGET_ART_LIB,
                                          getThreadSuspendByPeerWarningFunctionName(),
                                          (void *) maskThreadSuspendTimeout,
                                          (void **) &originalFunction);
  if (stubFunction == nullptr) {
    const int err_num = shadowhook_get_errno();
    const char *errMsg = shadowhook_to_errmsg(err_num);
    if (errMsg == nullptr || callbackObj == nullptr) {
      return;
    }
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Hook setup failed: %s", errMsg);
    hookPointFailed(errMsg);
    delete errMsg;
  } else {
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Hook setup success");
  }
}



void preventTargetThreadSuspendTimeOutFatal(long tid) {

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
  if (apiLevel < 23){
    return SYMBOL_THREAD_SUSPEND_BY_PEER_WARNING_5;
  } else if (apiLevel < 26) {
    // below android 8
    return SYMBOL_THREAD_SUSPEND_BY_PEER_WARNING_6_7;
  } else if (apiLevel < 34) {
    // above android 8 and below android 14
    return SYMBOL_THREAD_SUSPEND_BY_PEER_WARNING_8_13;
  } else {
    // android 14+
    return SYMBOL_THREAD_SUSPEND_BY_PEER_WARNING_14;
  }
}


void releaseHook() {
  // 实现释放钩子的功能
  if (stubFunction != nullptr) {
    shadowhook_unhook(stubFunction);
    stubFunction = nullptr;
  }
}
void prepareSetSuspendTimeoutLevel() { // 准备设置挂起超时级别的函数
  releaseHook();
  stubFunction = shadowhook_hook_sym_name(TARGET_ART_LIB,
                                          getThreadSuspendByPeerWarningFunctionName(),
                                          (void *) threadSuspendByPeerWarning,
                                          (void **) &originalFunction);
  if (stubFunction == nullptr) {
    const int err_num = shadowhook_get_errno();
    const char *errMsg = shadowhook_to_errmsg(err_num);
    if (errMsg == nullptr || callbackObj == nullptr) {
      return;
    }
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Hook setup failed: %s", errMsg);
    hookPointFailed(errMsg);
    delete errMsg;
  } else {
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Hook setup success");
  }
}


void cleanup(JNIEnv *env) {
  // 清理全局引用和分离线程
  if (callbackObj) {
    env->DeleteGlobalRef(callbackObj);
    callbackObj = nullptr;
  }
  if (kbArt::g_javaVM->DetachCurrentThread() != JNI_OK) {
    __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Could not detach current thread.");
  }
}

/**
 * 防止目标线程suspend异常
 */
extern "C"
JNIEXPORT void JNICALL
Java_com_knightboost_sliver_Sliver_preventThreadSuspendTimeoutFatal(JNIEnv *env, jclass clazz, jlong tid,jobject callback) {
  if (callbackObj != nullptr) {
    env->DeleteGlobalRef(callbackObj);
  }
  callbackObj = env->NewGlobalRef(callback);
  preparedMaskThreadTimeoutAbort();

}


}
