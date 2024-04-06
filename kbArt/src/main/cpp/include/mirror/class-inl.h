//
// Created by Administrator on 2024/4/4.
//
#pragma once
#include "object.h"
#include "class.h"
#include "base/length_prefixed_array.h"
namespace art {
namespace mirror {

inline uint32_t Class::NumMethods() {
  return NumMethods(GetMethodsPtr());
}
inline uint32_t Class::NumMethods(LengthPrefixedArray<ArtMethod> *methods) {
  return (methods == nullptr) ? 0 : methods->size();
}

ALWAYS_INLINE LengthPrefixedArray<ArtMethod> *Class::GetMethodsPtr() {
  return reinterpret_cast<LengthPrefixedArray<ArtMethod> *>(
      static_cast<uintptr_t>(GetField64(OFFSET_OF_OBJECT_MEMBER(Class, methods_))));
}

ALWAYS_INLINE ArraySlice<ArtMethod> Class::GetMethods(PointerSize pointer_size) {
  LengthPrefixedArray<ArtMethod> *methods = GetMethodsPtr();
  return GetMethodsSliceRangeUnchecked(methods, pointer_size, 0u, NumMethods(methods));
}

inline ArraySlice<ArtMethod> Class::GetMethodsSliceRangeUnchecked(
    LengthPrefixedArray<ArtMethod> *methods,
    PointerSize pointer_size,
    uint32_t start_offset,
    uint32_t end_offset) {

  uint32_t size = end_offset - start_offset;
  if (size == 0u) {
    return ArraySlice<ArtMethod>();
  }
  size_t method_size = ArtMethod::Size(pointer_size);
  size_t method_alignment = ArtMethod::Alignment(pointer_size);
  ArraySlice<ArtMethod> slice(&methods->At(0u, method_size, method_alignment),
                              methods->size(),
                              method_size);
  return slice.SubArray(start_offset, size);
}


}
}