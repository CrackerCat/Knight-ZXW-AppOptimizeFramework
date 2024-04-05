//
// Created by Knight-ZXW on 2024/4/3.
//
#pragma once
#include <cstddef>
#include <cstdint>
#include <iosfwd>
namespace art{

enum class PointerSize : uint32_t {
  k32 = 4,
  k64 = 8
};

std::ostream& operator<<(std::ostream& os, const PointerSize& rhs);

static constexpr PointerSize kRuntimePointerSize = sizeof(void*) == 8U
                                                   ? PointerSize::k64
                                                   : PointerSize::k32;
}
