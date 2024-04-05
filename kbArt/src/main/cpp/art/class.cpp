//
// Created by Knight-ZXW on 2024/4/3.
//
#include "class.h"
#include "art_xdl.h"
#include "string"
std::string art::mirror::PrettyClass(void *clazz) {
  auto Pretty = (std::string (*)(void *)) dsym("_ZN3art6mirror5Class11PrettyClassEv");
  return Pretty(clazz);
}
