#pragma once


#include <ncurses.h>
#include <ostream>

#include "data.hpp"
#include "iocurse.hpp"
#include "manipcurse.hpp"
#include "macros.hpp"

namespace ncxx {

/*A singleton. Yes I know singletons are terrible,
 but I can't think of a better way to encapsulate a
 global state library*/

class terminal {
  terminal(initOptions, int);
  ~terminal();

public:
  static terminal &init(initOptions options, int halfdelay) {
    static terminal itself(options, halfdelay);
    return itself;
  }
  static terminal &get() { return init(initOptions::NOTHING, 0); }
  window<> scr;
};
} // namespace ncxx