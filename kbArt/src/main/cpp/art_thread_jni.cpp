#include <jni.h>
#include "art_thread.h"
#include "logger.h"
#include "art_runtime.h"
//
// Created by Knight-ZXW on 2023/6/1.
//


static jfieldID nativePeerField = NULL;

extern "C"
JNIEXPORT jint JNICALL
Java_com_knightboost_artvm_ArtThread_getTid(JNIEnv *env,
                                            jclass clazz,
                                            jobject thread) {
  if (nativePeerField == nullptr) {
    jclass threadClass = env->GetObjectClass(thread);
    nativePeerField = env->GetFieldID(threadClass, "nativePeer", "J");
  }
  jlong nativePeerValue = env->GetLongField(thread, nativePeerField);
  if (nativePeerValue == 0){ //native thread has not yet been created/started, or has been destroyed.
    return -1;
  }
  auto *artThread = reinterpret_cast<art::Thread *>(nativePeerValue);
  uint32_t tid = artThread->GetTid();
  //double check, 保证上面计算tid时，线程还未被销毁，避免计算出一个异常的tid
  nativePeerValue = env->GetLongField(thread, nativePeerField);
  if (nativePeerValue == 0){ //native thread has not yet been created/started, or has been destroyed.
    return -1;
  }
  return tid;
}
extern "C"
JNIEXPORT jlong JNICALL
Java_com_knightboost_artvm_ArtThread_getCpuMicroTime(JNIEnv *env, jclass clazz, jlong nativePeerValue) {
  auto *artThread = reinterpret_cast<art::Thread *>(nativePeerValue);
  return artThread->GetCpuMicroTime();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_knightboost_artvm_ArtThread_resumeThread(JNIEnv *env, jclass clazz, jlong thread_native_peer) {
  // TODO: implement resumeThread()
  art::ArtRuntime::Get()->GetThreadList()->Resume((void *)thread_native_peer,art::SuspendReason::kForUserCode);
}
extern "C"
JNIEXPORT jlong JNICALL
Java_com_knightboost_artvm_ArtThread_suspendThreadByThreadId(JNIEnv *env, jclass clazz, jint thread_id) {
  // TODO: implement suspendThreadByThreadId()
  bool timeOut;
  art::ThreadList *thread_list = art::ArtRuntime::Get()->GetThreadList();
  void *thread = thread_list->SuspendThreadByThreadId(thread_id, art::SuspendReason::kForUserCode, &timeOut);
  if (thread == nullptr) {
    return -1;
  } else {
    return reinterpret_cast<jlong>(thread);
  }
}