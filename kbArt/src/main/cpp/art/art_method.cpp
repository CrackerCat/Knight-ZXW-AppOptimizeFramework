//
// Created by Administrator on 2024/4/24.
//
#include "art_method.h"
#include "cxx_helper.h"
uint32_t ArtMethod::GetAccessFlags() const  {
  return MemberOf<uint32_t>(art_method_ptr_, offset_->access_flags_offset_);
}
bool ArtMethod::IsMemorySharedMethod() {
  return IsMemorySharedMethod(GetAccessFlags());
}
bool ArtMethod::IsMemorySharedMethod(uint32_t access_flags) {
  return (access_flags & kAccMemorySharedMethod) != 0;
}
bool ArtMethod::IsDirect(uint32_t access_flags) {
  constexpr uint32_t direct = kAccStatic | kAccPrivate | kAccConstructor;
  return (access_flags & direct) != 0;
}
bool ArtMethod::IsDefaultConflicting(uint32_t access_flags) {
  // Default conflct methods are marked as copied, abstract and default.
  // We need to check the kAccIntrinsic too, see `IsCopied()`.
  static constexpr uint32_t kMask = kAccIntrinsic | kAccCopied | kAccAbstract | kAccDefault;
  static constexpr uint32_t kValue = kAccCopied | kAccAbstract | kAccDefault;
  return (access_flags & kMask) == kValue;
}
bool ArtMethod::IsAbstract(uint32_t access_flags) {
  // Default confliciting methods have `kAccAbstract` set but they are not actually abstract.
  return (access_flags & kAccAbstract) != 0 && !IsDefaultConflicting(access_flags);
}
bool ArtMethod::IsIntrinsic(uint32_t access_flags) {
  return (access_flags & kAccIntrinsic) != 0;
}
std::string ArtMethod::PrettyMethod(bool withSignature) {
  static std::string (*_PrettyMethod)(void *, bool) = nullptr;
  if (_PrettyMethod == nullptr) {
    _PrettyMethod = (std::string (*)(void *, bool)) dsym(
        "_ZN3art9ArtMethod12PrettyMethodEPS0_b");
  }
  return _PrettyMethod(this, withSignature);

}
//constexpr size_t ArtMethod::PtrSizedFieldsOffset(PointerSize pointer_size) {
//  // Round up to pointer size for padding field. Tested in art_method.cc.
//  return RoundUp(offsetof(ArtMethod, hotness_count_) + sizeof(hotness_count_),
//                 static_cast<size_t>(pointer_size));
//}

