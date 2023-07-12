#ifndef __n_format_hpp__
#define __n_format_hpp__

#include <n/array.hpp>
#include <n/iterator.hpp>
#include <n/string.hpp>
#include <n/utils.hpp>

namespace n {

template <character C>
inline constexpr C format_joker;

template <>
inline constexpr char format_joker<char> = '$';

template <>
inline constexpr wchar_t format_joker<wchar_t> = L'$';

template <typename O, typename C>
concept ostream = character<C> and requires(O& o, C c) { o.push(c); };

template <character C, ostream<C> O, typename T>
void to_ostream(O& o, const T& t);

template <character C, typename T>
struct formatter;

template <typename T, typename C, typename O>
concept formattable = character<C> and ostream<O, C> and
                      requires(O& o, T t) { n::formatter<C, T>{}(o, t); };

template <character C, ostream<C> O, formattable<C, O> T0,
          formattable<C, O>... TN>
constexpr void __format_to(O& dest, cstring_iterator<C> pattern, const T0& t0,
                           const TN&... tn) {
  while (pattern.has_next()) {
    auto c = pattern.next();

    if (c == format_joker<C>) break;

    dest.push(c);
  }

  n::formatter<C, T0>{}(dest, t0);

  if constexpr (sizeof...(TN) != 0) {
    __format_to(dest, pattern, tn...);
  } else {
    while (pattern.has_next()) dest.push(pattern.next());
  }
}

template <character C, ostream<C> O, formattable<C, O>... T>
constexpr O __format(cstring_iterator<C> pattern, const T&... t) {
  O o;
  __format_to<C>(o, pattern, t...);
  return o;
}

}  // namespace n

namespace n {

template <formattable<char, string<char>>... T>
constexpr string<char> format(cstring_iterator<char> pattern, const T&... t) {
  return __format<char, string<char>>(pattern, t...);
}

template <ostream<char> O, formattable<char, O>... T>
constexpr void format_to(O& dest, cstring_iterator<char> pattern,
                         const T&... t) {
  return __format_to(dest, pattern, t...);
}

}  // namespace n

namespace n {
template <character C>
struct formatter<C, C> {
  constexpr void operator()(ostream<C> auto& o, const C& c) { o.push(c); }
};

template <character C, iterator I>
struct formatter<C, I> {
  constexpr void operator()(ostream<C> auto& o, I i) {
    while (i.has_next()) {
      o.push(i.next());
    }
  }
};

template <character C>
struct formatter<C, C*> : public formatter<C, cstring_iterator<C>> {};

template <character C, size_t N>
struct formatter<C, C[N]> : public formatter<C, cstring_iterator<C>> {};

template <character C>
struct formatter<C, string<C>> {
  constexpr void operator()(ostream<C> auto& o, const string<C>& s) {
    formatter<C, decltype(s.iter())>{}(o, s.iter());
  }
};

template <character C, signed_integral I>
struct formatter<C, I> {
  constexpr void operator()(ostream<C> auto& o, I i) {
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

template <character C, unsigned_integral I>
struct formatter<C, I> {
  constexpr void operator()(ostream<C> auto& o, I i) {
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
struct formatter<C, bool> {
  constexpr void operator()(ostream<C> auto& o, bool b) {
    auto s = cstring_iterator<char>(b ? "true" : "false");
    formatter<C, decltype(s)>{}(o, s);
  }
};

}  // namespace n
#endif
