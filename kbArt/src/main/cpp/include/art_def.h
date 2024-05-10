//
// Created by Knight-ZXW on 2023/6/8.
//

#pragma once
#include "stdlib.h"
#include "stdint.h"
#include "string"
namespace art {

#define ANDROID_ICE_CREAM_SANDWICH      14
#define ANDROID_ICE_CREAM_SANDWICH_MR1  15
#define ANDROID_JELLY_BEAN              16
#define ANDROID_JELLY_BEAN_MR1          17
#define ANDROID_JELLY_BEAN_MR2          18
#define ANDROID_KITKAT                  19
#define ANDROID_KITKAT_WATCH            20
#define ANDROID_LOLLIPOP                21
#define ANDROID_LOLLIPOP_MR1            22
#define ANDROID_M                       23
#define ANDROID_N                       24
#define ANDROID_N_MR1                   25
#define ANDROID_O                       26
#define ANDROID_O_MR1                   27
#define ANDROID_P                       28
#define ANDROID_Q                       29
#define ANDROID_R                       30
#define ANDROID_S                       31
#define ANDROID_S_V2                    32
#define ANDROID_TIRAMISU                33
#define ANDROID_UPSIDE_DOWN_CAKE        34

enum class ThreadState : uint8_t {
  // `kRunnable` was previously 67 but it is now set to 0 so that we do not need to extract
  // flags from the thread's `state_and_flags` to check for any flag being set while Runnable.
  // Note: All atomic accesses for a location should use the same data size,
  // so the incorrect old approach of reading just 16 bits has been rewritten.

  kTerminated =
  66,                 // TERMINATED     TS_ZOMBIE    Thread.run has returned, but Thread* still around
  kRunnable = 0,                    // RUNNABLE       TS_RUNNING   runnable
  kObsoleteRunnable = 67,           // ---            ---          obsolete value
  kTimedWaiting = 68,               // TIMED_WAITING  TS_WAIT      in Object.wait() with a timeout
  kSleeping,                        // TIMED_WAITING  TS_SLEEPING  in Thread.sleep()
  kBlocked,                         // BLOCKED        TS_MONITOR   blocked on a monitor
  kWaiting,                         // WAITING        TS_WAIT      in Object.wait()
  kWaitingForLockInflation,         // WAITING        TS_WAIT      blocked inflating a thin-lock
  kWaitingForTaskProcessor,         // WAITING        TS_WAIT      blocked waiting for taskProcessor
  kWaitingForGcToComplete,          // WAITING        TS_WAIT      blocked waiting for GC
  kWaitingForCheckPointsToRun,      // WAITING        TS_WAIT      GC waiting for checkpoints to run
  kWaitingPerformingGc,             // WAITING        TS_WAIT      performing GC
  kWaitingForDebuggerSend,          // WAITING        TS_WAIT      blocked waiting for events to be sent
  kWaitingForDebuggerToAttach,      // WAITING        TS_WAIT      blocked waiting for debugger to attach
  kWaitingInMainDebuggerLoop,       // WAITING        TS_WAIT      blocking/reading/processing debugger events
  kWaitingForDebuggerSuspension,    // WAITING        TS_WAIT      waiting for debugger suspend all
  kWaitingForJniOnLoad,             // WAITING        TS_WAIT      waiting for execution of dlopen and JNI on load code
  kWaitingForSignalCatcherOutput,   // WAITING        TS_WAIT      waiting for signal catcher IO to complete
  kWaitingInMainSignalCatcherLoop,  // WAITING        TS_WAIT      blocking/reading/processing signals
  kWaitingForDeoptimization,        // WAITING        TS_WAIT      waiting for deoptimization suspend all
  kWaitingForMethodTracingStart,    // WAITING        TS_WAIT      waiting for method tracing to start
  kWaitingForVisitObjects,          // WAITING        TS_WAIT      waiting for visiting objects
  kWaitingForGetObjectsAllocated,   // WAITING        TS_WAIT      waiting for getting the number of allocated objects
  kWaitingWeakGcRootRead,           // WAITING        TS_WAIT      waiting on the GC to read a weak root
  kWaitingForGcThreadFlip,          // WAITING        TS_WAIT      waiting on the GC thread flip (CC collector) to finish
  kNativeForAbort,                  // WAITING        TS_WAIT      checking other threads are not run on abort.
  kStarting,                        // NEW            TS_WAIT      native thread started, not yet ready to run managed code
  kNative,                          // RUNNABLE       TS_RUNNING   running in a JNI native method
  kSuspended,                       // RUNNABLE       TS_RUNNING   suspended by GC or debugger
};

//Android 9
struct PartialRuntimeP {
  void *thread_list_;

  void *intern_table_;

  void *class_linker_;

  void *signal_catcher_;
  // If true, the runtime will connect to tombstoned via a socket to
  // request an open file descriptor to write its traces to.
  bool use_tombstoned_traces_;

  // Location to which traces must be written on SIGQUIT. Only used if
  // tombstoned_traces_ == false.
  std::string stack_trace_file_;

  void *java_vm_;

};

//Android 10
struct PartialRuntimeQ {
  void *thread_list_;

  void *intern_table_;

  void *class_linker_;

  void *signal_catcher_;

  void *java_vm_;
};

//Android 11
//https://cs.android.com/android/platform/superproject/+/android-11.0.0_r40:art/runtime/runtime.h
struct PartialRuntimeR {
  void *thread_list_;

  void *intern_table_;

  void *class_linker_;

  void *signal_catcher_;

  void *jni_id_manager_;

  void *java_vm_;

};

//Android 13~Android14
struct PartialRuntimeTiramisu {
  void *thread_list_;

  void *intern_table_;

  void *class_linker_;

  void *signal_catcher_;

  void *small_lrt_allocator_;

  void *jni_id_manager_;

  void *java_vm_;
};

struct JavaVMExt {
  void *functions;
  void *runtime;
};



//classLinker



}