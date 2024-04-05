#pragma once
//   (__builtin_constant_p(OFFSETOF_HELPER(t,f)) ? OFFSETOF_HELPER(t,f) : OFFSETOF_HELPER(t,f))
#define OFFSETOF_MEMBER(t, f) offsetof(t, f)
namespace art {

// Allow the meaning of offsets to be strongly typed.
class Offset {
 public:
  constexpr explicit Offset(size_t val) : val_(val) {}
  constexpr int32_t Int32Value() const {
    return static_cast<int32_t>(val_);
  }
  constexpr uint32_t Uint32Value() const {
    return static_cast<uint32_t>(val_);
  }
  constexpr size_t SizeValue() const {
    return val_;
  }
  Offset &operator+=(const size_t rhs) {
    val_ += rhs;
    return *this;
  }
  constexpr bool operator==(Offset o) const {
    return SizeValue() == o.SizeValue();
  }
  constexpr bool operator!=(Offset o) const {
    return !(*this == o);
  }
  constexpr bool operator<(Offset o) const {
    return SizeValue() < o.SizeValue();
  }
  constexpr bool operator<=(Offset o) const {
    return !(*this > o);
  }
  constexpr bool operator>(Offset o) const {
    return o < *this;
  }
  constexpr bool operator>=(Offset o) const {
    return !(*this < o);
  }

 protected:
  size_t val_;
};

// Offsets relative to an object.
class MemberOffset : public Offset {
 public:
  constexpr explicit MemberOffset(size_t val) : Offset(val) {}
};
}