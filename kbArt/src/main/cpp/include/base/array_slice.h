//
// Created by Knight-ZXW on 2024/4/3.
//
// source:https://cs.android.com/android/platform/superproject/main/+/main:art/libartbase/base/array_slice.h;drc=b520a15dc110253cf27b170fa7ac103fafcebade;l=32
#pragma once
#include "base/array_slice.h"
#include "stride_iterator.h"
#include "casts.h"
#include <ostream>
namespace kbArt{
// An ArraySlice is an abstraction over an array or a part of an array of a particular type. It does
// bounds checking and can be made from several common array-like structures in Art.
template <typename T>

class ArraySlice {
 public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using pointer = T*;
  using const_pointer = const T*;
  using iterator = StrideIterator<T>;
  using const_iterator = StrideIterator<const T>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  using difference_type = ptrdiff_t;
  using size_type = size_t;


  // Create an empty array slice.
  ArraySlice() : array_(nullptr), size_(0), element_size_(0) {}

  // Create an array slice of the first 'length' elements of the array, with each element being
  // element_size bytes long.
  ArraySlice(T* array,
             size_t length,
             size_t element_size = sizeof(T))
      : array_(array),
        size_(dchecked_integral_cast<uint32_t>(length)),
        element_size_(element_size) {
    DCHECK(array_ != nullptr || length == 0);
  }
  ArraySlice(const ArraySlice<T>&) = default;
  ArraySlice(ArraySlice<T>&&) noexcept = default;
  ArraySlice<T>& operator=(const ArraySlice<T>&) = default;
  ArraySlice<T>& operator=(ArraySlice<T>&&) noexcept = default;

  // Iterators.
  iterator begin() { return iterator(&AtUnchecked(0), element_size_); }
  const_iterator begin() const { return const_iterator(&AtUnchecked(0), element_size_); }
  const_iterator cbegin() const { return const_iterator(&AtUnchecked(0), element_size_); }
  StrideIterator<T> end() { return StrideIterator<T>(&AtUnchecked(size_), element_size_); }
  const_iterator end() const { return const_iterator(&AtUnchecked(size_), element_size_); }
  const_iterator cend() const { return const_iterator(&AtUnchecked(size_), element_size_); }
  reverse_iterator rbegin() { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
  const_reverse_iterator crbegin() const { return const_reverse_iterator(cend()); }
  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
  const_reverse_iterator crend() const { return const_reverse_iterator(cbegin()); }

  // Size.
  size_type size() const { return size_; }
  bool empty() const { return size() == 0u; }

  // Element access. NOTE: Not providing at() and data().

  reference operator[](size_t index) {
    return AtUnchecked(index);
  }

  const_reference operator[](size_t index) const {
    return AtUnchecked(index);
  }

  reference front() {
    DCHECK(!empty());
    return (*this)[0];
  }

  const_reference front() const {
    DCHECK(!empty());
    return (*this)[0];
  }

  reference back() {
    DCHECK(!empty());
    return (*this)[size_ - 1u];
  }

  const_reference back() const {
    DCHECK(!empty());
    return (*this)[size_ - 1u];
  }

  ArraySlice<T> SubArray(size_type pos) {
    return SubArray(pos, size() - pos);
  }

  ArraySlice<const T> SubArray(size_type pos) const {
    return SubArray(pos, size() - pos);
  }

  ArraySlice<T> SubArray(size_type pos, size_type length) {
    DCHECK_LE(pos, size());
    DCHECK_LE(length, size() - pos);
    return ArraySlice<T>(&AtUnchecked(pos), length, element_size_);
  }

  ArraySlice<const T> SubArray(size_type pos, size_type length) const {
    DCHECK_LE(pos, size());
    DCHECK_LE(length, size() - pos);
    return ArraySlice<const T>(&AtUnchecked(pos), length, element_size_);
  }

  size_t ElementSize() const {
    return element_size_;
  }

  bool Contains(const T* element) const {
    return &AtUnchecked(0) <= element && element < &AtUnchecked(size_) &&
        ((reinterpret_cast<uintptr_t>(element) -
            reinterpret_cast<uintptr_t>(&AtUnchecked(0))) % element_size_) == 0;
  }

  size_t OffsetOf(const T* element) const {
    DCHECK(Contains(element));
    // Since it's possible element_size_ != sizeof(T) we cannot just use pointer arithmatic
    uintptr_t base_ptr = reinterpret_cast<uintptr_t>(&AtUnchecked(0));
    uintptr_t obj_ptr = reinterpret_cast<uintptr_t>(element);
    return (obj_ptr - base_ptr) / element_size_;
  }

 private:
  T& AtUnchecked(size_t index) {
    return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(array_) + index * element_size_);
  }

  const T& AtUnchecked(size_t index) const {
    return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(array_) + index * element_size_);
  }

  T* array_;
  size_t size_;
  size_t element_size_;
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const ArraySlice<T>& ts) {
  bool first = true;
  os << "[";
  for (const T& t : ts) {
    if (!first) { os << ", "; }
    first = false;
    os << t;
  }
  os << "]";
  return os;
}
};
