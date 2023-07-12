#ifndef __n_extract_core_hpp__
#define __n_extract_core_hpp__

#include <n/array.hpp>
#include <n/iterator.hpp>
#include <n/result.hpp>
#include <n/utils.hpp>

namespace n {

template <typename T, typename IU>
class extractor;

template <typename T>
inline constexpr T extract_joker;

template <typename I, typename IU>
concept istream_fragment = iterator<I> and requires(I i) {
                                             { i.next() } -> basic_same_as<IU>;
                                           };
template <typename T>
struct tail_extract {};

template <typename T, typename I, typename IU>
concept extractable = istream_fragment<I, IU> and (requires(I i, maybe<T>& t) {
                                                    extractor<T, IU>::to(i, t);
                                                  } or same_as<T, tail_extract<IU>>);

template <typename T>
class extract_pattern_iterator;

enum class extract_rc : int {
  ok,
  pattern_missing_joker,
  pattern_toomany_joker,
  parsing_failed,
  mismatch_input_pattern,
  empty_input_tail,
  notempty_input_tail,
  notempty_pattern_tail
};

template <typename IU, istream_fragment<IU> I, extractable<I, IU> T0,
          extractable<I, IU>... TN>
constexpr extract_rc __extract(I input, extract_pattern_iterator<IU> pattern,
                                  maybe<T0>& t0, maybe<TN>&... tn) {
  while (input.has_next() and pattern.has_next()) {
    auto&& cp = pattern.next();

    if (cp == extract_joker<IU>) {
      break;
    }

    auto&& ci = input.next();

    if (cp == ci) {
      continue;
    } else {
      return extract_rc::mismatch_input_pattern;
    }
  }

  if constexpr (sizeof...(TN) == 0 and same_as<T0, tail_extract<IU>>) {
    return extract_rc::ok;
  } else {
    if (!input.has_next()) {
      return extract_rc::empty_input_tail;
    }

    size_t len = extractor<T0, IU>::to(input, t0);

    if (len == 0) {
      return extract_rc::parsing_failed;
    }

    while (len != 0 && input.has_next()) {
      input.next();
      len--;
    }

    if constexpr (sizeof...(TN) == 0) {
      while (input.has_next() and pattern.has_next()) {
        if (pattern.next() == input.next()) {
          continue;
        } else {
          return extract_rc::mismatch_input_pattern;
        }
      }

      if (pattern.has_next()) {
        return extract_rc::notempty_pattern_tail;
      } else if (input.has_next()) {
        return extract_rc::notempty_input_tail;
      } else {
        return extract_rc::ok;
      }
    } else if constexpr (sizeof...(TN) > 0) {
      return __extract(input, pattern, tn...);
    }
  }
}

}  // namespace n

#endif
