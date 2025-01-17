//
// Created by knight-zxw on 2023/1/3.
// Email: nimdanoob@163.com
//

#ifndef KB_ART_THREAD_H_
#define KB_ART_THREAD_H_

#include <atomic>
#include "macro.h"
#include "tls.h"
#include "common.h"
#include "base/atomic.h"
namespace art {

typedef uint64_t (*GetCpuMicroTime_t)(void *thread);

class Thread {
  // For the CC colector, normal weak reference access can be disabled on a per-thread basis, while
  // processing references.  After finishing, the reference processor asynchronously sets the
  // per-thread flags back to kEnabled with release memory ordering semantics. Each mutator thread
  // should check its flag with acquire semantics before assuming that it is enabled. However,
  // that is often too expensive, so the reading thread sets it to kVisiblyEnabled after seeing it
  // kEnabled.  The Reference.get() intrinsic can thus read it in relaxed mode, and reread (by
  // resorting to the slow path) with acquire semantics if it sees a value of kEnabled rather than
  // kVisiblyEnabled.
  enum class WeakRefAccessState : int32_t {
    kVisiblyEnabled = 0,  // Enabled, and previously read with acquire load by this thread.
    kEnabled,
    kDisabled
  };

  struct PACKED(4) tls_32bit_sized_values_12_14 {

    // We have no control over the size of 'bool', but want our boolean fields
    // to be 4-byte quantities.
    using bool32_t = uint32_t;

    // The state and flags field must be changed atomically so that flag values aren't lost.
    // See `StateAndFlags` for bit assignments of `ThreadFlag` and `ThreadState` values.
    // Keeping the state and flags together allows an atomic CAS to change from being
    // Suspended to Runnable without a suspend request occurring.
    std::atomic<uint32_t> state_and_flags;
    // A non-zero value is used to tell the current thread to enter a safe point

    // at the next poll.
    int suspend_count;

    // Thin lock thread id. This is a small integer used by the thin lock implementation.
    // This is not to be confused with the native thread's tid, nor is it the value returned
    // by java.lang.Thread.getId --- this is a distinct value, used only for locking. One
    // important difference between this id and the ids visible to managed code is that these
    // ones get reused (to ensure that they fit in the number of bits available).
    uint32_t thin_lock_thread_id;

    // System thread id.
    uint32_t tid;
    //其他字段不同版本也有变化

  } tls32_;
  struct PACKED(4) tls_32bit_sized_values_android_5_11 {
    // We have no control over the size of 'bool', but want our boolean fields
    // to be 4-byte quantities.
    using bool32_t = uint32_t;
    // The state and flags field must be changed atomically so that flag values aren't lost.
    // See `StateAndFlags` for bit assignments of `ThreadFlag` and `ThreadState` values.
    // Keeping the state and flags together allows an atomic CAS to change from being
    // Suspended to Runnable without a suspend request occurring.
    int32_t state_and_flags;
    // A non-zero value is used to tell the current thread to enter a safe point

    // at the next poll.
    int suspend_count;

      int debug_suspend_count;

    // Thin lock thread id. This is a small integer used by the thin lock implementation.
    // This is not to be confused with the native thread's tid, nor is it the value returned
    // by java.lang.Thread.getId --- this is a distinct value, used only for locking. One
    // important difference between this id and the ids visible to managed code is that these
    // ones get reused (to ensure that they fit in the number of bits available).
    //
    uint32_t thin_lock_thread_id;

    // System thread id.
    uint32_t tid;

  } tls32_android11;

 public:

// Guaranteed to be non-zero.
  /**
   * 获取 thin_lock_thread_id threadId
   * @return
   */
  uint32_t GetThreadId();

  /**
   * 获取系统 线程Id
   * @return
   */
  uint32_t GetTid();

  uint64_t GetCpuMicroTime();

  static void init();
  static Thread *Current();
};

}

#endif //KB_ART_THREAD_H_
