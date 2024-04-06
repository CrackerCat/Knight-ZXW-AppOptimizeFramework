//
// Created by Administrator on 2024/4/6.
//
#pragma once
#include "suspend_reason.h"
#include "art_thread.h"
namespace art {

class ThreadList {
 public:
  void SuspendThreadByThreadId(uint32_t threadId,
                               SuspendReason suspendReason,
                               bool *timed_out) ;
  void Resume(void *thread, SuspendReason suspendReason) ;
};

}