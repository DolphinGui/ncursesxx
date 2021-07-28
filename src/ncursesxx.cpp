#include "ncursesxx.hpp"

using namespace ncxx;

namespace {
WINDOW *initcurses(initOptions options, int delay) {
  initscr(); /* Start curses mode 		*/
  if ((options & initOptions::RAW) != initOptions::NOTHING)
    raw(); /* Line buffering disabled	*/
  if ((options & initOptions::KEYPAD) != initOptions::NOTHING)
    keypad(stdscr, TRUE); /* We get F1, F2 etc..		*/
  if ((options & initOptions::NOECHO) != initOptions::NOTHING)
    noecho();
  if (delay)
    halfdelay(delay);
  return stdscr;
}
} // namespace

terminal::terminal(initOptions options, int delay)
    : scr(initcurses(options, delay)) {}

terminal::~terminal() { endwin(); }