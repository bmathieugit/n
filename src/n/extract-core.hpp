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
concept extractable = istream_fragment<I, IU> and requires(I i, maybe<T>& t) {
                                                    extractor<T, IU>::to(i, t);
                                                  };

template <typename T>
class extract_pattern_iterator;

// TODO: renommer en extract_rc
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

template <typename T>
struct tail_extract {};

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

  size_t len = extractor<T0, IU>::to(input, t0);

  if constexpr (not same_as<T0, tail_extract<IU>>) {
    if (len == 0) {
      return extract_error::parsing_failed;
    }
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
        return extract_error::mismatch_input_pattern;
      }
    }

    if (pattern.has_next()) {
      return extract_error::notempty_pattern_tail;
    } else if (input.has_next()) {
      return extract_error::notempty_input_tail;
    } else {
      return extract_error::analyse_ok;
    }
  } else if constexpr (sizeof...(TN) > 0) {
    return __extract(input, pattern, tn...);
  }
}

template <typename T, typename IU>
class extractor<tail_extract<T>, IU> {
 public:
  template <istream_fragment<IU> I>
  static constexpr size_t to(I i, maybe<tail_extract<T>>&) {
    size_t l = 0;

    while (i.has_next()) {
      i.next();
      l += 1;
    }

    return l;
  }
};

}  // namespace n

#endif
