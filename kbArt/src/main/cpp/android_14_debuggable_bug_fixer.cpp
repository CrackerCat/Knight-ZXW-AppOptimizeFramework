#pragma once
#include "art.h"
#include "class_linker.h"
#include <android/api-level.h>
#include "logger.h"
#include "class.h"
using namespace kbArt;
class FixClassVisitor :public ClassVisitor{
  bool operator()(ObjPtr<mirror::Class> klass) override  {
    klass.reference_;
    LOGD("visitor","kclass %s", mirror::PrettyClass(reinterpret_cast<void *>(klass.reference_)));
    return true;
  }
};

void fix(){
  auto api_level = android_get_device_api_level();
  if (api_level == 34){
    LOGE("zxw","是Android 34");
    FixClassVisitor visitor;
    void *class_linker = reinterpret_cast<PartialRuntimeTiramisu *>(ArtHelper::partialRuntime)->class_linker_;
    VisitClasses(class_linker,&visitor);
  }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_knightboost_artvm_Android14DebuggableBugFixer_fix(JNIEnv *env, jclass clazz) {
  ArtHelper::init(env);

  fix();
}