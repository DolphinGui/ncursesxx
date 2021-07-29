#pragma once

#include <iostream>
#include <stdexcept>
#include <streambuf>

#include <ncurses.h>

#include "data.hpp"
#include "macros.hpp"
#include "manipcurse.hpp"

namespace ncxx {
template <typename charType>
class window : std::basic_streambuf<charType>,
               public std::basic_iostream<charType> {
  using traits = typename std::basic_streambuf<charType>::traits_type;
  using int_type = typename traits::int_type;

public:
  window(WINDOW *self)
      : internal(self), std::basic_streambuf<charType>(),
        std::basic_iostream<charType>(this) {
    this->unsetf(std::ios_base::skipws);
  }
  window() = default;
  //~window(){};

  friend inline window &operator<<(window &stream, _move s) NCXX_EXCEPT {
    auto result = wmove(stream.internal, s.c.y, s.c.x);
#ifndef NCXX_NO_EXCEPTIONS
    if (result != OK)
      throw std::runtime_error("wmove has failed");
#endif
    return stream;
  }

  friend inline window &operator<<(window &stream, _set s) NCXX_EXCEPT {
    auto result = wattron(stream.internal, s.p._getIdentity());
#ifndef NCXX_NO_EXCEPTIONS
    if (result != OK)
      throw std::runtime_error("attron color has failed");
#endif
    return stream;
  }

  friend inline window &operator<<(window &stream, _unset s) NCXX_EXCEPT {
    auto result = wattroff(stream.internal, s.p._getIdentity());
#ifndef NCXX_NO_EXCEPTIONS
    if (result != OK)
      throw std::runtime_error("attron color has failed");
#endif
    return stream;
  }

protected:
  void imbue(const std::locale &loc) override {
    throw "Not implemented, probably won't be ever";
  }
  // not sure if the error codes even match up
  inline int sync() noexcept override { return wrefresh(internal); }

  inline std::streamsize xsputn(const charType *s,
                                std::streamsize n) noexcept override {
    auto result = waddnwstr(internal, s, n);
    if (result != OK)
      return -1;
    return n;
  }
  inline int_type overflow(int_type c) noexcept override {
    auto result = waddch(internal, c);
    if (result != OK)
      return EOF;
    return traits::to_int_type(result);
  }

  inline int_type underflow() noexcept override {
    static_assert(sizeof(charType) <= sizeof(int_type),
                  "int_type cannot contain charType");
    if (!prev)
      prev = wgetch(internal);
    return traits::to_int_type(prev);
  }

  inline int_type uflow() noexcept override {
    prev = wgetch(internal);
    return traits::to_int_type(prev);
  }

  std::streamsize xsgetn(charType *s, std::streamsize count) noexcept override {
    int successful{};
    for (int i = 0; i < count; i++) {
      auto result = wgetch(internal);
      if (result == ERR)
        return successful;
      else
        *(s + i) = result;
      successful++;
    }
    return successful;
  }

private:
  WINDOW *internal;
  charType prev{};
};
} // namespace ncxx