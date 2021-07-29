#pragma once
#include <iostream>

#include "data.hpp"

namespace ncxx {

struct _move {
  coordinate c;
};

inline constexpr _move setxy(coordinate coord) noexcept { return {coord}; }

struct _set {
  color_pair p;
};

inline _set setColor(color_pair p) noexcept { return {p}; }

struct _unset {
  color_pair p;
};

inline _unset unsetColor(color_pair p) noexcept { return {p}; }

} // namespace ncxx