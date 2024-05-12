//
// Created by Administrator on 2022/12/28.
// Email: nimdanoob@163.com
//

#include "art_runtime.h"
#include "stack/stackvisitor.h"

#include "logger.h"
#include <vector>
#include <string>
#include <cstdlib>
#include <sys/system_properties.h>
#include <shadowhook.h>
#include <unistd.h>
#include "xdl.h"
#include "mutex"
#include "thread_list.h"
#include "art_xdl.h"
#include "cxx_helper.h"
#include "art_method_struct.h"
namespace art {

#define TAG "ArtRuntime"

//source from: https://github.com/tiann/FreeReflection/blob/master/library/src/main/cpp/art.cpp
template<typename T>
int findOffset(void *start, int regionStart, int regionEnd, T target) {
  if (nullptr == start || regionStart < 0 || regionEnd <= 0) {
    return -1;
  }
  char *c_start = reinterpret_cast<char *>(start);
  for (int i = regionStart; i < regionEnd; i += 4) {
    T *current_value = reinterpret_cast<T *>(c_start + i);
    if (target == *current_value) {
      LOGV("artHelper", "find target success, address is %p", current_value);
      return i;
    }
  }
  return -1;
}

bool ArtRuntime::SetJavaDebuggable(bool debuggable) {
  static void (*setJavaDebuggable)(void *, bool) = nullptr;
  if (setJavaDebuggable == nullptr) {
    setJavaDebuggable =
        reinterpret_cast<void (*)(void *, bool)>(dsym("_ZN3art7Runtime17SetJavaDebuggableEb"));
  }
  if (LIKELY(setJavaDebuggable != nullptr)) {
    setJavaDebuggable(runtime, debuggable);
    return true;
  }
  return false;
}

bool ArtRuntime::DisableClassVerify() {
  static void (*DisableVerifierEv)(void *) = nullptr;
  if (DisableVerifierEv == nullptr) {
    DisableVerifierEv =
        reinterpret_cast<void (*)(void *)>(dsym("_ZN3art7Runtime15DisableVerifierEv"));
  }
  if (LIKELY(DisableVerifierEv != nullptr)) {
    DisableVerifierEv(ArtRuntime::runtime);
    return true;
  }
  return false;
}
bool ArtRuntime::EnableClassVerify() {
  return false;
}

void *originJit = nullptr;
void *jitStub = nullptr;
void delayJitFunc(void *task, void *thread) {
  LOGI(TAG, " jit invoked, delay it ");
  sleep(5);
  LOGI(TAG, "jit task wake up");
  ((void (*)(void *, void *)) originJit)(task, thread);
  LOGI(TAG, "jit task done");
}

bool ArtRuntime::DelayJit() {
  const char *jit = "_ZN3art3jit14JitCompileTask3RunEPNS_6ThreadE";
  jitStub = shadowhook_hook_sym_name("libart.so", jit,
                                     (void *) delayJitFunc,
                                     (void **) &originJit);
  if (jitStub != nullptr) {
    return true;
  } else {
    return false;
  }
}
bool ArtRuntime::ResumeJit() {
  if (jitStub != nullptr) {
    shadowhook_unhook(jitStub);
    return true;
  }
  return false;
}
ArtRuntime *ArtRuntime::Get() {
  static ArtRuntime instance;
  return &instance;
}
bool ArtRuntime::OnLoad(JavaVM *vm, JNIEnv *env, jclass java_class) {
  api_level_ = getAndroidApiLevel();
  JavaVM *javaVM;
  env->GetJavaVM(&javaVM);

  auto *javaVMExt = reinterpret_cast<JavaVMExt *>(javaVM);
  runtime = javaVMExt->runtime;
  runtime_objects_.runtime_ = javaVMExt->runtime;
  const int MAX = 2000;

  int offsetOfVmExt = findOffset(runtime, 0, MAX, javaVMExt);
  if (offsetOfVmExt < 0) {
    LOGV("ArtRuntime", "find offset of VmExt failed: %d", offsetOfVmExt);
    return false;
  }

  //runtime offset init
  if (api_level_ >= ANDROID_TIRAMISU) {
    ArtRuntime::partialRuntime =
        reinterpret_cast<char *>(runtime) + offsetOfVmExt -
            offsetof(PartialRuntimeTiramisu, java_vm_);
    runtime_objects_.thread_list_ =
        reinterpret_cast<PartialRuntimeTiramisu *>(ArtRuntime::partialRuntime)->thread_list_;
    runtime_objects_.class_linker_ =reinterpret_cast<PartialRuntimeTiramisu *>(ArtRuntime::partialRuntime)->class_linker_;

  } else if (api_level_ >= ANDROID_R) {
    ArtRuntime::partialRuntime =
        reinterpret_cast<char *>(runtime) + offsetOfVmExt -
            offsetof(PartialRuntimeR, java_vm_);
    runtime_objects_.thread_list_ = reinterpret_cast<PartialRuntimeR *>(ArtRuntime::partialRuntime)->thread_list_;
    runtime_objects_.class_linker_ =reinterpret_cast<PartialRuntimeTiramisu *>(ArtRuntime::partialRuntime)->class_linker_;
  } else if (api_level_ >= ANDROID_Q) {
    ArtRuntime::partialRuntime =
        reinterpret_cast<char *>(runtime) + offsetOfVmExt -
            offsetof(PartialRuntimeQ, java_vm_);
    runtime_objects_.thread_list_ = reinterpret_cast<PartialRuntimeQ *>(ArtRuntime::partialRuntime)->thread_list_;
    runtime_objects_.class_linker_ =reinterpret_cast<PartialRuntimeTiramisu *>(ArtRuntime::partialRuntime)->class_linker_;
  } else if (api_level_ >= ANDROID_O_MR1) {
    ArtRuntime::partialRuntime =
        reinterpret_cast<char *>(runtime) + offsetOfVmExt -
            offsetof(PartialRuntimeP, java_vm_);
    runtime_objects_.thread_list_ = reinterpret_cast<PartialRuntimeP *>(ArtRuntime::partialRuntime)->thread_list_;
    runtime_objects_.class_linker_ =reinterpret_cast<PartialRuntimeTiramisu *>(ArtRuntime::partialRuntime)->class_linker_;
  } else {
    //TODO 未验证的系统版本
    return false;
  }

  LOGE("zxw","开始尝试获取偏移");
  //art_method init
  if (api_level_ >= ANDROID_S && api_level_ <= ANDROID_UPSIDE_DOWN_CAKE) {
    method_offset_.declaring_class_offset_ = offsetof(ArtMethod_12, declaring_class_);
    method_offset_.access_flags_offset_ = offsetof(ArtMethod_12, access_flags_);
  } else if (api_level_ >= ANDROID_Q) {
    method_offset_.declaring_class_offset_ = offsetof(ArtMethod_10, declaring_class_);
    method_offset_.access_flags_offset_ = offsetof(ArtMethod_10, access_flags_);
  } else if (api_level_ == ANDROID_P) {
    method_offset_.declaring_class_offset_ = offsetof(ArtMethod_9, declaring_class_);
    method_offset_.access_flags_offset_ = offsetof(ArtMethod_9, access_flags_);
  } else if (api_level_ >= ANDROID_O) {
    method_offset_.declaring_class_offset_ = offsetof(ArtMethod_8, declaring_class_);
    method_offset_.access_flags_offset_ = offsetof(ArtMethod_8, access_flags_);
  } else if (api_level_ >=  ANDROID_N_MR1){
    method_offset_.declaring_class_offset_ = offsetof(ArtMethod_7_1, declaring_class_);
    method_offset_.access_flags_offset_ = offsetof(ArtMethod_7_1, access_flags_);
  } else if (api_level_ >=  ANDROID_N){
    method_offset_.declaring_class_offset_ = offsetof(ArtMethod_7, declaring_class_);
    method_offset_.access_flags_offset_ = offsetof(ArtMethod_7, access_flags_);
  } else if (api_level_ >=  ANDROID_M){
    method_offset_.declaring_class_offset_ = offsetof(ArtMethod_6, declaring_class_);
    method_offset_.access_flags_offset_ = offsetof(ArtMethod_6, access_flags_);
  }
  //TODO 对偏移进行检查，如果偏移不准确，则认为该系统可能魔改后，位移不一致。

  LOGE("zxw","获取偏移成功");



  //art method offset init
  return true;
}

}
