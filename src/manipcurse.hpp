/** @file manipcurse.hpp
 *  @brief the manipulators header
 *
 *  It defines the various manipulators for \ref window.
 *
 *  @author Shin.
 */

#pragma once
#include <iostream>

#include "data.hpp"
#include "macros.hpp"

namespace ncxx {
struct _move {
  coordinate c;
};
/** \fn
 * @brief moves the cursor to the coordinates in the window.
 */
inline constexpr _move setxy(coordinate coord) noexcept { return {coord}; }
struct _set {
  charAttributes p;
};
/**
 * @brief enables the attributes
 */
inline _set setAttributes(charAttributes p) noexcept { return {p}; }
struct _unset {
  charAttributes p;
};
/**
 * @brief disables the attributes
 */
inline _unset unsetAttributes(charAttributes p) noexcept { return {p}; }
struct _setbackground {
  charAttributes p;
};
/**
 * @brief sets the attributes onto the background,
 * and all preexisting characters on the screen.
 */
inline _setbackground setBackground(charAttributes p) noexcept { return {p}; }
struct _border {
  NCXX_CHARTYPE ls, rs, ts, bs, tl, tr, bl, br;
};
/**
 * @brief prints characters onto the inner border of window.
 * @param tl topleft
 * @param tr topright
 * @param ts topside
 * @param rs rightside
 * @param ls leftside
 * @param bl bottomside
 * @param br bottomright
 * @param bs bottomside
 */
inline constexpr _border setBorder(NCXX_CHARTYPE tl, NCXX_CHARTYPE ts,
                                   NCXX_CHARTYPE tr, NCXX_CHARTYPE ls,
                                   NCXX_CHARTYPE rs, NCXX_CHARTYPE bl,
                                   NCXX_CHARTYPE bs,
                                   NCXX_CHARTYPE br) noexcept {
  return {ls, rs, ts, bs, tl, tr, bl, br};
}
/**
 * @brief calls box(). the other characters are default.
 */
inline constexpr _border box(NCXX_CHARTYPE vertical,
                             NCXX_CHARTYPE horizontal) noexcept {
  return {vertical, vertical, horizontal, horizontal};
}

} // namespace ncxx