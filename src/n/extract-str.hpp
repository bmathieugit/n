#ifndef __n_extract_str_hpp__
#define __n_extract_str_hpp__

#include <n/array.hpp>
#include <n/extract-core.hpp>
#include <n/iterator.hpp>
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
constexpr extract_rc extract(I input, extract_pattern_iterator<char> pattern,
                                maybe<T>&... t) {
  return __extract(input, pattern, t...);
}

template <character C>
class extractor<C, C> {
 public:
  template <istream_fragment<C> I>
  static constexpr size_t  to(I input, maybe<C>& mc) {
    if (input.has_next()) {
      mc = move(input.next());
      return 1;
    } else {
      return 0;
    }
  }
};

template <character C>
class extractor<string<C>, C> {
 public:
  template <istream_fragment<C> I>
  static constexpr size_t to(I input, maybe<string<C>>& ms) {
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

template <unsigned_integral SI, character C>
class extractor<SI, C> {
 public:
  template <istream_fragment<C> I>
  static constexpr size_t to(I input, maybe<SI>& msi) {
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

template <signed_integral SI, character C>
class extractor<SI, C> {
 public:
  template <istream_fragment<C> I>
  static constexpr size_t to(I input, maybe<SI>& msi) {
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
class extractor<bool, C> {
 public:
  template <istream_fragment<C> I>
  static constexpr size_t to(I i, maybe<bool>& mb) {
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
