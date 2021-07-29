#include <fstream>
#include <iomanip>
#include <streambuf>
#include <string>

#include "ncursesxx.hpp"

int main() {
  auto &t(ncxx::terminal::init(ncxx::initOptions::KEYPAD |
                                  ncxx::initOptions::NOECHO |
                                  ncxx::initOptions::RAW,
                              0));

  //t.scr << L"bwrcvxhgs" << std::flush;
  std::wstring a;
  std::getline(t.scr, a);
  ncxx::color_pair pair(ncxx::color::RED, ncxx::color::GREEN);
  t.scr << ncxx::setColor(pair) << a;
  t.scr << ncxx::unsetColor(pair) << L"baan";
  //t.scr << std::wse;
 
  wchar_t b;
  t.scr >> b;
}