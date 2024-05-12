//
// Created by Administrator on 2024/4/6.
//
#pragma once
#include "stddef.h"
#include "suspend_reason.h"
#include "art_thread.h"
namespace art {

class ThreadList {
 public:
  void* SuspendThreadByThreadId(uint32_t threadId,
                               SuspendReason suspendReason,
                               bool *timed_out) ;
  //TODO Android 8.1以上 和8.0 以下不一样
  // see https://android-review.googlesource.com/q/Ic7a1ae6a25e687d65f5aa10c1aad54a7b80dd086
  bool Resume(void *thread, SuspendReason suspendReason) ;
};

}