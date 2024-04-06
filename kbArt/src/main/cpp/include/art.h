//
// Created by Administrator on 2022/12/28.
// Email: nimdanoob@163.com
//

#ifndef KB_ART_H_
#define KB_ART_H_
#include <stdint.h>
#include <jni.h>
#include <string>
#include "stackvisitor.h"
#include "shadow_frame.h"
#include "common.h"
#include "art_method.h"
#include "art_def.h"
#include "thread_list.h"
#include "jni/jni_id_manager.h"
namespace kbArt {

class ArtHelper {

 public:
  static int init(JNIEnv *env);

  static void* getArtSoHandle();

  static gc::collector::ThreadList * getThreadList();

  static void StackVisitorWalkStack(StackVisitor *visitor, bool include_transitions);


  static mirror::jni::JniIdManager  *getJniIdManager();


  static bool SetJdwpAllowed(bool allowed);

  static bool IsJdwpAllow();

  static bool SetJavaDebuggable(bool debuggable);

  static bool DisableClassVerify();

  static bool  EnableClassVerify();

  static bool  DelayJit();

  static bool  ResumeJit();


 private:
  static int load_symbols();

 public:
  static void *runtime;
  static void *partialRuntime;
  static art::mirror::jni::JniIdManager * jniIdManager;
 private:
  static char* artPath;


};
}

#endif //KB_ART_H_
