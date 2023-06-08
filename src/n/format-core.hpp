#ifndef __n_format_core_hpp__
#define __n_format_core_hpp__

#include <n/utils.hpp>
#include <n/iterator.hpp>



namespace n {

// JOKER.

template <typename T>
constexpr T format_joker;






// FORMATTER

template <typename O, typename T>
concept ostream = requires(O o, T t) { o.push(t); };

template <typename T, typename OU>
class formatter;

template <typename T, typename OU, typename O>
concept formattable =
    ostream<O, OU> and requires(T t, O& o) { formatter<T, OU>::to(o, t); };

// PATTERN

template <typename OU>
class format_pattern_iterator;

// IMPLEMENTATION

template <typename OU, ostream<OU> O, formattable<OU, O> T>
constexpr void __format_one_to(O& dest, format_pattern_iterator<OU>& ifmt, const T& t) {
  while (ifmt.has_next()) {
    auto c = ifmt.next();

    if (c == format_joker<OU>) break;

    dest.push(c);
  }

  formatter<T, OU>::to(dest, t);
}

template <typename OU, ostream<OU> O, formattable<OU, O>... T>
constexpr void __format_to(O& dest, format_pattern_iterator<OU> pattern, const T&... t) {
  (__format_one_to(dest, pattern, t), ...);
  while (pattern.has_next()) dest.push(pattern.next());
}

template <typename OU, ostream<OU> O, formattable<OU, O>... T>
constexpr O __format(format_pattern_iterator<OU> pattern, const T&... t) {
  O o;
  __format_to(o, pattern, t...);
  return o;
}


// API spécialisée pour les char

template<character C>
class format_pattern_iterator<C> : public cstring_iterator<C>{
  public:
    format_pattern_iterator(const C* s) : cstring_iterator<C>(s){}
};

template <formattable<char, string<char>>... T>
constexpr string<char> format(const char* fmt, const T&... t) {
  return __format<char, string<char>>(format_pattern_iterator<char>(fmt), t...);
}

template <ostream<char> O, formattable<char, O>... T>
constexpr void format_to(O& dest, const char* fmt, const T&... t) {
  return __format_to(dest, format_pattern_iterator<char>(fmt), t...);
}

}  // namespace n

#endif
