//
// Created by Administrator on 2024/4/6.
//
#pragma once
#include <art_def.h>
#include "jni.h"
#include "../art_method.h"
#include "common.h"
#include "art_xdl.h"
namespace art {
namespace mirror {
namespace jni {

class JniIdManager {
 public:
  ArtMethod *DecodeMethodId(jmethodID methodId) {
    if (getAndroidApiLevel() < ANDROID_API_R) {
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
};

}
}
}