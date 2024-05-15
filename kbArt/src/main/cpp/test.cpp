#include <jni.h>
#include <android/log.h>
#include <shadowhook.h>
#include "opt/suspend_time_out_fix.h"
#include "logger.h"
#include "art_thread.h"
#include <iostream>
#include <thread>
#include <chrono>
#include "scoped_thread_state_change.h"
#include "art_xdl.h"
//
// Created by Knight-ZXW on 2024/5/14.
//

typedef void (*ThreadSuspendByPeerWarning)(void *self,
                                           enum LogSeverity severity,
                                           const char *message,
                                           jobject peer);

extern "C"
JNIEXPORT void JNICALL
Java_com_knightboost_test_Test_callNativeThreadSuspendTimeout(JNIEnv *env, jclass clazz, jobject target_thread, jlong native_peer) {
  void *handle = shadowhook_dlopen("libart.so");
  auto hookPointFunc = (ThreadSuspendByPeerWarning) shadowhook_dlsym(handle,
                                                                     kbArt::getThreadSuspendByPeerWarningFunctionName());
  if (hookPointFunc != nullptr) {
    void *child_thread = reinterpret_cast<void *>(native_peer);
    // only 14 worked for test.
    __android_log_print(ANDROID_LOG_INFO, "TEST", "thread_point : %p", child_thread);
    hookPointFunc(child_thread, FATAL, SUSPEND_LOG_MSG, target_thread);
  } else {
    __android_log_print(ANDROID_LOG_ERROR, "TEST", "ELF symbol not found!");
  }
  shadowhook_dlclose(handle);
}

//_ZN3art6Thread17FromManagedThreadERKNS_33ScopedObjectAccessAlreadyRunnableEP8_jobject

void *orig = NULL;
void *stub = NULL;
jint  targetThreadId = 0;
typedef void *(*FromManagedThread)(void *soa, jobject peer);
void *proxyFromManagedThread(void *soa, jobject peer) {
  void *r = ((FromManagedThread) orig)(soa, peer);
  art::Thread *current = art::Thread::Current();
  LOGE("zxw", "调用了 proxyFromManagedThread peer = %p , currentThread is %d", peer,current->GetTid());
  if (current->GetTid() ==targetThreadId){
    LOGE("zxw","命中目标线程，目标线程开始睡眠");
    std::this_thread::sleep_for(std::chrono::seconds(6));
    LOGE("zxw","命中目标线程，目标线程睡眠结束");
  } else{
    LOGE("zxw","未命中直接返回");
  }
  return r;
}

