/** @file ncursesxx.hpp
 *  @brief the main header
 *
 *  It defines the ncurses initializer,
 *  and includes other headers.
 *
 *  @author Shin.
 */

/** @mainpage ncurses++ main man
 *  This is a c++ iostream adapter for
 *  ncurses. It was made in 2 days as
 *  a side project. I tested most stuff,
 *  and it should work properly. Get started
 *  by refering to the @ref ncxx::terminal class.
 */

#pragma once

#include <ncurses.h>
#include <ostream>

#include "data.hpp"
#include "iocurse.hpp"
#include "macros.hpp"
#include "manipcurse.hpp"

namespace ncxx {

/**
 * @brief the ncurses state machine
 * Its constructor and destructor deals with ncurses.
 * Internally it holds a static variable, so it's lazy
 * initialized and destructed at program end.
 */
class terminal {
  terminal(initOptions, int);
  ~terminal();

public:
  /**
   * @brief the initializer
   * refer to @ref ncxx::initOptions to see all of the options.
   * By defualt, none are chosen.
   * @param timeout the timeout for getch()
   */
  static terminal &init(initOptions options, int timeout) {
    static terminal itself(options, timeout);
    return itself;
  }
  /**
   * @brief gets terminal
   * calls @ref ncxx::terminal::init() with default options. 
   */
  static terminal &get() { return init(initOptions::nothing, 0); }
  /**
   * @brief the default screen
   * this is a standin for stdscr. see @ref ncxx::window for more.
   */
  window scr;
};
} // namespace ncxx