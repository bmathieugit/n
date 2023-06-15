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

template <typename T, typename I, typename IU>
concept extractable =
    istream_fragment<I, IU> and
    requires(I i, maybe<T>& t) {
      extractor<T, IU>::to(limit_iterator<I>(i, size_t(0)), t);
    };

template <typename T>
class extract_pattern_iterator;

enum class extract_error : int {
  analyse_ok,
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
constexpr extract_error __extract(I input, extract_pattern_iterator<IU> pattern,
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
      return extract_error::mismatch_input_pattern;
    }
  }

  if (!input.has_next()) {
    return extract_error::empty_input_tail;
  }

  auto len = extractor<T0, IU>::len(input);

  if (len == 0 or !extractor<T0, IU>::to(limit_iterator<I>(input, len), t0)) {
    return extract_error::parsing_failed;
  }

  while (len != 0 && input.has_next()) {
    input.next();
    len--;
  }

  if constexpr (sizeof...(TN) > 0) {
    return __extract(input, pattern, tn...);
  } else if (pattern.has_next()) {
    return extract_error::notempty_pattern_tail;
  } else if (input.has_next()) {
    return extract_error::notempty_input_tail;
  } else {
    return extract_error::analyse_ok;
  }
}

}  // namespace n

#endif
