//
// Created by Knight-ZXW on 2024/5/14.
//

#ifndef APPOPTIMIZEFRAMEWORK_KBART_SRC_MAIN_CPP_SUSPEND_TIME_OUT_FIX_H_
#define APPOPTIMIZEFRAMEWORK_KBART_SRC_MAIN_CPP_SUSPEND_TIME_OUT_FIX_H_
#include <jni.h>
enum LogSeverity {
  VERBOSE,
  DEBUG,
  INFO,
  WARNING,
  ERROR,
  FATAL_WITHOUT_ABORT,  // For loggability tests, this is considered identical to FATAL.
  FATAL,
};
#define SUSPEND_LOG_MSG "Thread suspension timed out"


namespace kbArt {

LogSeverity ToLogSeverity(int logLevel);

const char *getThreadSuspendByPeerWarningFunctionName();
}
#endif //APPOPTIMIZEFRAMEWORK_KBART_SRC_MAIN_CPP_SUSPEND_TIME_OUT_FIX_H_
