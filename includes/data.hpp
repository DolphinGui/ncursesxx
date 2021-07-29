#pragma once

#include <cstdint>
#include <ncurses.h>
#include <stdexcept>
#include <utility>

#include "macros.hpp"

namespace ncxx {

enum struct initOptions : uint32_t {
  NOTHING = 0,
  RAW = 1,
  NOECHO = 1 << 1,
  KEYPAD = 1 << 2
};
MTX_BITFLAG_OPS_MACRO(initOptions, uint32_t);
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
  BLACK = COLOR_BLACK,
  RED = COLOR_RED,
  GREEN = COLOR_GREEN,
  YELLOW = COLOR_YELLOW,
  BLUE = COLOR_BLUE,
  CYAN = COLOR_CYAN,
  MAGENTA = COLOR_MAGENTA,
  WHITE = COLOR_WHITE
};

using color = colors;
class color_pair {
  static int counter;
  int identity;
  using fore = color;
  using back = color;
  template<typename charType>
  friend class window;
  color_pair(int identity) : identity(identity){};

public:
  color_pair(color fore, color back) : identity(counter) {
    if (COLOR_PAIRS <= counter)
      throw std::logic_error("too many color pairs constructed.");
    auto result =
        init_pair(counter, static_cast<short>(fore), static_cast<short>(back));
    counter++;
  }
  color_pair(const color_pair &) = default;
  ~color_pair() { counter--; }
  std::pair<fore, back> content() {
    color fore, back;
    pair_content(identity, reinterpret_cast<short *>(&fore),
                 reinterpret_cast<short *>(&back));
    return {fore, back};
  }
  int _getIdentity() { return identity; }
  operator charAttributes() {
    return static_cast<charAttributes>(COLOR_PAIR(identity));
  }
};
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