#pragma once

#ifndef NCXX_CHARTYPE
#define NCXX_CHARTYPE wchar_t
#endif

#ifndef NCXX_NO_EXCEPTIONS
#define NCXX_EXCEPT noexcept
#define NCXX_RETURN_TYPE void
#define NCXX_RETURN_VALUE
#else
#define NCXX_RETURN_TYPE int
#define NCXX_RETURN_VALUE result
#endif

/*ALWAYS throws if color is not possible
 IF_POSSIBLE will just fail silently.*/
#define NCXX_COLOR_ALWAYS 0
#define NCXX_COLOR_IF_POSSIBLE 1
#define NCXX_COLOR_NEVER 2

#define NCXX_COLOR NCXX_COLOR_IF_POSSIBLE
#ifndef NCXX_NO_EXCEPTIONS
#define NCXX_STREAM_OP_PUSH(MANIPTYPE, COMMAND, EXCEPTION_MESSAGE)\
friend inline window &operator<<(window &stream, MANIPTYPE s) NCXX_EXCEPT {\
  auto result = COMMAND;\
  if (result != OK)\
    throw std::runtime_error(EXCEPTION_MESSAGE);\
  return stream;\
}
#else
#define NCXX_STREAM_OP(MANIPTYPE, COMMAND)\
friend inline window &operator<<(window &stream, MANIPTYPE s) NCXX_EXCEPT {\
  auto result = COMMAND;\
  return stream;\
}
#endif

#ifndef NCXX_NO_EXCEPTIONS
#define NCXX_STREAM_OP_PULL(MANIPTYPE, COMMAND, EXCEPTION_MESSAGE)\
friend inline window &operator>>(window &stream, MANIPTYPE s) NCXX_EXCEPT {\
  auto result = COMMAND;\
  if (result != OK)\
    throw std::runtime_error(EXCEPTION_MESSAGE);\
  return stream;\
}
#else
#define NCXX_STREAM_OP(MANIPTYPE, COMMAND)\
friend inline window &operator<<(window &stream, MANIPTYPE s) NCXX_EXCEPT {\
  auto result = COMMAND;\
  return stream;\
}
#endif

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
