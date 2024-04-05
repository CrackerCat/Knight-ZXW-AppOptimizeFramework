//
// Created by Administrator on 2024/4/4.
//

#pragma once
#include "base/macros.h"
namespace art{
namespace mirror{

// How we want to check the heap's correctness.
enum VerifyObjectMode {
  kVerifyObjectModeDisabled,  // Heap verification is disabled.
  kVerifyObjectModeFast,  // Check heap accesses quickly by using VerifyClassClass.
  kVerifyObjectModeAll  // Check heap accesses thoroughly.
};


enum EXPORT VerifyObjectFlags {
    kVerifyNone = 0x0,
    // Verify self when we are doing an operation.
    kVerifyThis = 0x1,
    // Verify reads from objects.
    kVerifyReads = 0x2,
    // Verify writes to objects.
    kVerifyWrites = 0x4,
    // Verify all things.
    kVerifyAll = kVerifyThis | kVerifyReads | kVerifyWrites,
};

static constexpr VerifyObjectFlags kDefaultVerifyFlags = kVerifyNone;
static constexpr VerifyObjectMode kVerifyObjectSupport =
    kDefaultVerifyFlags != 0 ? kVerifyObjectModeFast : kVerifyObjectModeDisabled;

}
}