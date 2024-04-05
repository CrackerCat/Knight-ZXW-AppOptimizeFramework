//
// Created by Knight-ZXW on 2024/4/3.
//
#pragma once
#include "class.h"
#include "obj_ptr.h"
#include "base/array_slice.h"
namespace art {
void VisitClasses(void *class_linker, void *visitor);

class ClassVisitor {
 public:
  virtual ~ClassVisitor() {}
  // Return true to continue visiting.
  virtual bool operator()(ObjPtr<mirror::Class> klass) = 0;
};


class ClassLinker{


};
}

