//
// Created by Administrator on 2024/4/6.
//
#include "thread_list.h"
#include "art_xdl.h"

namespace art {


void *ThreadList::SuspendThreadByThreadId(uint32_t threadId,
                                          SuspendReason suspendReason,
                                          bool *timed_out) {

  static void *(*suspend_thread_by_thread_id)(void *,
                                              uint32_t,
                                              SuspendReason,
                                              bool *) = nullptr;
  if (suspend_thread_by_thread_id == nullptr) {
    //TODO 8.1以下版本做区分，其函数签名不一致
    suspend_thread_by_thread_id =
        reinterpret_cast<void *(*)(void *,
                                   uint32_t,
                                   SuspendReason,
                                   bool *)>(dsym(
            "_ZN3art10ThreadList23SuspendThreadByThreadIdEjNS_13SuspendReasonEPb"));
  }
  //
  return suspend_thread_by_thread_id(this, threadId, suspendReason, timed_out);
}
// 8.1 以上版本
bool ThreadList::Resume(void *thread, SuspendReason suspendReason) {
  //TODO 8.1以下版本做区分，其函数签名不一致
  static bool (*resume)(void *, void *, SuspendReason) = nullptr;
  if (resume == nullptr) {
    resume = reinterpret_cast<bool (*)(void *thread_list,
                                       void *thread,
                                       SuspendReason suspendReason)>(dsym(
        "_ZN3art10ThreadList6ResumeEPNS_6ThreadENS_13SuspendReasonE"));
  }
  return resume(this, thread, suspendReason);
}

}
