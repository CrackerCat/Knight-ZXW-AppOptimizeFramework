//
// Created by Administrator on 2024/4/4.
//
#pragma once
#include "offsets.h"
#include "macro.h"
#include "atomic.h"
#include "verify_object.h"
#include "object_reference.h"
namespace art{
namespace mirror{
// Fields within mirror objects aren't accessed directly so that the appropriate amount of
// handshaking is done with GC (for example, read and write barriers). This macro is used to
// compute an offset for the Set/Get methods defined in Object that can safely access fields.
#define OFFSET_OF_OBJECT_MEMBER(type, field) \
    art::MemberOffset(OFFSETOF_MEMBER(type, field))

class MANAGED Object {
 public:
  // The number of vtable entries in java.lang.Object.
  static constexpr size_t kVTableLength = 11;
  static uint32_t hash_code_seed;
  // The Class representing the type of the object.
  uint32_t klass_;
  // Monitor and hash code information.
  uint32_t monitor_;


  template<typename kType, bool kIsVolatile>
  ALWAYS_INLINE kType GetFieldPrimitive(MemberOffset field_offset){
      const uint8_t* raw_addr = reinterpret_cast<const uint8_t*>(this) + field_offset.Int32Value();
      const kType* addr = reinterpret_cast<const kType*>(raw_addr);
      if (kIsVolatile) {
        return reinterpret_cast<const Atomic<kType>*>(addr)->load(std::memory_order_seq_cst);
      } else {
        return reinterpret_cast<const Atomic<kType>*>(addr)->LoadJavaData();
      }
  };

  template<VerifyObjectFlags kVerifyFlags = kDefaultVerifyFlags, bool kIsVolatile = false>
  ALWAYS_INLINE int64_t GetField64(MemberOffset field_offset)
   {
      return GetFieldPrimitive<int64_t, kIsVolatile>(field_offset);
  }
};

}

}
