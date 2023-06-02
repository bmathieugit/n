#ifndef __n_format_hpp__
#define __n_format_hpp__

#include <string.h>

#include <n/string.hpp>
#include <n/vector.hpp>

namespace n {

template <typename T>
class formatter;

template <typename T>
concept formattable = requires { formatter<T>{}; };

template <typename T>
concept ostream =
    requires(T t) { t.push('c'); } or requires(T t) { t.push(wchar_t('c')); };

template <ostream O, character C, formattable T>
constexpr void __format_one_to(O& dest, vector_iterator<C>& ifmt, const T& t) {
  while (ifmt.has_next()) {
    auto c = ifmt.next();
    if (c == '$') {
      formatter<T>::to(dest, t);
      break;
    } else {
      dest.push(c);
    }
  }
}

template <ostream O, character C, formattable... T>
constexpr void __format_to(O& dest, const C* fmt, const T&... t) {
  vector_iterator<C> ifmt(fmt, fmt + strlen(fmt));
  (__format_one_to(dest, ifmt, t), ...);
}

template <character C, formattable... T>
constexpr string<C> __format(const C* fmt, const T&... t) {
  string<C> dest;
  __format_to(dest, fmt, t...);
  return dest;
}

template <formattable... T>
constexpr string<char> format(const char* fmt, const T&... t) {
  return __format<char>(fmt, t...);
}

template <formattable... T>
constexpr string<wchar_t> format(const wchar_t* fmt, const T&... t) {
  return __format<wchar_t>(fmt, t...);
}

template <ostream O, formattable... T>
constexpr void format_to(O& dest, const char* fmt, const T&... t) {
  return __format_to(dest, fmt, t...);
}

template <ostream O, formattable... T>
constexpr void format_to(O& dest, const wchar_t* fmt, const T&... t) {
  return __format_to(dest, fmt, t...);
}

template <character C>
class formatter<C> {
 public:
  template <ostream O>
  static constexpr void to(O& dest, C c) {
    dest.push(c);
  }
};

template <character C>
class formatter<string<C>> {
 public:
  template <ostream O>
  static constexpr void to(O& dest, const string<C>& s) {
    auto is = s.iter();
    while (is.has_next()) dest.push(is.next());
  }
};

template <typename T>
concept signed_integral = same_as<T, short> or same_as<T, int> or
                          same_as<T, long> or same_as<T, long long>;

template <typename T>
concept unsigned_integral =
    same_as<T, unsigned short> or same_as<T, unsigned int> or
    same_as<T, unsigned long> or same_as<T, unsigned long long>;

template <signed_integral I>
class formatter<I> {
 public:
  template <ostream O>
  constexpr static void to(O& o, I i) {
    vector<char> tbuff;

    if (i == 0) {
      tbuff.push('0');
    } else {
      const bool neg = i < 0;
      I tmp = neg ? -i : i;

      while (tmp != 0) {
        tbuff.push("0123456789"[tmp % 10]);
        tmp /= 10;
      }

      if (neg) {
        tbuff.push('-');
      }
    }
    auto ibuff = tbuff.iter();
    while (ibuff.has_next()){
      o.push(ibuff.next());
    }
  }
};

}  // namespace n
#endif
