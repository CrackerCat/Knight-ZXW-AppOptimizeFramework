//
// Created by Administrator on 2024/4/6.
//
#include "debugger.h"
void art::Dbg::Dbg::SetJdwpAllowed(bool allowed) {
  static void (*setJdwpAllowed)(bool) = nullptr;
  if (setJdwpAllowed == nullptr) {
    setJdwpAllowed = reinterpret_cast<void (*)(bool)>(dsym("_ZN3art3Dbg14SetJdwpAllowedEb"));
  }
  if (setJdwpAllowed != nullptr) {
    setJdwpAllowed(allowed);
  } else {
  }
}
bool art::Dbg::Dbg::IsJdwpAllowed() {
  static bool (*isJdwpAllow)() = nullptr;
  if (isJdwpAllow == nullptr) {
    isJdwpAllow =
        reinterpret_cast<bool (*)()>(dsym( "_ZN3art3Dbg13IsJdwpAllowedEv"));
  }
  return isJdwpAllow();
}
