//
// Created by Administrator on 2024/4/6.
//
#pragma once
namespace art{
// The various reasons that we might be suspending a thread.
enum class SuspendReason : char {
  // Suspending for internal reasons (e.g. GC, stack trace, etc.).
  // TODO Split this into more descriptive sections.
  kInternal,
  // Suspending due to non-runtime, user controlled, code. (For example Thread#Suspend()).
  kForUserCode,
};

}