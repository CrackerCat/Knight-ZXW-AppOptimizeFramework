//
// Created by Knight-ZXW on 2024/4/3.
//
#pragma once
#include "stddef.h"
#include "string"
#include "object.h"
#include "base/atomic.h"
#include "object_reference.h"
#include "pointer_size.h"
#include "art_method.h"
#include "array_slice.h"
#include "length_prefixed_array.h"
namespace art {
namespace mirror {


//TODO https://github.com/xiaobaiyey/jnihook/blob/21e2ed982d96ae17718bef48ac6de7e61639371b/app/src/main/cpp/jnihook/DexDefine.H#L2599



//https://cs.android.com/android/platform/superproject/main/+/main:art/runtime/mirror/class.h;drc=c20a5932acf6189c8ba42f2745ebc4e155540e27;l=1447
class  MANAGED Class final :public Object{
 public:
  // Defining class loader, or null for the "bootstrap" system loader.
  uint32_t class_loader_;
  // For array classes, the component class object for instanceof/checkcast
  // (for String[][][], this will be String[][]). null for non-array classes.
  uint32_t component_type_;
  // DexCache of resolved constant pool entries (will be null for classes generated by the
  // runtime such as arrays and primitive classes).
  uint32_t dex_cache;
  // Extraneous class data that is not always needed. This field is allocated lazily and may
  // only be set with 'this' locked. This is synchronized on 'this'.
  // TODO(allight) We should probably synchronize it on something external or handle allocation in
  // some other (safe) way to prevent possible deadlocks.
  uint32_t ext_data_;

  // The interface table (iftable_) contains pairs of a interface class and an array of the
  // interface methods. There is one pair per interface supported by this class.  That means one
  // pair for each interface we support directly, indirectly via superclass, or indirectly via a
  // superinterface.  This will be null if neither we nor our superclass implement any interfaces.
  //
  // Why we need this: given "class Foo implements Face", declare "Face faceObj = new Foo()".
  // Invoke faceObj.blah(), where "blah" is part of the Face interface.  We can't easily use a
  // single vtable.
  //
  // For every interface a concrete class implements, we create an array of the concrete vtable_
  // methods for the methods in the interface.
  uint32_t iftable_;
  // Descriptor for the class such as "java.lang.Class" or "[C". Lazily initialized by ComputeName
  // Descriptor for the class such as "java.lang.Class" or "[C". Lazily initialized by ComputeName
  uint32_t name_;
  // The superclass, or null if this is java.lang.Object or a primitive type.
  //
  // Note that interfaces have java.lang.Object as their
  // superclass. This doesn't match the expectations in JNI
  // GetSuperClass or java.lang.Class.getSuperClass() which need to
  // check for interfaces and return null.
  uint32_t super_class_;
  // Virtual method table (vtable), for use by "invoke-virtual".  The vtable from the superclass is
  // copied in, and virtual methods from our class either replace those from the super or are
  // appended. For abstract classes, methods may be created in the vtable that aren't in
  // virtual_ methods_ for miranda methods.
  uint32_t vtable_;
  // instance fields
  //
  // These describe the layout of the contents of an Object.
  // Note that only the fields directly declared by this class are
  // listed in ifields; fields declared by a superclass are listed in
  // the superclass's Class.ifields.
  //
  // ArtFields are allocated as a length prefixed ArtField array, and not an array of pointers to
  // ArtFields.
  uint64_t ifields_;
  // Pointer to an ArtMethod length-prefixed array. All the methods where this class is the place
  // where they are logically defined. This includes all private, static, final and virtual methods
  // as well as inherited default methods and miranda methods.
  //
  // The slice methods_ [0, virtual_methods_offset_) are the direct (static, private, init) methods
  // declared by this class.
  //
  // The slice methods_ [virtual_methods_offset_, copied_methods_offset_) are the virtual methods
  // declared by this class.
  //
  // The slice methods_ [copied_methods_offset_, |methods_|) are the methods that are copied from
  // interfaces such as miranda or default methods. These are copied for resolution purposes as this
  // class is where they are (logically) declared as far as the virtual dispatch is concerned.
  //
  // Note that this field is used by the native debugger as the unique identifier for the type.
  uint64_t methods_;
  //...
  ALWAYS_INLINE LengthPrefixedArray<ArtMethod>* GetMethodsPtr();
  ALWAYS_INLINE ArraySlice<ArtMethod> GetMethods(PointerSize pointer_size);
  ALWAYS_INLINE static ArraySlice<ArtMethod> GetMethodsSliceRangeUnchecked(
      LengthPrefixedArray<ArtMethod>* methods,
      PointerSize pointer_size,
      uint32_t start_offset,
      uint32_t end_offset);

  ALWAYS_INLINE uint32_t NumMethods();
  static ALWAYS_INLINE uint32_t NumMethods(LengthPrefixedArray<ArtMethod>* methods);

  static std::string PrettyClass(void* clazz){
    auto Pretty = (std::string (*)(void *)) dsym("_ZN3art6mirror5Class11PrettyClassEv");
    return Pretty(clazz);
  };
};
}

}