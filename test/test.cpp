#include <fstream>
#include <iomanip>
#include <ostream>
#include <streambuf>
#include <string>

#include "ncursesxx.hpp"
#include <locale.h>

int main() {

  auto &t(ncxx::terminal::init(ncxx::initOptions::KEYPAD |
                                   ncxx::initOptions::NOECHO |
                                   ncxx::initOptions::RAW,
                               0));
  ncxx::color_pair pair(ncxx::color::GREEN, ncxx::color::RED);
  ncxx::charAttributes before;
  t.scr >> before;
  t.scr << ncxx::setBackground(pair | ncxx::charAttributes::blink) << 
  L"fdasfew";
  wchar_t a;
  t.scr >> a;
  t.scr << ncxx::setBackground(before);
  t.scr >> a;
}