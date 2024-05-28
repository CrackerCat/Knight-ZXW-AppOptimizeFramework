//
// Created by Knight-ZXW on 2023/6/1.
//
#include <xdl.h>
#include "art_thread.h"
#include "art_xdl.h"
#include "art_def.h"
#include <pthread.h>
#define pthread_key_self_Symbol "_ZN3art6Thread17pthread_key_self_E"
uint32_t art::Thread::GetThreadId() {
  static uint32_t offset = 0;
  if (offset == 0) {
    int api_level = android_get_device_api_level();
    if (LIKELY(api_level >= __ANDROID_API_R__)) { // >=Android 12
      offset = offsetof(tls_32bit_sized_values_12_14, thin_lock_thread_id);
    } else if (api_level >= __ANDROID_API_L__) { // >=5.0 and <=Android 10
      offset = offsetof(tls_32bit_sized_values_android_5_11, thin_lock_thread_id);
    } else {  //5.0以下未支持
      //TODO 未兼容
    }
  }
  return *(uint32_t *) ((char *) this + offset);
}

uint32_t art::Thread::GetTid() {
  static uint32_t offset = -1;
  if (offset == -1) {
    int api_level = android_get_device_api_level();
    if (api_level >= __ANDROID_API_R__) { // >=Android 12
      offset = offsetof(tls_32bit_sized_values_12_14, tid);
    } else if (api_level >= __ANDROID_API_L__) { // <=Android 10
      offset = offsetof(tls_32bit_sized_values_android_5_11, tid);
    }
  }
  return *(uint32_t *) ((char *) this + offset);
}

art::Thread *art::Thread::Current() {

  if (apiLevel >=__ANDROID_API_N__){ //>=24 (7.0)
    //ifdef  __BIONIC__;
    void *thread = __get_tls()[TLS_SLOT_ART_THREAD_SELF];
    return reinterpret_cast<Thread *>(thread);
  } else { // api23(Android 6.0) 及 以下 使用的 pthread_key_self , https://cs.android.com/android/platform/superproject/+/android-6.0.0_r1:art/runtime/thread-inl.h
    static pthread_key_t *pthread_key_self_symbol_address = nullptr;
    if (pthread_key_self_symbol_address == nullptr) {
      pthread_key_self_symbol_address = static_cast<pthread_key_t *>(findArtSoSym(pthread_key_self_Symbol));
      if (pthread_key_self_symbol_address == nullptr){
        // TODO 不兼容的版本
      }
    }
    void *thread = pthread_getspecific(*pthread_key_self_symbol_address);
    return reinterpret_cast<Thread *>(thread);
  }

}

uint64_t art::Thread::GetCpuMicroTime() {
  static GetCpuMicroTime_t get_cpu_micro_time = nullptr;
  if (UNLIKELY(get_cpu_micro_time == nullptr)) {
    get_cpu_micro_time =
        reinterpret_cast<GetCpuMicroTime_t>(xdl_dsym(open_art_handle(),
                                                     "_ZNK3art6Thread15GetCpuMicroTimeEv",
                                                     nullptr));
  }
  return get_cpu_micro_time(this);
}
//检查各种符号兼容性
void art::Thread::init() {
  //TODO 判断 Thread Current 的获取方式
  // 通过在Java层创建一个线程，验证各种兼容性
}
