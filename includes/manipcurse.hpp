#pragma once
#include <iostream>

#include "data.hpp"
#include "macros.hpp"

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

struct _setbackground {
  charAttributes p;
};

inline _setbackground setBackground(charAttributes p) noexcept { return {p}; }

struct _border {
  NCXX_CHARTYPE ls, rs, ts, bs, tl, tr, bl, br;
};
inline constexpr _border borderSet(NCXX_CHARTYPE tl, NCXX_CHARTYPE ts,
                                   NCXX_CHARTYPE tr, NCXX_CHARTYPE ls,
                                   NCXX_CHARTYPE rs, NCXX_CHARTYPE bl,
                                   NCXX_CHARTYPE bs,
                                   NCXX_CHARTYPE br) noexcept {
  return {ls, rs, ts, bs, tl, tr, bl, br};
}

} // namespace ncxx