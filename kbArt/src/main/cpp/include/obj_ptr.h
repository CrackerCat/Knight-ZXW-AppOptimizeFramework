//
// Created by Knight-ZXW on 2024/4/3.
//
#pragma once
#include "stddef.h"
#include "stdint.h"
#include "type_traits"
// Always inline ObjPtr methods even in debug builds.
#define OBJPTR_INLINE __attribute__ ((always_inline))
namespace kbArt{
// Value type representing a pointer to a mirror::Object of type MirrorType
// Since the cookie is thread based, it is not safe to share an ObjPtr between threads.
template<class MirrorType>
class ObjPtr {
 public:
  OBJPTR_INLINE ObjPtr() : ObjPtr(nullptr) {}

  OBJPTR_INLINE ObjPtr(std::nullptr_t)
      : reference_(0u) {
    DCHECK(IsNull());
  }

  // Note: The following constructors allow implicit conversion. This simplifies code that uses
  //       them, e.g., for parameter passing. However, in general, implicit-conversion constructors
  //       are discouraged and detected by clang-tidy.


  OBJPTR_INLINE MirrorType* operator->() const;

  // Ptr makes sure that the object pointer is valid.
  OBJPTR_INLINE MirrorType* Ptr() const;

  // Note: The following constructors allow implicit conversion. This simplifies code that uses
  //       them, e.g., for parameter passing. However, in general, implicit-conversion constructors
  //       are discouraged and detected by clang-tidy.


  // The encoded reference and cookie.
  uintptr_t reference_;

  OBJPTR_INLINE bool IsNull() const {
    return reference_ == 0;
  }

  // Ptr unchecked does not check that object pointer is valid. Do not use if you can avoid it.
  OBJPTR_INLINE MirrorType* PtrUnchecked() const {
    return reinterpret_cast<MirrorType*>(reference_);
  }
};

}
