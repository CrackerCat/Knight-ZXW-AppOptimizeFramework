//
// Created by Administrator on 2024/4/6.
//
#pragma once
#include <art_def.h>
#include "jni.h"
#include "common.h"
#include "art_xdl.h"
namespace art {
class ArtMethod;
namespace jni {

class JniIdManager {
 public:
  ArtMethod *DecodeMethodId(jmethodID methodId);
};

}
}