/** @file data.hpp
 *  @brief the data header
 *
 *  It defines enums and structs.
 *
 *  @author Shin.
 */

#pragma once

#include <cstdint>
#include <ncurses.h>
#include <stdexcept>

#include "macros.hpp"

namespace ncxx {
/**
 *  @brief the initoptions bitflag enum
 *  enabling a flag enables the feature. otherwise
 *  its counterpart is called.
 *
 *  cbreak/raw:
 *  cbreak means that the input is buffered until a \\n is input.
 *  by default all input is not buffered.
 *  echo/noecho:
 *  echo means that input is also output onto the terminal.
 *  by default this is off
 *  keypad:
 *  allows for keypad, arrow, and other input to be read.
 *  by default this is off
 */
enum struct initOptions : uint32_t {
  nothing = 0,
  cbreak = 1,
  echo = 1 << 1,
  keypad = 1 << 2,
  nocolor = 1 << 3
};
MTX_BITFLAG_OPS_MACRO(initOptions, uint32_t);
/**
 *  @brief a bitflag enum for attributes
 *  I'm not sure if this is how it's supposed to work,
 *  although by all indications it is. In any case, it
 *  can also hold @ref ncxx::color_pair
 */
enum charAttributes : chtype {
  normal = A_NORMAL,
  standout = A_STANDOUT,
  underline = A_UNDERLINE,
  reverse = A_REVERSE,
  blink = A_BLINK,
  dim = A_DIM,
  bold = A_BOLD,
  protect = A_PROTECT,
  invis = A_INVIS,
  alt = A_ALTCHARSET,
  extract = A_CHARTEXT
};
MTX_BITFLAG_OPS_MACRO(charAttributes, chtype)
enum struct colors {
  black = COLOR_BLACK,
  red = COLOR_RED,
  green = COLOR_GREEN,
  yellow = COLOR_YELLOW,
  blue = COLOR_BLUE,
  cyan = COLOR_CYAN,
  magenta = COLOR_MAGENTA,
  white = COLOR_WHITE
};

using color = colors;
struct attributeOut;
class window;
/**
 *  @brief a wrapper for color pairs.
 *  this also casts to @ref ncxx::charAttributes
 */
class color_pair {
  static int counter;
  int identity;
  using fore = color;
  using back = color;
  friend class window;
  color_pair(int identity) : identity(identity){};

public:
  /**
   *  @brief failbit for this class
   *  if true, then the constructor has failed.
   *  only appears if NCXX_NO_EXCEPTION is defined
   */
#ifdef NCXX_NO_EXCEPTIONS
  static bool failed;
#endif
  /**
   *  @brief constructor for color_pair
   *  this really just wraps init_pair.
   */
  color_pair(color fore, color back) : identity(counter) NCXX_EXCEPT {

    if (COLOR_PAIRS <= counter)
#ifndef NCXX_NO_EXCEPTIONS
      throw std::logic_error("too many color pairs constructed.");
#else
      failed = true;
#endif
    auto result =
        init_pair(counter, static_cast<short>(fore), static_cast<short>(back));
    if (result != OK)
#ifndef NCXX_NO_EXCEPTIONS
      throw std::runtime_error("init_pair() failed");
#else
      failed = true;
#endif
    else
      counter++;
  }
  color_pair(const color_pair &) = default;
  ~color_pair() { counter--; }
  /**
   *  @brief retrieves the colors of the foreground and background.
   *  @return a pair with foreground as first, and background as second.
   */
  std::pair<fore, back> content() {
    color fore, back;
    pair_content(identity, reinterpret_cast<short *>(&fore),
                 reinterpret_cast<short *>(&back));
    return {fore, back};
  }
  /**
   *  @brief allows implicit casting to charAttributes.
   */
  operator charAttributes() {
    return static_cast<charAttributes>(COLOR_PAIR(identity));
  }
  inline int &_identityHandle() noexcept { return identity; }
};
#ifdef NCXX_NO_EXCEPTIONS
inline bool color_pair::failed{};
#endif
inline int color_pair::counter = 1;
struct attributeOut {
  color_pair p;
  charAttributes c;
};

/*
enum struct mouseInfo : mmask_t {
  RELEASE1 = BUTTON1_RELEASED,
  PRESS1 = BUTTON1_PRESSED,
  CLICK1 = BUTTON1_CLICKED,
  DOUBLE_CLICK1 = BUTTON1_DOUBLE_CLICKED,
  TRIPLE_CLICK1 = BUTTON1_TRIPLE_CLICKED,
  RELEASE2 = BUTTON2_RELEASED,
  PRESS2 = BUTTON2_PRESSED,
  CLICK2 = BUTTON2_CLICKED,
  DOUBLE_CLICK2 = BUTTON2_DOUBLE_CLICKED,
  TRIPLE_CLICK2 = BUTTON2_TRIPLE_CLICKED,
  RELEASE3 = BUTTON3_RELEASED,
  PRESS3 = BUTTON3_PRESSED,
  CLICK3 = BUTTON3_CLICKED,
  DOUBLE_CLICK3 = BUTTON3_DOUBLE_CLICKED,
  TRIPLE_CLICK3 = BUTTON3_TRIPLE_CLICKED,
  RELEASE4 = BUTTON4_RELEASED,
  PRESS4 = BUTTON4_PRESSED,
  CLICK4 = BUTTON4_CLICKED,
  DOUBLE_CLICK4 = BUTTON4_DOUBLE_CLICKED,
  TRIPLE_CLICK4 = BUTTON4_TRIPLE_CLICKED,
  SHIFT = BUTTON_SHIFT,
  CTRL = BUTTON_CTRL,
  ALT = BUTTON_ALT,
  ALL = ALL_MOUSE_EVENTS,
  POSITION = REPORT_MOUSE_POSITION
};*/

struct coordinate {
  long x, y;
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