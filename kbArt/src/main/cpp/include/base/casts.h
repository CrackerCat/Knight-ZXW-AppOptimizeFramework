//
// Created by Knight-ZXW on 2024/4/3.
//
#pragma once

#include <stdint.h>
#include <string.h>

#include <limits>
#include <type_traits>


#include "stl_util_identity.h"

namespace kbArt {

// Use implicit_cast as a safe version of static_cast or const_cast
// for upcasting in the type hierarchy (i.e. casting a pointer to Foo
// to a pointer to SuperclassOfFoo or casting a pointer to Foo to
// a const pointer to Foo).
// When you use implicit_cast, the compiler checks that the cast is safe.
// Such explicit implicit_casts are necessary in surprisingly many
// situations where C++ demands an exact type match instead of an
// argument type convertible to a target type.
//
// The From type can be inferred, so the preferred syntax for using
// implicit_cast is the same as for static_cast etc.:
//
//   implicit_cast<ToType>(expr)
//
// implicit_cast would have been part of the C++ standard library,
// but the proposal was submitted too late.  It will probably make
// its way into the language in the future.
template<typename To, typename From>
inline To implicit_cast(From const &f) {
  return f;
}

// When you upcast (that is, cast a pointer from type Foo to type
// SuperclassOfFoo), it's fine to use implicit_cast<>, since upcasts
// always succeed.  When you downcast (that is, cast a pointer from
// type Foo to type SubclassOfFoo), static_cast<> isn't safe, because
// how do you know the pointer is really of type SubclassOfFoo?  It
// could be a bare Foo, or of type DifferentSubclassOfFoo.  Thus,
// when you downcast, you should use this macro.



template <class Dest, class Source>
inline Dest bit_cast(const Source& source) {
  // Compile time assertion: sizeof(Dest) == sizeof(Source)
  // A compile error here means your Dest and Source have different sizes.
  static_assert(sizeof(Dest) == sizeof(Source), "sizes should be equal");
  Dest dest;
  memcpy(&dest, &source, sizeof(dest));
  return dest;
}

// A version of static_cast that DCHECKs that the value can be precisely represented
// when converting to Dest.
template <typename Dest, typename Source>
constexpr Dest dchecked_integral_cast(Source source) {
  return static_cast<Dest>(source);
}



}  // namespace art