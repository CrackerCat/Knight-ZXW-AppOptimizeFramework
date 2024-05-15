//
// Created by Knight-ZXW on 2024/5/15.
//

#ifndef APPOPTIMIZEFRAMEWORK_KBART_SRC_MAIN_CPP_INCLUDE_SCOPED_THREAD_STATE_CHANGE_H_
#define APPOPTIMIZEFRAMEWORK_KBART_SRC_MAIN_CPP_INCLUDE_SCOPED_THREAD_STATE_CHANGE_H_
#include "base/value_object.h"
namespace art {
class ScopedThreadState : public ValueObject {

 public:
  ScopedThreadState(void *thread){
    self_ =thread;
  }
  // Self thread, can be null.
  void * self_;
  // The full JNIEnv.
  void * env_;
  // The full JavaVM.
  void * vm_;
};
}

#endif //APPOPTIMIZEFRAMEWORK_KBART_SRC_MAIN_CPP_INCLUDE_SCOPED_THREAD_STATE_CHANGE_H_
