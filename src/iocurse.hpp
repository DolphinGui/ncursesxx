/** @file iocurse.hpp
 *  @brief the window header
 *
 *  It defines the @ref ncxx::window object, which replaces WINDOW*,
 *  defines various friend operands for window, and is really
 *  the bulk of this library.
 *
 *  @author Shin.
 */

#pragma once

#include <iostream>
#include <stdexcept>
#include <streambuf>

#include <ncurses.h>

#include "data.hpp"
#include "macros.hpp"
#include "manipcurse.hpp"

namespace ncxx {
/**
 * @brief the window object
 * It inherits both streambuf and iostream,
 * so it's usable just like std::cout or cin.
 * it's character type is determined in the @ref macros.hpp
 * file.
 */
class window : std::basic_streambuf<NCXX_CHARTYPE>,
               public std::basic_iostream<NCXX_CHARTYPE> {
  using traits = typename std::basic_streambuf<NCXX_CHARTYPE>::traits_type;
  using int_type = typename traits::int_type;
  friend class terminal;
  /**
   * @brief the internal window constructor
   * It initializes the window pointer, constructs the
   * streambuf and iostream, then sets it to allow whitespace.
   * @param self the pointer to be set internally.
   */
  window(WINDOW *self)
      : internal(self), std::basic_streambuf<NCXX_CHARTYPE>(),
        std::basic_iostream<NCXX_CHARTYPE>(this) {
    this->unsetf(std::ios_base::skipws);
  }

public:
  /**
   * @brief the window constructor
   * It calls newwin(), then delegates to an internal constructor.
   * @param width number of columns wanted
   * @param height number of rows wanted
   * @param coordinate where the top left corner is located.
   */
  window(int width, int height, coordinate location)
      : window(newwin(height, width, location.y, location.x)) {}
  /**
   * @brief the subwindow constructor
   * It calls subwin(), then delegates to an internal constructor.
   * It also touchwin() the parent when updating.
   * @param width number of columns wanted
   * @param height number of rows wanted
   * @param coordinate where the top left corner is located.
   */
  window(const window &parent, int width, int height, coordinate location)
      : window(subwin(parent.internal, height, width, location.y, location.x)) {
    internal = parent.internal;
  }
  window() = default;
  /**
   * @brief the subwindow destructor
   * It calls delwin()
   */
  ~window() { auto result = delwin(internal); };
  /**
   * @brief duplicates a window
   * It calls dupwin(), then constructs an object around it.
   * @param w the window to duplicate
   */
  inline static window duplicate(window w) noexcept {
    return window(dupwin(w.internal));
  }
  /**
   * @brief gets dimentions of window
   * It calls getmaxx and getmaxy
   */
  inline coordinate max() noexcept {
    return {getmaxx(internal), getmaxy(internal)};
  }
  /**
   * @brief moves the window
   * It calls mvwin() on the calling object. If it throws or fails,
   * it's probably because it was outside of the max limit of
   * stdscr.
   * @param location where to move the topleft corner
   */
  inline NCXX_RETURN_TYPE move(coordinate location) NCXX_EXCEPT {
    auto result = mvwin(internal, location.y, location.x);
#ifndef NCXX_NO_EXCEPTIONS
    if (result != OK)
      throw std::runtime_error("wmove has failed");
#endif
    return NCXX_RETURN_VALUE;
  }
  inline explicit operator WINDOW *() { return internal; }
  /**
   * @brief refer to @ref ncxx::setxy()
   */
  NCXX_STREAM_OP_PUSH(_move, wmove(stream.internal, s.c.y, s.c.x),
                      "wmove failed")
  /**
   * @brief refer to @ref ncxx::setColor()
   */
  NCXX_STREAM_OP_PUSH(_set, wattron(stream.internal, s.p), "color set failed")
  /**
   * @brief refer to @ref ncxx::setAttributes()
   */
  NCXX_STREAM_OP_PUSH(_unset, wattroff(stream.internal, s.p),
                      "color unset failed")
  /**
   * @brief refer to @ref ncxx::unsetAttributes()
   */
  NCXX_STREAM_OP_PUSH(_border,
                      wborder(stream.internal, s.ls, s.rs, s.ts, s.bs, s.tl,
                              s.tr, s.bl, s.br),
                      "border set failed")
  /**
   * @brief refer to @ref ::setBackground()
   */
  NCXX_STREAM_OP_PUSH(_setbackground, wbkgd(stream.internal, s.p),
                      "backgroundset failed")
  /**
   * @brief gets the background of the window in color pair and attribute form.
   */
  NCXX_STREAM_OP_PULL(attributeOut &,
                      wattr_get(stream.internal,
                                reinterpret_cast<attr_t *>(&s.c),
                                reinterpret_cast<short *>(s.p._identityHandle()),
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
  inline int sync() noexcept override {
    if (!parent)
      touchwin(parent);
    return wrefresh(internal);
  }

  inline std::streamsize xsputn(const NCXX_CHARTYPE *s,
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
    static_assert(sizeof(NCXX_CHARTYPE) <= sizeof(int_type),
                  "int_type cannot contain NCXX_CHARTYPE");
    if (!prev)
      prev = wgetch(internal);
    return traits::to_int_type(prev);
  }

  inline int_type uflow() noexcept override {
    prev = wgetch(internal);
    return traits::to_int_type(prev);
  }

  std::streamsize xsgetn(NCXX_CHARTYPE *s,
                         std::streamsize count) noexcept override {
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
  WINDOW *parent{};
  NCXX_CHARTYPE prev{};
};
} // namespace ncxx