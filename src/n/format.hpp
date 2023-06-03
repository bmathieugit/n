#ifndef __n_format_hpp__
#define __n_format_hpp__

#include <n/array.hpp>
#include <n/iterator.hpp>
#include <n/string.hpp>
#include <n/vector.hpp>
#include <n/format-core.hpp>

namespace n {

template <character C>
class formatter<C, C> {
 public:
  template <ostream<C> O>
  static constexpr void to(O& o, C c) {
    o.push(c);
  }
};

template <iterator I, character C>
class formatter<I, C> {
 public:
  template <ostream<C> O>
  static constexpr void to(O& o, I i) {
    while (i.has_next()) {
      o.push(i.next());
    }
  }
};

template <character C>
class formatter<C*, C> : public formatter<cstring_iterator<C>, C> {};

template <character C, size_t N>
class formatter<C[N], C> : public formatter<C*, C> {};

template <character C>
class formatter<string<C>, C> {
 public:
  template <ostream<C> O>
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

template <signed_integral I, character C>
class formatter<I, C> {
 public:
  template <ostream<C> O>
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

template <unsigned_integral I, character C>
class formatter<I, C> {
 public:
  template <ostream<C> O>
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

template <character C>
class formatter<bool, C> {
 public:
  template <ostream<C> O>
  constexpr static void to(O& o, bool b) {
    auto s = cstring_iterator<char>(b ? "true" : "false");
    formatter<cstring_iterator<char>, C>::to(o, s);
  }
};

}  // namespace n
#endif
