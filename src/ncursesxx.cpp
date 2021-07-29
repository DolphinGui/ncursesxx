#include "ncursesxx.hpp"
#include "macros.hpp"
#include <stdexcept>
using namespace ncxx;

#if (NCXX_COLOR != NCXX_COLOR_ALWAYS)
#define EXCEPT noexcept
#else
#define EXCEPT
#endif
namespace {
WINDOW *initcurses(initOptions options, int delay) EXCEPT {
  initscr(); /* Start curses mode 		*/
  if ((options & initOptions::RAW) != initOptions::NOTHING)
    raw(); /* Line buffering disabled	*/
  if ((options & initOptions::KEYPAD) != initOptions::NOTHING)
    keypad(stdscr, TRUE); /* We get F1, F2 etc..		*/
  if ((options & initOptions::NOECHO) != initOptions::NOTHING)
    noecho();
  if (delay)
    halfdelay(delay);
#if (NCXX_COLOR != NCXX_COLOR_NEVER)
  auto result = start_color();
#endif
#if (NCXX_COLOR == NCXX_COLOR_ALWAYS)
  if (result != OK)
    throw std::runtime_error("Color is not available from terminal");
#endif
  return stdscr;
}
} // namespace

terminal::terminal(initOptions options, int delay)
    : scr(initcurses(options, delay)) {}

terminal::~terminal() { endwin(); }