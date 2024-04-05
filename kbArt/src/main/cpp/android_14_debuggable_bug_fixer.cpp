#pragma once
#include "cstddef"
#include "art.h"
#include "class_linker.h"
#include "object.h"
#include "class.h"
#include "class-inl.h"
#include <android/api-level.h>
#include "logger.h"
using namespace kbArt;
using namespace art;
using namespace art::mirror;
class FixClassVisitor :public art::ClassVisitor{
  bool operator()(art::ObjPtr<art::mirror::Class> klass) override  {
    for (auto &m : ((art::mirror::Class*)klass.reference_)->GetMethods(kRuntimePointerSize)){
      if (m.IsMemorySharedMethod()){
        LOGE("zxw","is memorySharedMethod is %s <- class %s",m.PrettyMethod(true).c_str(),
             PrettyClass((void *)klass.reference_).c_str());
      }
      m.ClearMemorySharedMethod();
    }

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