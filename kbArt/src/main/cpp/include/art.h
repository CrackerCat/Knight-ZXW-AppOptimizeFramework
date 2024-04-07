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
#include "art_method.h"
#include "art_def.h"
#include "jni/jni_id_manager.h"
#include "thread_list.h"
namespace art {

class ArtHelper {

 public:
  static int init(JNIEnv *env);

  static ThreadList *getThreadList();

  static jni::JniIdManager *getJniIdManager();

  static bool SetJavaDebuggable(bool debuggable);

  static bool DisableClassVerify();

  static bool EnableClassVerify();

  static bool DelayJit();

  static bool ResumeJit();

 private:

 public:
  static void *runtime;
  static void *partialRuntime;
  static jni::JniIdManager *jniIdManager;
 private:
  static char *artPath;

};
}

#endif //KB_ART_H_
