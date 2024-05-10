//
// Created by Administrator on 2024/4/6.
//
#include "jni_id_manager.h"
#include "art_method.h"
ArtMethod *jni::JniIdManager::DecodeMethodId(jmethodID methodId)  {
  if (getAndroidApiLevel() < ANDROID_R) {
    return reinterpret_cast<ArtMethod *>(methodId);
  }
  static void *(*_decodeMethodId)(void *, jmethodID) = nullptr;
  if (_decodeMethodId == nullptr) {
    _decodeMethodId = reinterpret_cast<void *(*)(void *, jmethodID) > (dsym(
        "_ZN3art3jni12JniIdManager14DecodeMethodIdEP10_jmethodID"
    ));
  }
  return static_cast<ArtMethod *>(_decodeMethodId(this, methodId));
};