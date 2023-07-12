#ifndef __n_measure_hpp__
#define __n_measure_hpp__

#include <n/iterator.hpp>

namespace n {
template <typename T, typename IU>
class measurator;

template <typename IU>
inline constexpr IU measure_joker;

template <typename I, typename IU>
concept istream = iterator<I> and requires(I i) {
                                    { i.next() } -> basic_same_as<IU>;
                                  };

template <typename T>
struct measure {
  size_t value = 0;
};

template <typename T, typename I, typename IU>
concept __has_measure_method = requires(I i, measure<T>& u) {
                                 {
                                   measurator<T, IU>::measure(i)
                                   } -> basic_same_as<size_t>;
                               };

template <typename T, typename I, typename IU>
concept measurable = istream<I, IU> and __has_measure_method<T, I, IU>;

template <typename IU>
class measure_pattern_iterator;

template <typename IU, istream<IU> I, measurable<I, IU> T0,
          measurable<I, IU>... TN>
constexpr void __measure(I input, measure_pattern_iterator<IU> pattern,
                         measure<T0>& m0, measure<TN>&... mn) {
  bool jokered = false;

  while (input.has_next() and pattern.has_next()) {
    auto&& cp = pattern.next();
    auto&& ci = input.next();

    if (cp != ci) {
      break;
    }

    if (cp == measure_joker<IU>) {
      jokered = true;
    }
  }

  if (jokered) {
    measurator<T0, IU>::measure(input, m0);
  
    if constexpr (sizeof...(TN) != 0) {
      __measure<IU>(input, pattern, mn...);
    }
  }
}

}  // namespace n

#endif
