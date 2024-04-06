//
// Created by Administrator on 2024/4/6.
//
#pragma once
#include "art_xdl.h"
namespace art {
namespace Dbg {
class Dbg {
 public:
  static void SetJdwpAllowed(bool allowed);
  static bool IsJdwpAllowed();
};

}
}