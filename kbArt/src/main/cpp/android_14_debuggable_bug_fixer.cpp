#pragma once
#include <chrono>

#include "cstddef"
#include "art.h"
#include "class_linker.h"
#include "object.h"
#include "class.h"
#include "class-inl.h"
#include <android/api-level.h>
#include "logger.h"
#include "thread_list.h"
using namespace art;
using namespace art::mirror;
class FixClassVisitor :public art::ClassVisitor{
  bool operator()(art::ObjPtr<art::mirror::Class> klass) override  {
    for (auto &m : ((art::mirror::Class*)klass.reference_)->GetMethods(kRuntimePointerSize)){
      if (m.IsMemorySharedMethod()){
        m.ClearMemorySharedMethod();
      }
    }
    return true;
  }
};

void fix(){
  auto api_level = android_get_device_api_level();
  if (api_level == 34){
    auto start = std::chrono::steady_clock::now();
    FixClassVisitor visitor;
    void *class_linker = reinterpret_cast<PartialRuntimeTiramisu *>(ArtHelper::partialRuntime)->class_linker_;
    VisitClasses(class_linker,&visitor);
    auto end = std::chrono::steady_clock::now();
    LOGI("Android14BugFixer","clear memory shared methods cost %lld us",
         std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
  }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_knightboost_artvm_Android14DebuggableBugFixer_fix(JNIEnv *env, jclass clazz) {
  ArtHelper::init(env);

  fix();
}