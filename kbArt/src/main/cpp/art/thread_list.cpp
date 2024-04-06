//
// Created by Administrator on 2024/4/6.
//
#include "thread_list.h"
#include "art_xdl.h"

namespace art{
void ThreadList::SuspendThreadByThreadId(uint32_t threadId,
                                                        SuspendReason suspendReason,
                                                        bool *timed_out) {
  {

    void (*suspend_thread_by_thread_id)(void *,
                                        uint32_t,
                                        SuspendReason,
                                        bool *);
    if (suspend_thread_by_thread_id == nullptr) {
      suspend_thread_by_thread_id =
          reinterpret_cast<void (*)(void *,
                                    uint32_t,
                                    SuspendReason,
                                    bool *)>(dsym(
              "_ZN3art10ThreadList23SuspendThreadByThreadIdEjNS_13SuspendReasonEPb"));
    }
    //
    return suspend_thread_by_thread_id(this, threadId, suspendReason, timed_out);
  }
}
void ThreadList::Resume(void *thread, SuspendReason suspendReason){
  bool (*resume)(void *, void *, SuspendReason);
  if (resume == nullptr) {
    resume = reinterpret_cast<bool (*)(void *thread_list,
                                       void *thread,
                                       SuspendReason suspendReason)>(dsym(
        "_ZN3art10ThreadList6ResumeEPNS_6ThreadENS_13SuspendReasonE"));
  }
  resume(this, thread, suspendReason);
}
}
