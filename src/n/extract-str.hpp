#ifndef __n_extract_str_hpp__
#define __n_extract_str_hpp__

#include <n/array.hpp>
#include <n/extract-core.hpp>
#include <n/string.hpp>
#include <n/utils.hpp>

namespace n {

template <>
inline constexpr char extract_joker<char> = '$';

template <>
inline constexpr wchar_t extract_joker<wchar_t> = L'$';

template <character C>
class extract_pattern_iterator<C> : public cstring_iterator<C> {
 public:
  constexpr extract_pattern_iterator(const C* s) : cstring_iterator<C>(s) {}
};

template <istream_fragment<char> I, extractable<I, char>... T>
constexpr void extract(I input, extract_pattern_iterator<char> pattern,
                       maybe<T>&... t) {
  return __extract(input, pattern, t...);
}

template <character C>
class extractor<C, C> {
 public:
  template <istream_fragment<C> I>
  static constexpr limit_iterator<I> from(I& i) {
    if (i.has_next()) {
      auto itmp = limit_iterator<I>(i, 1);
      i.next();
      return itmp;
    } else {
      return limit_iterator<I>(i, 0);
    }
  }

  template <istream_fragment<C> I>
  static constexpr void to(I input, maybe<C>& mc) {
    C c = input.next();
    mc = move(c);
  }
};

template <character C>
class extractor<string<C>, C> {
 public:
  template <istream_fragment<C> I>
  static constexpr limit_iterator<I> from(I& i) {
    auto itmp = i;
    auto size = 0;

    if (i.has_next() && i.next() == '"') {
      size++;
      C c;

      while (i.has_next() and (c = i.next()) != '"') size++;

      if (c == '"') {
        size++;
        return limit_iterator<I>(itmp, size);
      }
    }

    return limit_iterator<I>(itmp, 0);
  }

  template <istream_fragment<C> I>
  static constexpr void to(I input, maybe<string<C>>& ms) {
    string<C> s;

    while (input.has_next()) {
      s.push(input.next());
    }

    ms = move(s);
  }
};

template <unsigned_integral SI, character C>
class extractor<SI, C> {
 public:
  template <istream_fragment<C> I>
  static constexpr limit_iterator<I> from(I& i) {
    auto itmp = i;
    auto size = 0;
    C c;
    while (i.has_next() && '0' <= (c = i.next()) && c <= '9') size++;
    return limit_iterator<I>(itmp, size);
  }

  template <istream_fragment<C> I>
  static constexpr void to(I i, maybe<SI>& msi) {
    SI si = 0;

    while (i.has_next()) {
      si = 10 * si + i.next() - '0';
    }

    msi = move(si);
  }
};

template <signed_integral SI, character C>
class extractor<SI, C> {
 public:
  template <istream_fragment<C> I>
  static constexpr limit_iterator<I> from(I& i) {
    C c;
    auto size = 0;
    auto itmp = i;

    if (i.has_next()) {
      c = i.next();
      if (c == '+' || c == '-' || ('0' <= c && c <= '9')) {
        size++;
        while (i.has_next() && '0' <= (c = i.next()) && c <= '9') {
          size++;
        }
        if ('0' <= c && c <= '9') {
          return limit_iterator<I>(itmp, size);
        }
      }
    }

    return limit_iterator<I>(itmp, 0);
  }

  template <istream_fragment<C> I>
  static constexpr void to(I i, maybe<SI>& msi) {
    bool neg = false;
    SI si = 0;

    if (i.has_next()) {
      auto c = i.next();

      if (c == '+') {
        neg = false;
      } else if (c == '-') {
        neg = true;
      } else {
        neg = false;
        si = c - '0';
      }
    }

    while (i.has_next()) {
      si = 10 * si + (i.next() - '0');
    }

    if (neg) {
      si *= -1;
    }

    msi = move(si);
  }
};

template <character C>
class extractor<bool, C> {
 public:
  template <istream_fragment<C> I>
  static constexpr limit_iterator<I> from(I& i) {
    auto itmp = i;
    auto itmp0 = limit_iterator<I>(i, 4);
    auto itmp1 = limit_iterator<I>(i, 5);
    auto size = 0;
    if (equal(itmp0, cstring_iterator("true"))) {
      while (size++ < 4) i.next();
      return itmp0;
    } else if (equal(itmp1, cstring_iterator("false"))) {
      while (size++ < 5) i.next();
      return itmp1;
    } else
      return limit_iterator<I>(itmp, 0);
  }

  template <istream_fragment<C> I>
  static constexpr void to(I i, maybe<bool>& mb) {
    auto itrue = cstring_iterator("true");
    auto ifalse = cstring_iterator("false");

    if (equal(itrue, i)) {
      mb = true;
    } else if (equal(ifalse, i)) {
      mb = false;
    }
  }
};

}  // namespace n

#endif
