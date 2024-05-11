#include <jni.h>
#include "art_runtime.h"
#include "debugger.h"
//
// Created by Knight-ZXW on 2023/6/8.
//

extern "C"

using namespace art;

extern "C" JNIEXPORT jboolean JNICALL
Java_com_knightboost_artvm_KbArt_nSetJdwpAllowed(JNIEnv *env, jclass clazz, jboolean allowed) {
  art::Dbg::Dbg::SetJdwpAllowed(allowed);
  return true;
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_knightboost_artvm_KbArt_nSetJavaDebuggable(JNIEnv *env,
                                                    jclass clazz,
                                                    jboolean debuggable) {
  ArtRuntime::Get()->SetJavaDebuggable(debuggable);
  return true;
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_knightboost_artvm_KbArt_nIsJdwpAllow(JNIEnv *env, jclass clazz) {
  return art::Dbg::Dbg::IsJdwpAllowed();
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_knightboost_artvm_KbArt_nDisableClassVerify(JNIEnv *env, jclass clazz) {
  return ArtRuntime::Get()->DisableClassVerify();
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_knightboost_artvm_KbArt_nDelayJit(JNIEnv *env, jclass clazz) {
  return ArtRuntime::Get()->DelayJit();
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_knightboost_artvm_KbArt_nResumeJit(JNIEnv *env, jclass clazz) {
  return ArtRuntime::Get()->ResumeJit();
}