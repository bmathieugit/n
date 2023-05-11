#ifndef __n_format_hpp__
#define __n_format_hpp__

#include <stdio.h>

#include <n/string.hpp>
#include <n/utils.hpp>

namespace n {

template <typename T>
class formatter;

template <typename T>
concept formattable = requires { formatter<T>{}; };

template <typename T>
concept ostream =
    requires(T t) { t.push('c'); } || requires(T t) { t.push(wchar_t('c')); };

template <ostream O, formattable T>
constexpr void format_one_to(O &o, const T &t) {
  formatter<rm_cvref<T>>::to(o, t);
}

namespace impl {

template <ostream O, typename I, formattable T>
constexpr void format_to_one_by_one(O &o, I &ifmt, const T &t) {
  while (ifmt.has_next()) {
    auto c = ifmt.next();

    if (c == '$') {
      break;
    } else {
      format_one_to(o, c);
    }
  }

  format_one_to(o, t);
}

template <character C, ostream O, formattable... T>
constexpr void format_to(O &o, string_view<C> fmt, const T &...t) {
  auto ifmt = fmt.iter();

  (format_to_one_by_one(o, ifmt, t), ...);
  format_one_to(o, ifmt);
}

}  // namespace impl

template <ostream O, formattable... T>
constexpr void format_to(O &o, string_view<char> fmt, const T &...t) {
  impl::format_to(o, fmt, t...);
}

template <ostream O, formattable... T>
constexpr void format_to(O &o, string_view<wchar_t> fmt, const T &...t) {
  impl::format_to(o, fmt, t...);
}

template <character C>
class formatter<C> {
 public:
  template <ostream O>
  constexpr static void to(O &os, C c) {
    os.push(c);
  }
};

template <signed_integral I>
class formatter<I> {
 public:
  template <ostream O>
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

template <unsigned_integral I>
class formatter<I> {
 public:
  template <ostream O>
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

template <floating_point F>
class formatter<F> {
 public:
  template <ostream O>
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
  template <ostream O>
  constexpr static void to(O &o, bool b) {
    constexpr string_view<char> _true = "true";
    constexpr string_view<char> _false = "false";
    format_one_to(o, b ? _true : _false);
  }
};

template <iterator I>
class formatter<I> {
 public:
  template <ostream O>
  constexpr static void to(O &o, I i) {
    while (i.has_next()) {
      format_one_to(o, i.next());
    }
  }
};

template <character C>
class formatter<string_view<C>> {
 public:
  template <ostream O>
  constexpr static void to(O &o, string_view<C> s) {
    formatter<decltype(s.iter())>::to(o, s.iter());
  }
};

template <character C>
class formatter<C *> {
 public:
  template <ostream O>
  constexpr static void to(O &o, C *s) {
    formatter<string_view<C>>::to(o, string_view<C>(s));
  }
};

template <character C, size_t N>
class formatter<C[N]> {
 public:
  template <ostream O>
  constexpr static void to(O &o, const C (&s)[N]) {
    formatter<string_view<C>>::to(o, s);
  }
};

template <character C>
class formatter<const C *> {
 public:
  template <ostream O>
  constexpr static void to(O &o, const C *s) {
    formatter<string_view<C>>::to(o, string_view<C>(s));
  }
};

template <character C, size_t N>
class formatter<const C[N]> {
 public:
  template <ostream O>
  constexpr static void to(O &o, const C (&s)[N]) {
    formatter<string_view<C>>::to(o, s);
  }
};

template <pointer_eq P>
class formatter<P> {
 public:
  template <ostream O>
  constexpr static void to(O &os, P p) {
    format_one_to(os, (size_t)(void *)(p));
  }
};

}  // namespace n
#endif
