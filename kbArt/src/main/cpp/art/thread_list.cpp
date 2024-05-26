//
// Created by Administrator on 2024/4/6.
//
#include "thread_list.h"
#include "art_xdl.h"
#include "logger.h"
#include "string"
namespace art {

void *ThreadList::SuspendThreadByThreadId(uint32_t threadId,
                                          SuspendReason suspendReason,
                                          bool *timed_out) {


  static void *(*suspend_thread_by_thread_id)(void *,
                                              uint32_t,
                                              SuspendReason,
                                              bool *) = nullptr;
  if (suspend_thread_by_thread_id == nullptr) {
    std::string symbol;
    int32_t api_level = getAndroidApiLevel();
    if (api_level > 26 && api_level <= 34) {
      symbol = "_ZN3art10ThreadList23SuspendThreadByThreadIdEjNS_13SuspendReasonEPb";
    } else {
      symbol = "_ZN3art10ThreadList23SuspendThreadByThreadIdEjbPb";
    }
    suspend_thread_by_thread_id =
        reinterpret_cast<void *(*)(void *,
                                   uint32_t,
                                   SuspendReason,
                                   bool *)>(findArtSoSym(
            symbol.c_str()));
  }
  //
  return suspend_thread_by_thread_id(this, threadId, suspendReason, timed_out);
}
bool ThreadList::Resume(void *thread, SuspendReason suspendReason) {
  static bool (*resume)(void *, void *, SuspendReason) = nullptr;
  if (resume == nullptr) {
    std::string symbol;
    int32_t api_level = getAndroidApiLevel();
    if (api_level > 26 && api_level <= 34) {
      symbol = "_ZN3art10ThreadList6ResumeEPNS_6ThreadENS_13SuspendReasonE";
    } else {
      symbol = "_ZN3art10ThreadList6ResumeEPNS_6ThreadEb";
    }
    resume = reinterpret_cast<bool (*)(void *thread_list,
                                       void *thread,
                                       SuspendReason suspendReason)>(findArtSoSym(
        symbol.c_str()));
  }
  return resume(this, thread, suspendReason);
}

}
