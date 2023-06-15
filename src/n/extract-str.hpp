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
constexpr extract_error extract(I input, extract_pattern_iterator<char> pattern,
                                maybe<T>&... t) {
  return __extract(input, pattern, t...);
}

template <character C>
class extractor<C, C> {
 public:
  template <istream_fragment<C> I>
  static constexpr size_t len(I input) {
    return input.has_next() ? 1 : 0;
  }

  template <istream_fragment<C> I>
  static constexpr bool to(limit_iterator<I> input, maybe<C>& mc) {
    if (input.has_next()) {
      mc = move(input.next());
      return true;
    } else {
      return false;
    }
  }
};

template <character C>
class extractor<string<C>, C> {
 public:
  template <istream_fragment<C> I>
  static constexpr size_t len(I input) {
    if (input.has_next() and input.next() == '"') {
      size_t l = 1;

      if (input.has_next()) {
        auto c = input.next();

        if (c == '"') {
          return 2;
        } else {
          l++;
        }

        while (input.has_next()) {
          c = input.next();

          if (c != '"') {
            ++l;
          } else {
            break;
          }
        }

        if (c == '"') {
          return l + 1;
        }
      }
    }

    return 0;
  }

  template <istream_fragment<C> I>
  static constexpr bool to(limit_iterator<I> input, maybe<string<C>>& ms) {
    string<C> tmp;

    if (input.has_next() && input.len() >= 2) {
      auto c = input.next();

      while (input.has_next()) {
        c = input.next();

        if (c != '"') {
          tmp.push(c);
        } else {
          break;
        }
      }

      if (c == '"') {
        ms = move(tmp);
        return true;
      }
    }

    return false;
  }
};

template <unsigned_integral SI, character C>
class extractor<SI, C> {
 public:
  template <istream_fragment<C> I>
  static constexpr size_t len(I input) {
    size_t l = 0;

    while (input.has_next()) {
      auto c = input.next();

      if ('0' <= c and c <= '9') {
        ++l;
      }
    }

    return l;
  }

  template <istream_fragment<C> I>
  static constexpr bool to(limit_iterator<I> input, maybe<SI>& msi) {
    if (input.has_next()) {
      auto c = input.next();

      if ('0' <= c && c <= '9') {
        SI si = c - '0';

        while (input.has_next()) {
          c = input.next();

          if ('0' <= c and c <= '9') {
            si = 10 * si + c - '0';
          } else {
            break;
          }
        }

        msi = move(si);
        return true;
      }
    }

    return false;
  }
};

template <signed_integral SI, character C>
class extractor<SI, C> {
 public:
  template <istream_fragment<C> I>
  static constexpr size_t len(I input) {
    size_t l = 0;
    if (input.has_next()) {
      auto c = input.next();

      if (c == '+' or c == '-' or ('0' <= c and c <= '9')) {
        ++l;

        while (input.has_next()) {
          c = input.next();

          if ('0' <= c and c <= '9') {
            ++l;
          } else {
            break;
          }
        }
      }
    }

    return l;
  }

  template <istream_fragment<C> I>
  static constexpr bool to(limit_iterator<I> input, maybe<SI>& msi) {
    if (input.has_next()) {
      auto c = input.next();
      bool neg = false;

      if (c == '-') {
        neg = true;

        if (input.has_next()) {
          c = input.next();
        }
      } else if (c == '+' && input.has_next()) {
        c = input.next();
      }

      if ('0' <= c && c <= '9') {
        SI si = c - '0';

        while (input.has_next()) {
          c = input.next();
        
          if ('0' <= c and c <= '9') {
            si = 10 * si + c - '0';
          } else {
            break;
          }
        }

        msi = move(neg ? -si : si);
        return true;
      }
    }

    return false;
  }
};

template <character C>
class extractor<bool, C> {
 public:
  template <istream_fragment<C> I>
  static constexpr size_t len(I i) {
    constexpr auto itrue = cstring_iterator("true");
    constexpr auto ifalse = cstring_iterator("false");

    if (equal(itrue, i)) {
      return 4;
    } else if (equal(ifalse, i)) {
      return 5;
    } else {
      return 0;
    }
  }

  template <istream_fragment<C> I>
  static constexpr bool to(limit_iterator<I> i, maybe<bool>& mb) {
    constexpr auto itrue = cstring_iterator("true");
    constexpr auto ifalse = cstring_iterator("false");

    if (equal(itrue, i)) {
      mb = true;
      return true;
    } else if (equal(ifalse, i)) {
      mb = false;
      return true;
    } else {
      return false;
    }
  }
};

}  // namespace n

#endif
