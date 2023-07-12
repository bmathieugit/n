#ifndef __n_extract_hpp__
#define __n_extract_hpp__

#include <n/iterator.hpp>
#include <n/string.hpp>
#include <n/utils.hpp>

namespace n {
template <character C, typename T>
struct extractor;

template <typename T>
inline constexpr T extract_joker;

template <>
inline constexpr char extract_joker<char> = '$';

template <>
inline constexpr wchar_t extract_joker<wchar_t> = L'$';

template <typename I, typename C>
concept istream = iterator<I> and character<C>;

template <typename T, typename I, typename C>
concept extractable =
    istream<I, C> and requires(I i, maybe<T>& t) { extractor<C, T>{}(i, t); };

template <character C, istream<C> I, extractable<I, C> T0,
          extractable<I, C>... TN>
constexpr void __extract(I input, cstring_iterator<C> pattern, maybe<T0>& t0,
                         maybe<TN>&... tn) {
  while (input.has_next() and pattern.has_next()) {
    C cp = pattern.next();

    if (cp == extract_joker<C>) {
      break;
    }

    C ci = input.next();

    if (cp == ci) {
      continue;
    }
  }

  if (input.has_next()) {
    input = advance(input, extractor<C, T0>{}(input, t0));
  }

  if constexpr (sizeof...(TN) != 0) {
    __extract<C>(input, pattern, tn...);
  }
}

template <istream<char> I, extractable<I, char>... T>
constexpr void extract(I input, cstring_iterator<char> pattern,
                       maybe<T>&... t) {
  return __extract(input, pattern, t...);
}

template <character C>
struct extractor<C, C> {
  constexpr size_t operator()(istream<C> auto input, maybe<C>& mc) {
    if (input.has_next()) {
      mc = move(input.next());
      return 1;
    } else {
      return 0;
    }
  }
};

template <character C>
struct extractor<C, string<C>> {
  constexpr size_t operator()(istream<C>auto input, maybe<string<C>>& ms) {
    string<C> tmp;
    size_t l = 0;
    if (input.has_next()) {
      auto c = input.next();

      if (c != '"') {
        return 0;
      } else {
        ++l;
      }

      while (input.has_next()) {
        c = input.next();

        if (c != '"') {
          ++l;
          tmp.push(c);
        } else {
          break;
        }
      }

      if (c == '"') {
        ++l;
        ms = move(tmp);
        return l;
      }
    }

    return 0;
  }
};

template <character C, unsigned_integral SI>
struct extractor<C, SI> {
  constexpr size_t operator()(istream<C>auto input, maybe<SI>& msi) {
    if (input.has_next()) {
      size_t l = 0;
      auto c = input.next();

      if ('0' <= c && c <= '9') {
        ++l;
        SI si = c - '0';

        while (input.has_next()) {
          c = input.next();

          if ('0' <= c and c <= '9') {
            si = 10 * si + c - '0';
            ++l;
          } else {
            break;
          }
        }

        msi = move(si);
        return l;
      }
    }

    return 0;
  }
};

template <character C, signed_integral SI>
struct  extractor<C, SI> {
   constexpr size_t operator()(istream<C>auto input, maybe<SI>& msi) {
    size_t l = 0;

    if (input.has_next()) {
      auto c = input.next();
      bool neg = false;

      if (c == '-') {
        neg = true;
        ++l;
        if (input.has_next()) {
          c = input.next();
        }
      } else if (c == '+' && input.has_next()) {
        ++l;
        c = input.next();
      }

      if ('0' <= c && c <= '9') {
        SI si = c - '0';
        ++l;

        while (input.has_next()) {
          c = input.next();

          if ('0' <= c and c <= '9') {
            si = 10 * si + c - '0';
            ++l;
          } else {
            break;
          }
        }

        msi = move(neg ? -si : si);
        return l;
      }
    }

    return 0;
  }
};

template <character C>
struct extractor<C, bool> {
   constexpr size_t operator()(istream<C> auto i, maybe<bool>& mb) {
    constexpr auto itrue = cstring_iterator("true");
    constexpr auto ifalse = cstring_iterator("false");

    if (equal(itrue, i)) {
      mb = true;
      return 4;
    } else if (equal(ifalse, i)) {
      mb = false;
      return 5;
    } else {
      return 0;
    }
  }
};


}  // namespace n
#endif
