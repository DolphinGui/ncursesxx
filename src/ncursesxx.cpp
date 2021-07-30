#include <curses.h>
#include <stdexcept>

#include "ncursesxx.hpp"

using namespace ncxx;

namespace {
WINDOW *initcurses(initOptions options, int delay) NCXX_EXCEPT {
  auto local = setlocale(LC_CTYPE, "");
  initscr(); /* Start curses mode 		*/
  if ((options & initOptions::cbreak) != initOptions::nothing)
    cbreak(); /* Line buffering disabled	*/
  else
   raw();
  if ((options & initOptions::keypad) != initOptions::nothing)
    keypad(stdscr, TRUE); /* We get F1, F2 etc..		*/
  if ((options & initOptions::echo) != initOptions::nothing)
    echo();
  else
   noecho();
  if (delay)
    halfdelay(delay);
  if((options & initOptions::nocolor) == initOptions::nothing)
    auto result = start_color();
#ifdef NCXX_NO_EXCEPTIONS
  if (result != OK)
    throw std::runtime_error("Color is not available from terminal");
#endif
  return stdscr;
}
} // namespace

terminal::terminal(initOptions options, int delay)
    : scr(initcurses(options, delay)) {}

terminal::~terminal() { endwin(); }