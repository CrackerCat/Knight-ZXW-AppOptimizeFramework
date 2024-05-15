//
// Created by Administrator on 2024/4/4.
//

#pragma once

#include <stddef.h>  // for size_t
#include <unistd.h>  // for TEMP_FAILURE_RETRY

#define EXPORT __attribute__((visibility("default")))

#define PACKED(x) __attribute__ ((__aligned__(x), __packed__))
#define ALWAYS_INLINE  __attribute__ ((always_inline))


// Changing this definition will cause you a lot of pain.  A majority of
// vendor code defines LIKELY and UNLIKELY this way, and includes
// this header through an indirect path.
#define LIKELY( exp )       (__builtin_expect( (exp) != 0, true  ))
#define UNLIKELY( exp )     (__builtin_expect( (exp) != 0, false ))

// Define that a position within code is unreachable, for example:
//   int foo () { LOG(FATAL) << "Don't call me"; UNREACHABLE(); }
// without the UNREACHABLE a return statement would be necessary.
#define UNREACHABLE  __builtin_unreachable


#define NO_RETURN [[ noreturn ]]

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);   \
  void operator=(const TypeName&)

#define OPTION __unused

#define OPEN_API __attribute__((visibility("default")))
#define C_API extern "C"

// A macro to disallow new and delete operators for a class. It goes in the private: declarations.
// NOTE: Providing placement new (and matching delete) for constructing container elements.
#define DISALLOW_ALLOCATION() \
  public: \
    NO_RETURN ALWAYS_INLINE void operator delete(void*, size_t) { UNREACHABLE(); } \
    ALWAYS_INLINE void* operator new(size_t, void* ptr) noexcept { return ptr; } \
    ALWAYS_INLINE void operator delete(void*, void*) noexcept { } \
  private: \
    void* operator new(size_t) = delete  // NOLINT

#define OFFSETOF_MEMBER(t, f) offsetof(t, f)

#define ALIGNED(x) __attribute__ ((__aligned__(x)))
#define PACKED(x) __attribute__ ((__aligned__(x), __packed__))