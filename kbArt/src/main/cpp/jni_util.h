//
// Created by Knight-ZXW on 2024/5/14.
//
#include "jni.h"
#include "kbart_jni.h"
#ifndef APPOPTIMIZEFRAMEWORK_KBART_SRC_MAIN_CPP_JNI_UTIL_H_
#define APPOPTIMIZEFRAMEWORK_KBART_SRC_MAIN_CPP_JNI_UTIL_H_


JNIEnv* getJNIEnv() {
  JNIEnv* env;
  kbArt::g_javaVM->AttachCurrentThread(&env, NULL); // 附加当前线程的JNIEnv
  return env;
}
#endif //APPOPTIMIZEFRAMEWORK_KBART_SRC_MAIN_CPP_JNI_UTIL_H_
