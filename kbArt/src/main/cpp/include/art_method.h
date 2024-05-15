//
// Created by knight-zxw on 2022/12/31.
//

#ifndef KB_art_method_H_
#define KB_art_method_H_
#include <jni.h>
#include "string"
#include "modifiers.h"
#include "common.h"
#include "base/macros.h"
#include "base/pointer_size.h"
#include "base/bit_utils.h"
#include "art_xdl.h"
#include "art_runtime.h"
#include "cxx_helper.h"
#include "art_method_struct.h"
namespace art {
struct ArtMethodOffsets;


class ArtMethod final {
 public:
  explicit ArtMethod(void *method) : art_method_ptr_(method) {
    ArtRuntime *runtime = ArtRuntime::Get();
    offset_ = runtime->MethodOffsets();
  }
  static constexpr uint32_t kRuntimeMethodDexMethodIndex = 0xFFFFFFFF;

  // Fake padding field gets inserted here.

  // Must be the last fields in the method.
  struct PtrSizedFields {
    // Depending on the method type, the data is
    //   - native method: pointer to the JNI function registered to this method
    //                    or a function to resolve the JNI function,
    //   - resolution method: pointer to a function to resolve the method and
    //                        the JNI function for @CriticalNative.
    //   - conflict method: ImtConflictTable,
    //   - abstract/interface method: the single-implementation if any,
    //   - proxy method: the original interface method or constructor,
    //   - default conflict method: null
    //   - other methods: during AOT the code item offset, at runtime a pointer
    //                    to the code item.
    void *data_;

    // Method dispatch from quick compiled code invokes this pointer which may cause bridging into
    // the interpreter.
    void *entry_point_from_quick_compiled_code_;
  } ptr_sized_fields_;

  uint32_t GetAccessFlags() const;

//  inline bool IsRuntimeMethod() const {
//    return dex_method_index_ == kRuntimeMethodDexMethodIndex;
//  }

  inline bool IsRuntimeMethod() {
    // dex_method_index_ == kRuntimeMethodDexMethodIndex;
    return MemberOf<uint32_t>(art_method_ptr_,
                              ArtRuntime::Get()->MethodOffsets()->access_flags_offset_)
        == kRuntimeMethodDexMethodIndex;
  }

  // Returns true if the method resides in shared memory.
  bool IsMemorySharedMethod();
  static bool IsMemorySharedMethod(uint32_t access_flags);

  void ClearMemorySharedMethod() {
    uint32_t access_flags = GetAccessFlags();
    if (IsIntrinsic(access_flags) || IsAbstract(access_flags)) {
      return;
    }
    if (IsMemorySharedMethod(access_flags)) {
      ClearAccessFlags(kAccMemorySharedMethod);
    }
  }

  // This setter guarantees atomicity.
  void ClearAccessFlags(uint32_t flag) {
    ((std::atomic<uint32_t>) GetAccessFlags()).fetch_add(~flag, std::memory_order_relaxed);
  }

  // Returns true if the method is static, private, or a constructor.
  bool IsDirect() const {
    return IsDirect(GetAccessFlags());
  }

  static bool IsDirect(uint32_t access_flags);

  static bool IsDefaultConflicting(uint32_t access_flags);

  static bool IsAbstract(uint32_t access_flags);

  static bool IsIntrinsic(uint32_t access_flags);

  // Size of an instance of this native class.
  static constexpr size_t Size(PointerSize pointer_size) {
    return PtrSizedFieldsOffset(pointer_size) +
        (sizeof(PtrSizedFields) / sizeof(void *)) * static_cast<size_t>(pointer_size);
  }

  // Alignment of an instance of this native class.
  static constexpr size_t Alignment(PointerSize pointer_size) {
    // The ArtMethod alignment is the same as image pointer size. This differs from
    // alignof(ArtMethod) if cross-compiling with pointer_size != sizeof(void*).
    return static_cast<size_t>(pointer_size);
  }

  std::string PrettyMethod(bool withSignature);

 private:
  void *art_method_ptr_;
  ArtMethodOffsets *offset_;
  static constexpr size_t PtrSizedFieldsOffset(PointerSize pointer_size){
    return RoundUp(offsetof(ArtMethod12_14, hotness_count_) + sizeof(uint16_t),
                   static_cast<size_t>(pointer_size));
  };

};

}

#endif //KB_art_method_H_
