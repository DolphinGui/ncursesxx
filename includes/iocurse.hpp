#pragma once

#include <iostream>
#include <stdexcept>
#include <streambuf>

#include <ncurses.h>

#include "data.hpp"
#include "macros.hpp"
#include "manipcurse.hpp"

namespace ncxx {
template <typename charType = NCXX_CHARTYPE>
class window : std::basic_streambuf<charType>,
               public std::basic_iostream<charType> {
  using traits = typename std::basic_streambuf<charType>::traits_type;
  using int_type = typename traits::int_type;
  friend class terminal;
  window(WINDOW *self)
      : internal(self), std::basic_streambuf<charType>(),
        std::basic_iostream<charType>(this) {
    this->unsetf(std::ios_base::skipws);
  }

public:
  window(int width, int height, coordinate location)
      : window(newwin(height, width, location.y, location.x)) {}
  window(const window &parent, int width, int height, coordinate location)
      : window(subwin(parent.internal, height, width, location.y, location.x)) {
  }
  window() = default;
  ~window() { auto result = delwin(internal); };
  inline window duplicate(window other) noexcept {
    return window(dupwin(other.internal));
  }
  inline coordinate max() noexcept {
    return {getmaxx(internal), getmaxy(internal)};
  }

  /*window &operator<<(window &(*pf)(window &));*/

  inline NCXX_RETURN_TYPE move(coordinate location) NCXX_EXCEPT {
    auto result = mvwin(internal, location.y, location.x);
#ifndef NCXX_NO_EXCEPTIONS
    if (result != OK)
      throw std::runtime_error("wmove has failed");
#endif
    return NCXX_RETURN_VALUE;
  }

  inline explicit operator WINDOW *() { return internal; }
  NCXX_STREAM_OP_PUSH(_move, wmove(stream.internal, s.c.y, s.c.x),
                      "wmove failed")
  NCXX_STREAM_OP_PUSH(_set,
                      wattron(stream.internal, COLOR_PAIR(s.p._getIdentity())),
                      "color set failed")
  NCXX_STREAM_OP_PUSH(_unset,
                      wattroff(stream.internal, COLOR_PAIR(s.p._getIdentity())),
                      "color unset failed")
  NCXX_STREAM_OP_PUSH(_border,
                      wborder(stream.internal, s.ls, s.rs, s.ts, s.bs, s.tl,
                              s.tr, s.bl, s.br),
                      "border set failed")
  NCXX_STREAM_OP_PUSH(_setbackground, wbkgd(stream.internal, s.p),
                      "backgroundset failed")
  NCXX_STREAM_OP_PULL(attributeOut &,
                      wattr_get(stream.internal,
                                reinterpret_cast<attr_t *>(&s.c),
                                reinterpret_cast<short *>(s.p.identity),
                                nullptr),
                      "wget_attr failed")
  friend inline window &operator>>(window &stream, charAttributes s) noexcept {
    s = static_cast<charAttributes>(getbkgd(stream.internal));
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