#ifndef __n_format_hpp__
#define __n_format_hpp__

#include <n/array.hpp>
#include <n/iterator.hpp>
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

template <ostream O, formattable T>
constexpr void __format_one_to(O& dest, iterator auto& ifmt, const T& t) {
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

template <ostream O, formattable... T>
constexpr void __format_to(O& dest, const char* fmt, const T&... t) {
  pointer_iterator<const char> ifmt(fmt, strlen(fmt));
  (__format_one_to(dest, ifmt, t), ...);
  while (ifmt.has_next()) dest.push(ifmt.next());
}

template <formattable... T>
constexpr string<char> __format(const char* fmt, const T&... t) {
  string<char> dest;
  __format_to(dest, fmt, t...);
  return dest;
}

template <formattable... T>
constexpr string<char> format(const char* fmt, const T&... t) {
  return __format(fmt, t...);
}

template <ostream O, formattable... T>
constexpr void format_to(O& dest, const char* fmt, const T&... t) {
  return __format_to(dest, fmt, t...);
}

template <character C>
class formatter<C> {
 public:
  template <ostream O>
  static constexpr void to(O& o, C c) {
    o.push(c);
  }
};

template <iterator I>
class formatter<I> {
 public:
  template <ostream O>
  static constexpr void to(O& o, I i) {
    while (i.has_next()) {
      o.push(i.next());
    }
  }
};

template <character C>
class formatter<C*> : public formatter<cstring_iterator<C>> {};

template <character C, size_t N>
class formatter<C[N]> : public formatter<C*> {};

template <character C>
class formatter<string<C>> {
 public:
  template <ostream O>
  static constexpr void to(O& o, const string<C>& s) {
    auto is = s.iter();
    while (is.has_next()) o.push(is.next());
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
    array<char, 20> tbuff;

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

    maybe<char> m;
    while ((m = tbuff.pop()).has()) {
      o.push(m.get());
    }
  }
};

template <unsigned_integral I>
class formatter<I> {
 public:
  template <ostream O>
  constexpr static void to(O& o, I i) {
    array<char, 20> tbuff;

    if (i == 0) {
      tbuff.push('0');
    } else {
      while (i != 0) {
        tbuff.push("0123456789"[i % 10]);
        i /= 10;
      }
    }

    maybe<char> m;
    while ((m = tbuff.pop()).has()) {
      o.push(m.get());
    }
  }
};

template <>
class formatter<bool> {
 public:
  template <ostream O>
  constexpr static void to(O& o, bool b) {
    string<char> s = (b ? "true" : "false");
    formatter<string<char>>::to(o, s);
  }
};

}  // namespace n
#endif
