//
// Created by Administrator on 2022/12/28.
// Email: nimdanoob@163.com
//

#include "art.h"
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

namespace art {
void *ArtHelper::runtime = nullptr;
void *ArtHelper::partialRuntime = nullptr;
char *ArtHelper::artPath = nullptr;
art::jni::JniIdManager *ArtHelper::jniIdManager = nullptr;
static void *thread_list = nullptr;

static int api_level = 0;

static std::mutex mutex;
static std::mutex mtx;
static bool initialized = false;


#define TAG "ArtHelper"

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


int ArtHelper::init(JNIEnv *env) {
  //TODO 保证只初始化一次
  if (initialized) {
    return 0;
  }

  std::lock_guard<std::mutex> lock(mtx);
  api_level = getAndroidApiLevel();
  JavaVM *javaVM;
  env->GetJavaVM(&javaVM);

  auto *javaVMExt = reinterpret_cast<JavaVMExt *>(javaVM);
  runtime = javaVMExt->runtime;
  const int MAX = 2000;

  int offsetOfVmExt = findOffset(runtime, 0, MAX, javaVMExt);
  if (offsetOfVmExt < 0) {
    LOGV("ArtHelper", "find offset of VmExt failed: %d", offsetOfVmExt);
    return -1;
  }

  if (api_level >= ANDROID_API_TIRAMISU) {
    ArtHelper::partialRuntime =
        reinterpret_cast<char *>(runtime) + offsetOfVmExt -
            offsetof(PartialRuntimeTiramisu, java_vm_);
    thread_list =
        reinterpret_cast<PartialRuntimeTiramisu *>(ArtHelper::partialRuntime)->thread_list_;
  } else if (api_level >= ANDROID_API_R) {
    ArtHelper::partialRuntime =
        reinterpret_cast<char *>(runtime) + offsetOfVmExt -
            offsetof(PartialRuntimeR, java_vm_);
    thread_list = reinterpret_cast<PartialRuntimeR *>(ArtHelper::partialRuntime)->thread_list_;
  } else if (api_level >= ANDROID_API_Q) {
    ArtHelper::partialRuntime =
        reinterpret_cast<char *>(runtime) + offsetOfVmExt -
            offsetof(PartialRuntimeQ, java_vm_);
    thread_list = reinterpret_cast<PartialRuntimeQ *>(ArtHelper::partialRuntime)->thread_list_;
  } else if (api_level >= __ANDROID_API_O_MR1__) {
    ArtHelper::partialRuntime =
        reinterpret_cast<char *>(runtime) + offsetOfVmExt -
            offsetof(PartialRuntimeP, java_vm_);
    thread_list = reinterpret_cast<PartialRuntimeP *>(ArtHelper::partialRuntime)->thread_list_;
  } else {
    //TODO 未验证的系统版本
    return -1;
  }

  initialized = true;
  return 0;
}

ThreadList *ArtHelper::getThreadList() {
  return static_cast<ThreadList *>(thread_list);
}

jni::JniIdManager  *ArtHelper::getJniIdManager() {
  if (api_level < ANDROID_API_TIRAMISU) {
    return nullptr;
  }
  if (runtime == nullptr) {
    void *address = nullptr;
    if (api_level >= ANDROID_API_TIRAMISU) {
      address =
          static_cast<void *>(
              (reinterpret_cast<PartialRuntimeTiramisu *>(ArtHelper::partialRuntime))->jni_id_manager_);
    } else if (api_level >= ANDROID_API_R) {
      address = static_cast<art::jni::JniIdManager *>(
          (reinterpret_cast<PartialRuntimeR *>(ArtHelper::partialRuntime))->jni_id_manager_);
    }
    if (address != nullptr) {
      jniIdManager = static_cast<art::jni::JniIdManager *>(address);
    }
  }
  return static_cast<jni::JniIdManager *>(jniIdManager);
}



bool ArtHelper::SetJavaDebuggable(bool debuggable) {
  static void (*setJavaDebuggable)(void *, bool) = nullptr;
  if (setJavaDebuggable == nullptr) {
    setJavaDebuggable = reinterpret_cast<void (*)(void *, bool)>(dsym("_ZN3art7Runtime17SetJavaDebuggableEb"));
  }
  if (setJavaDebuggable != nullptr) {
    setJavaDebuggable(runtime, debuggable);
    return true;
  }
  return false;
}


bool ArtHelper::DisableClassVerify() {
  static void (*DisableVerifierEv)(void *) = nullptr;
  if (DisableVerifierEv == nullptr) {
    DisableVerifierEv = reinterpret_cast<void (*)(void *)>(dsym("_ZN3art7Runtime15DisableVerifierEv"));
  }
  if (DisableVerifierEv != nullptr) {
    DisableVerifierEv(ArtHelper::runtime);
    return true;
  }
  return false;
}
bool ArtHelper::EnableClassVerify() {
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

bool ArtHelper::DelayJit() {
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
bool ArtHelper::ResumeJit() {
  if (jitStub != nullptr) {
    shadowhook_unhook(jitStub);
    return true;
  }
  return false;
}

}
