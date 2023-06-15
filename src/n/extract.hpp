#ifndef __n_extract_hpp__
#define __n_extract_hpp__

#include <n/array.hpp>
#include <n/extract-core.hpp>
#include <n/string.hpp>

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
class extractor<string<C>, C> {
 public:
  template <istream_fragment<C> I>
  static constexpr void to(I input, maybe<string<C>>& ms) {
    string<C> s;

    while (input.has_next()) {
      s.push(input.next());
    }

    ms = move(s);
  }
};

}  // namespace n

#endif
