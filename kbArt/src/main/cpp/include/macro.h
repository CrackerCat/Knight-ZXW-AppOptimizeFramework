//
// Created by knight-zxw on 2023/1/2.
// Email: nimdanoob@163.com
//
#pragma once

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