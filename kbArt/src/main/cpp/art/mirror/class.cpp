//
// Created by Administrator on 2024/4/6.
//
#include "class.h"
namespace art {
namespace mirror {
std::string Class::PrettyClass(void *clazz) {
  static std::string (*_PrettyClass)(void *) = nullptr;
  if (_PrettyClass == nullptr) {
    _PrettyClass = (std::string (*)(void *)) dsym(
        "_ZN3art6mirror5Class11PrettyClassEv");
  }
  return _PrettyClass(clazz);
};
}
}