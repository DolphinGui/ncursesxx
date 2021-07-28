#include "ncursesxx.hpp"
#include <curses.h>
#include <fstream>

int main() {
  auto &t(ncxx::terminal::get(ncxx::initOptions::KEYPAD |
                                  ncxx::initOptions::NOECHO |
                                  ncxx::initOptions::RAW,
                              0));

  t.scr << L"bwrcvxhgs" << std::endl;
  wchar_t a;
  t.scr >> a;
}