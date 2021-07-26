#pragma once

#include <ncurses.h>

#include "enums.hpp"

namespace ncxx {


/*A singleton. Yes I know singletons are terrible,
 but there is literally no other way to encapsulate a
 global state library*/
class terminal {
  terminal(initOptions, int);

public:
  static terminal &get(initOptions options, int halfdelay) {
    static terminal itself(options, halfdelay);
    return itself;
  }
  static terminal &get() { return get(initOptions::NOTHING, 0); }
};
} // namespace ncxx