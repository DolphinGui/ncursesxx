#include "ncursesxx.hpp"

int main() {
  auto &t(ncxx::terminal::get(ncxx::initOptions::KEYPAD |
                                  ncxx::initOptions::NOECHO |
                                  ncxx::initOptions::RAW,
                              0));
  t.scr <<"ban";
  getch();
}