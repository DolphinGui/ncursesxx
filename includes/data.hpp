#pragma once

#include <cstdint>
#include <ios>

namespace ncxx {

enum struct initOptions : uint32_t {
  NOTHING = 0,
  RAW = 1,
  NOECHO = 1 << 1,
  KEYPAD = 1 << 2
};

constexpr inline initOptions operator&(initOptions left,
                                       initOptions right) noexcept {
  return static_cast<initOptions>(static_cast<uint32_t>(left) &
                                  static_cast<uint32_t>(right));
}
constexpr inline initOptions operator|(initOptions left,
                                       initOptions right) noexcept {
  return static_cast<initOptions>(static_cast<uint32_t>(left) |
                                  static_cast<uint32_t>(right));
}

constexpr inline initOptions operator^(initOptions left,
                                       initOptions right) noexcept {
  return static_cast<initOptions>(static_cast<uint32_t>(left) ^
                                  static_cast<uint32_t>(right));
}

constexpr inline initOptions operator~(initOptions right) noexcept {
  return static_cast<initOptions>(~static_cast<uint32_t>(right));
}

inline initOptions &operator|=(initOptions &a, initOptions b) {
  return reinterpret_cast<initOptions &>(reinterpret_cast<uint32_t &>(a) |=
                                         static_cast<uint32_t>(b));
}

inline initOptions &operator^=(initOptions &a, initOptions b) {
  return reinterpret_cast<initOptions &>(reinterpret_cast<uint32_t &>(a) ^=
                                         static_cast<uint32_t>(b));
}

inline initOptions &operator&=(initOptions &a, initOptions b) {
  return reinterpret_cast<initOptions &>(reinterpret_cast<uint32_t &>(a) &=
                                         static_cast<uint32_t>(b));
}

struct coordinate {
  long x, y;
  std::streamoff a;
  constexpr inline bool operator==(coordinate other) const noexcept {
    return x == other.x && y == other.y;
  }
  constexpr inline bool operator!=(coordinate other) const noexcept {
    return !(*this == other);
  }
};

typedef coordinate offset;

constexpr inline offset operator+(coordinate left, coordinate right) noexcept {
  return {left.x + right.x, left.y + right.y};
}
constexpr inline offset operator-(coordinate left, coordinate right) noexcept {
  return {left.x - right.x, left.y - right.y};
}
constexpr inline offset operator+=(coordinate &left,
                                   coordinate right) noexcept {
  return {left.x += right.x, left.y += right.y};
}
constexpr inline offset operator-=(coordinate &left,
                                   coordinate right) noexcept {
  return {left.x -= right.x, left.y -= right.y};
}

} // namespace ncxx