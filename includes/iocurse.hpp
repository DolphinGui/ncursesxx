#pragma once

#include <exception>
#include <ios>
#include <iostream>
#include <istream>
#include <ncurses.h>
#include <ostream>
#include <stdexcept>
#include <streambuf>
#include <string>

#include "data.hpp"

namespace ncxx {
class window : std::basic_streambuf<wchar_t>,
               public std::basic_iostream<wchar_t> {
public:
  window(WINDOW *self)
      : internal(self), std::basic_streambuf<wchar_t>(),
        std::basic_iostream<wchar_t>(this){};
  //~window();

protected:
  void imbue(const std::locale &loc) override {
    throw "Not implemented, probably won't be ever";
  }
  // not sure if the error codes even match up
  inline int sync() noexcept override { return wrefresh(internal); }

  inline std::streamsize xsputn(const wchar_t *s,
                                std::streamsize n) noexcept override {
    auto result = waddnwstr(internal, s, n);
    if (result != OK)
      return -1;
    return n;
  }
  inline uint overflow(uint c) override {
    auto result = waddch(internal, c);
    if (result != OK)
      return EOF;
    return 0;
  }

  uint underflow() override {
    wchar_t ch = wgetch(internal);
    return ch;
  }

  std::streamsize xsgetn(wchar_t *s,
                          std::streamsize count) override {
    int successful{};
    for (int i = 0; i < count; i++) {
      auto result = wgetch(internal);
      if (result != OK)
        return successful;
      else
        *(s + i) = result;
      successful++;
    }
    return successful;
  }

private:
  WINDOW *internal;
};
} // namespace ncxx