//
// Created by Knight-ZXW on 2023/6/1.
//
#include <jni.h>
#include "art_runtime.h"
#include "tls.h"
#include "common.h"
#include "logger.h"
JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
  JNIEnv *env = nullptr;
  if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
    return -1;
  }
  LOGD("zxw","开始初始化");
  art::ArtRuntime::Get()->OnLoad(vm,env, nullptr);
  return JNI_VERSION_1_6;
}

