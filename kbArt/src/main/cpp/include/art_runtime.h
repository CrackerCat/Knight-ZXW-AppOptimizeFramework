//
// Created by Administrator on 2022/12/28.
// Email: nimdanoob@163.com
//

#ifndef KB_ART_H_
#define KB_ART_H_
#include <stdint.h>
#include <jni.h>
#include <string>
#include "stack/stackvisitor.h"
#include "shadow_frame.h"
#include "common.h"
#include "art_def.h"
#include "jni/jni_id_manager.h"
#include "thread_list.h"
#include "primitive_types.h"
namespace art {

struct RuntimeObjects final {
  ptr_t OPTION runtime_;
  ptr_t OPTION heap_;
  ptr_t OPTION thread_list_;
  ptr_t OPTION class_linker_;
  ptr_t OPTION intern_table_;
};

struct ArtMethodOffsets final {
  size_t method_size_;
  offset_t declaring_class_offset_;
  offset_t jni_code_offset_;
  offset_t quick_code_offset_;
  offset_t OPTION interpreter_code_offset_;
  offset_t access_flags_offset_;
  offset_t dex_code_item_offset_offset_;
  offset_t dex_method_index_offset_;
  offset_t method_index_offset_;
  offset_t OPTION hotness_count_offset_;
};

class ArtRuntime {

 public:

  static ArtRuntime *Get();
  ArtRuntime() {}
  bool OnLoad(JavaVM *vm, JNIEnv *env, jclass java_class);
//  jni::JniIdManager *getJniIdManager();

  bool SetJavaDebuggable(bool debuggable);
  bool DisableClassVerify();
  bool EnableClassVerify();
  bool DelayJit();
  bool ResumeJit();

  ThreadList * GetThreadList(){
    return (ThreadList *)runtime_objects_.thread_list_;
  }

  void* GetClassLinker(){
    return runtime_objects_.class_linker_;
  }
  ArtMethodOffsets*  MethodOffsets(){
    return &method_offset_;
  }

 private:
  JavaVM *vm_;
  void *runtime;
  ArtMethodOffsets method_offset_;
  void *art_elf_image_;
  s4 api_level_;
  jni::JniIdManager *jniIdManager;
  RuntimeObjects runtime_objects_;
 public:
  void *partialRuntime;


};
}

#endif //KB_ART_H_
