//
// Created by Knight-ZXW on 2023/6/1.
//
#include <jni.h>
#include "art_runtime.h"
#include "tls.h"
#include "common.h"
#include "logger.h"
#include "kbart_jni.h"
#include "art_def.h"
namespace art {
int apiLevel = 0;
}
namespace kbArt {
JavaVM *gJavaVM = NULL;
JNIEnv *getJNIEnv() {
  JNIEnv *env;
  kbArt::gJavaVM->AttachCurrentThread(&env, NULL); // 附加当前线程的JNIEnv
  return env;
}
}
JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
  kbArt::gJavaVM = vm;
  art::apiLevel = android_get_device_api_level();
  JNIEnv *env = nullptr;
  if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
    return -1;
  }
  art::ArtRuntime::Get()->OnLoad(vm, env, nullptr);
  return JNI_VERSION_1_6;
}


