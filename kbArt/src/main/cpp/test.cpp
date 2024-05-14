#include <jni.h>
#include <android/log.h>
#include <shadowhook.h>
#include "suspend_time_out_fix.h"

//
// Created by Knight-ZXW on 2024/5/14.
//


extern "C"
JNIEXPORT void JNICALL
Java_com_knightboost_test_Test_infiniteRun(JNIEnv *env, jclass clazz) {
  while (true) {
    // 这里的代码会无限循环执行
  }
}

typedef void (*ThreadSuspendByPeerWarning)(void *self,
                                           enum LogSeverity severity,
                                           const char *message,
                                           jobject peer);


extern "C" JNIEXPORT
void JNICALL
Java_com_thread_1hook_ThreadHook_callNativeThreadSuspendTimeout(JNIEnv *env,
                                                                jobject javaThread /* this */,
                                                                jlong nativePeer,
                                                                jobject peer) {
  void *handle = shadowhook_dlopen("libart.so");
  auto hookPointFunc = (ThreadSuspendByPeerWarning) shadowhook_dlsym(handle,
                                                                     kbArt::getThreadSuspendByPeerWarningFunctionName());
  if (hookPointFunc != nullptr) {
    void *child_thread = reinterpret_cast<void *>(nativePeer);
    // only 14 worked for test.
    __android_log_print(ANDROID_LOG_INFO, "TEST", "thread_point : %p", child_thread);
    hookPointFunc(child_thread, FATAL, SUSPEND_LOG_MSG, peer);
  } else {
    __android_log_print(ANDROID_LOG_ERROR, "TEST", "ELF symbol not found!");
  }
  shadowhook_dlclose(handle);
}
