#pragma once

#include <ncurses.h>
#include <ostream>

#include "data.hpp"
#include "iocurse.hpp"

namespace ncxx {


/*A singleton. Yes I know singletons are terrible,
 but there is literally no other way to encapsulate a
 global state library*/
class terminal {
  terminal(initOptions, int);
  ~terminal();

public:
  static terminal &get(initOptions options, int halfdelay) {
    static terminal itself(options, halfdelay);
    return itself;
  }
  static terminal &get() { return get(initOptions::NOTHING, 0); }
  window scr;
};
} // namespace ncxx