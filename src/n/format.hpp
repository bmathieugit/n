#ifndef __n_format_hpp__
#define __n_format_hpp__

#include <n/string.hpp>
#include <n/utils.hpp>

namespace n {

template <typename T>
class formatter;

/*template <typename T, typename >
concept formattable =
    ostream<O> && requires(const T &t, O &o) { formatter<T>::to(o, t); };
*/

template <typename O, typename T>
constexpr void format_one_to(O &o, const T &t) {
  formatter<rm_cvref<T>>::to(o, t);
}

namespace impl {

template <typename O, typename I, typename H>
constexpr void format_to_one_by_one(O &o, I &ifmt, const H &h) {
  while (ifmt.has_next()) {
    auto c = ifmt.next();

    if (c == '$') {
      break;
    } else {
      format_one_to(o, c);
    }
  }

  format_one_to(o, h);
}

template <character C, typename O, typename... T>
constexpr void format_to(O &o, string_view<C> fmt, const T &...t) {
  auto ifmt = fmt.iter();
  (format_to_one_by_one(o, ifmt, t), ...);
  while (ifmt.has_next()) format_one_to(o, ifmt.next());
}

}  // namespace impl

template <typename O, typename... A>
constexpr void format_to(O &o, string_view<char> fmt, const A &...a) {
  impl::format_to(o, fmt, a...);
}

template <typename O, typename... A>
constexpr void format_to(O &o, string_view<wchar_t> fmt, const A &...a) {
  impl::format_to(o, fmt, a...);
}

template <character C>
class formatter<C> {
 public:
  template <typename O>
  constexpr static void to(O &os, C o) {
    os.push(o);
  }
};

template <typename I>
concept signed_integral = same_as<I, short> || same_as<I, int> ||
                          same_as<I, long> || same_as<I, long long>;

template <signed_integral I>
class formatter<I> {
 public:
  template <typename O>
  constexpr static void to(O &o, I i) {
    static_vector<char, 20> tbuff;

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

    format_one_to(o, tbuff.riter());
  }
};

template <typename I>
concept unsigned_integral =
    same_as<I, unsigned short> || same_as<I, unsigned int> ||
    same_as<I, unsigned long> || same_as<I, unsigned long long>;

template <unsigned_integral I>
class formatter<I> {
 public:
  template <typename O>
  constexpr static void to(O &o, I i) {
    static_vector<char, 20> tbuff;

    if (i == 0) {
      tbuff.push('0');
    } else {
      while (i != 0) {
        tbuff.push("0123456789"[i % 10]);
        i /= 10;
      }
    }

    format_one_to(o, tbuff.riter());
  }
};

template <typename F>
concept floating_point = same_as<F, float> || same_as<F, double>;

template <floating_point F>
class formatter<F> {
 public:
  template <typename O>
  constexpr static void to(O &o, F d) {
    size_t i = static_cast<size_t>(d);
    size_t e = static_cast<size_t>((d - i) * 10'000.0);

    format_one_to(o, i);
    format_one_to(o, '.');
    format_one_to(o, e);
  }
};

template <>
class formatter<bool> {
 public:
  template <typename O>
  constexpr static void to(O &o, bool b) {
    constexpr string_view<char> _true = "true";
    constexpr string_view<char> _false = "false";
    format_one_to(o, b ? _true : _false);
  }
};

template <pointer_eq P>
class formatter<P> {
 public:
  template <typename O>
  constexpr static void to(O &os, P p) {
    format_one_to(os, (size_t)(void *)(p));
  }
};

template <typename I>
concept iterator = requires(I i) {
                     i.has_next();
                     i.next();
                   };

template <iterator I>
class formatter<I> {
 public:
  template <typename O>
  constexpr static void to(O &o, I i) {
    while (i.has_next()) {
      auto c = i.next();
      o.push(c);
    }
  }
};
/*
template <iterable I>
class formatter<I> {
 public:
  template <typename O>
  constexpr static void to(O &o, const I &i) {
    formatter<decltype(i.iter())>::to(o, i.iter());
  }
};
*/
}  // namespace n
#endif
