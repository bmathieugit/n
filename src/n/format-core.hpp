#ifndef __n_format_core_hpp__
#define __n_format_core_hpp__

#include <n/iterator.hpp>
#include <n/utils.hpp>

namespace n {

template <typename T>
constexpr T format_joker;

template <typename O, typename T>
concept ostream = requires(O o, T t) { o.push(t); };

template <typename T, typename OU>
class formatter;

template <typename T, typename OU, typename O>
concept formattable =
    ostream<O, OU> and requires(T t, O& o) { formatter<T, OU>::to(o, t); };

template <typename OU>
class format_pattern_iterator;

template <typename OU, ostream<OU> O, formattable<OU, O> T>
constexpr void __format_one_to(O& dest, format_pattern_iterator<OU>& ifmt,
                               const T& t) {
  while (ifmt.has_next()) {
    auto c = ifmt.next();

    if (c == format_joker<OU>) break;

    dest.push(c);
  }

  formatter<T, OU>::to(dest, t);
}

template <typename OU, ostream<OU> O, formattable<OU, O>... T>
constexpr void __format_to(O& dest, format_pattern_iterator<OU> pattern,
                           const T&... t) {
  (__format_one_to(dest, pattern, t), ...);
  while (pattern.has_next()) dest.push(pattern.next());
}

template <typename OU, ostream<OU> O, formattable<OU, O>... T>
constexpr O __format(format_pattern_iterator<OU> pattern, const T&... t) {
  O o;
  __format_to(o, pattern, t...);
  return o;
}

}  // namespace n

#endif
