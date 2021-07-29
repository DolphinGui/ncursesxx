#pragma once

#ifndef NCXX_CHARTYPE
#define NCXX_CHARTYPE wchar_t
#endif

#ifndef NCXX_NO_EXCEPTIONS
#define NCXX_EXCEPT noexcept
#endif
/*ALWAYS throws if color is not possible
 IF_POSSIBLE will just fail silently.*/
#define NCXX_COLOR_ALWAYS 0
#define NCXX_COLOR_IF_POSSIBLE 1
#define NCXX_COLOR_NEVER 2

#define NCXX_COLOR NCXX_COLOR_IF_POSSIBLE
#define MTX_BITFLAG_OPS_MACRO(ENUM, UNDERLYING)                                \
  constexpr inline ENUM operator&(ENUM left, ENUM right) noexcept {            \
    return static_cast<ENUM>(static_cast<UNDERLYING>(left) &                   \
                             static_cast<UNDERLYING>(right));                  \
  }                                                                            \
  constexpr inline ENUM operator|(ENUM left, ENUM right) noexcept {            \
    return static_cast<ENUM>(static_cast<UNDERLYING>(left) |                   \
                             static_cast<UNDERLYING>(right));                  \
  }                                                                            \
                                                                               \
  constexpr inline ENUM operator^(ENUM left, ENUM right) noexcept {            \
    return static_cast<ENUM>(static_cast<UNDERLYING>(left) ^                   \
                             static_cast<UNDERLYING>(right));                  \
  }                                                                            \
                                                                               \
  constexpr inline ENUM operator~(ENUM right) noexcept {                       \
    return static_cast<ENUM>(~static_cast<UNDERLYING>(right));                 \
  }                                                                            \
                                                                               \
  inline ENUM &operator|=(ENUM &a, ENUM b) {                                   \
    return reinterpret_cast<ENUM &>(reinterpret_cast<UNDERLYING &>(a) |=       \
                                    static_cast<UNDERLYING>(b));               \
  }                                                                            \
                                                                               \
  inline ENUM &operator^=(ENUM &a, ENUM b) {                                   \
    return reinterpret_cast<ENUM &>(reinterpret_cast<UNDERLYING &>(a) ^=       \
                                    static_cast<UNDERLYING>(b));               \
  }                                                                            \
                                                                               \
  inline ENUM &operator&=(ENUM &a, ENUM b) {                                   \
    return reinterpret_cast<ENUM &>(reinterpret_cast<UNDERLYING &>(a) &=       \
                                    static_cast<UNDERLYING>(b));               \
  }
