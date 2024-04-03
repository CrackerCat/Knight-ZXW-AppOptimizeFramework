//
// Created by Knight-ZXW on 2024/4/3.
//

#pragma once
#include "obj_ptr.h"
#include "base/bit_utils.h"
namespace kbArt{
template<class MirrorType>
inline MirrorType* ObjPtr<MirrorType>::operator->() const {
  return Ptr();
}
template<class MirrorType>
inline MirrorType* ObjPtr<MirrorType>::Ptr() const {
  return PtrUnchecked();
}

}