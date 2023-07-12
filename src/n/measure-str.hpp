#ifndef __n_measure_str_hpp__
#define __n_measure_str_hpp__

#include <n/measure.hpp>
#include <n/string.hpp>

namespace n {

template <>
inline constexpr char measure_joker<char> = '$';

template <>
inline constexpr wchar_t measure_joker<wchar_t> = L'$';

template <character C>
class measure_pattern_iterator<C> : public cstring_iterator<C> {
 public:
  constexpr measure_pattern_iterator(const C* s) : cstring_iterator<C>(s) {}
};

template <istream<char> I, measurable<I, char>... T>
constexpr void measure_(I input, measure_pattern_iterator<char> pattern,
                       measure<T>&... t) {
  return __measure(input, pattern, t...);
}

template <character C>
class measurator<C, C> {
 public:
  template <istream<C> I>
  static constexpr void measure(I input, measure<C>& mc) {
    if (input.has_next()) {
      mc.value = 1;
    }
  }
};

template <character C>
class measurator<string<C>, C> {
 public:
  template <istream<C> I>
  static constexpr void measure(I i, measure<string<C>>& ms) {
    size_t m = 0;

    if (i.has_next() and i.next() == '"') {
      m += 1;

      C c;

      while (i.has_next() and (c = i.next()) != '"') {
        m += 1;
      }

      if (c == '"') {
        m += 1;
      } else {
        m = 0;
      }
    }

    ms.value = m;
  }
};

template <unsigned_integral SI, character C>
class measurator<SI, C> {
 public:
  template <istream<C> I>
  static constexpr void measure(I i, measure<SI>& msi) {
    size_t m = 0;
    C c;

    while (i.has_next() and '0' <= (c = i.next()) and c <= '9') {
      m += 1;
    }

    msi.value = m;
  }
};

template <signed_integral SI, character C>
class measurator<SI, C> {
 public:
  template <istream<C> I>
  static constexpr void measure(I i, measure<SI>& msi) {
    size_t m = 0;
    C c;

    if (i.has_next()) {
      c = i.next();
      if (c == '+' or c == '-') {
        m += 1;
      }
    }

    while (i.has_next() and '0' <= (c = i.next()) and c <= '9') {
      m += 1;
    }

    msi.value = m;
  }
};

template <character C>
class measurator<bool, C> {
 public:
  template <istream<C> I>
  static constexpr void measure(I i, measure<bool>& mb) {
    constexpr auto itrue = cstring_iterator("true");
    constexpr auto ifalse = cstring_iterator("false");

    if (equal(itrue, i)) {
      mb.value = 4;
    } else if (equal(ifalse, i)) {
      mb.value = 5;
    }
  }
};

}  // namespace n

#endif
