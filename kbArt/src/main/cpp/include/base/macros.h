//
// Created by Administrator on 2024/4/4.
//

#pragma once
#define EXPORT __attribute__((visibility("default")))

// offsetof is not defined by the spec on types with non-standard layout,
// however it is implemented by compilers in practice.
// (note that reinterpret_cast is not valid constexpr)
//
// Alternative approach would be something like:
// #define OFFSETOF_HELPER(t, f) \
//   (reinterpret_cast<uintptr_t>(&reinterpret_cast<t*>(16)->f) - static_cast<uintptr_t>(16u))
// #define OFFSETOF_MEMBER(t, f) \
//   (__builtin_constant_p(OFFSETOF_HELPER(t,f)) ? OFFSETOF_HELPER(t,f) : OFFSETOF_HELPER(t,f))
#define OFFSETOF_MEMBER(t, f) offsetof(t, f)

#define ALIGNED(x) __attribute__ ((__aligned__(x)))
#define PACKED(x) __attribute__ ((__aligned__(x), __packed__))