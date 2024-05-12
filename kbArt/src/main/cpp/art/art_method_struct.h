//
// Created by Administrator on 2024/5/11.
//

#pragma once
#include "base/primitive_types.h"
namespace art {
//23
class ArtMethod_6 final {
 public:
  uint32_t declaring_class_;
  uint32_t dex_cache_resolved_methods_;
  uint32_t dex_cache_resolved_types_;
  // Access flags; low 16 bits are defined by spec.
  uint32_t access_flags_;
  /* Dex file fields. The defining dex file is available via declaring_class_->dex_cache_ */
  // Offset to the CodeItem.
  uint32_t dex_code_item_offset_;
  // Index into method_ids of the dex file associated with this method.
  uint32_t dex_method_index_;
  /* End of dex file fields. */
  // Entry within a dispatch table for this method. For static/direct methods the index is into
  // the declaringClass.directMethods, for virtual methods the vtable and for interface methods the
  // ifTable.
  uint32_t method_index_;
  // Fake padding field gets inserted here.

  // Must be the last fields in the method.
  // PACKED(4) is necessary for the correctness of
  // RoundUp(OFFSETOF_MEMBER(ArtMethod, ptr_sized_fields_), pointer_size).
  struct PACKED(4) PtrSizedFields {
    // Method dispatch from the interpreter invokes this pointer which may cause a bridge into
    // compiled code.
    void *entry_point_from_interpreter_;

    // Pointer to JNI function registered to this method, or a function to resolve the JNI function.
    void *entry_point_from_jni_;

    // Method dispatch from quick compiled code invokes this pointer which may cause bridging into
    // the interpreter.
    void *entry_point_from_quick_compiled_code_;
  } ptr_sized_fields_;
};
//24
class ArtMethod_7 final {
 public:
  uint32_t declaring_class_;
  uint32_t access_flags_;
  uint32_t dex_code_item_offset_;
  uint32_t dex_method_index_;
  uint16_t method_index_;
  uint16_t hotness_count_;

  struct PACKED(4) PtrSizedFields {
    void *dex_cache_resolved_methods_;
    void *dex_cache_resolved_types_;
    void *entry_point_from_jni_;
    void *entry_point_from_quick_compiled_code_;
  } ptr_sized_fields_;
};
//25
class ArtMethod_7_1 final {
 public:
  uint32_t declaring_class_;
  uint32_t access_flags_;
  uint32_t dex_code_item_offset_;
  uint32_t dex_method_index_;
  uint16_t method_index_;
  uint16_t hotness_count_;

  struct PACKED(4) PtrSizedFields {
    void *dex_cache_resolved_methods_;
    void *dex_cache_resolved_types_;
    void *entry_point_from_jni_;
    void *entry_point_from_quick_compiled_code_;
  } ptr_sized_fields_;
};
//26~27 (8 and 8.1)
class ArtMethod_8 final {
 public:
  uint32_t declaring_class_;
  uint32_t access_flags_;
  uint32_t dex_code_item_offset_;
  uint32_t dex_method_index_;
  uint16_t method_index_;
  uint16_t hotness_count_;

  struct PtrSizedFields {
    void *dex_cache_resolved_methods_;
    void *data;
    void *entry_point_from_quick_compiled_code_;
  } ptr_sized_fields_;
};

//28
class ArtMethod_9 final {
 public:
  uint32_t declaring_class_;
  uint32_t access_flags_;
  uint32_t dex_code_item_offset_;
  uint32_t dex_method_index_;
  uint16_t method_index_;
  uint16_t hotness_count_;

  struct PtrSizedFields {
    void *data;
    void *entry_point_from_quick_compiled_code_;
  } ptr_sized_fields_;
};

// 29~30
class ArtMethod_10 final {
 public:
  uint32_t declaring_class_;
  uint32_t access_flags_;
  uint32_t dex_code_item_offset_;
  uint32_t dex_method_index_;
  uint16_t method_index_;
  union {
    uint16_t hotness_count_;
    uint16_t imt_index_;
  };

  struct PACKED(4) PtrSizedFields {
    void *data;
    void *entry_point_from_quick_compiled_code_;
  } ptr_sized_fields_;
};

// 31~34
class ArtMethod_12 final {
 public:
  uint32_t declaring_class_;
  uint32_t access_flags_;
  uint32_t dex_method_index_;
  uint16_t method_index_;
  union {
    uint16_t hotness_count_;
    uint16_t imt_index_;
  };

  struct PACKED(4) PtrSizedFields {
    void *data;
    void *entry_point_from_quick_compiled_code_;
  } ptr_sized_fields_;
};
}
