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

template <typename IU, istream_fragment<IU> I, typename T>
void __extract_one_to(I input, maybe<T>& t) {
  extractor<T, IU>::to(input, t);
}

template <typename T>
class extract_pattern_iterator;

template<typename IU, istream_fragment<IU> I> 
constexpr void __extract_escape(I& i, extract_pattern_iterator<IU>& pattern) {

}

template <typename IU, istream_fragment<IU> I, extractable<I, IU>... T>
constexpr void __extract(I input, extract_pattern_iterator<IU> pattern, maybe<T>&... t) {
  if constexpr (sizeof...(T) > 0) {
    array<limit_iterator<I>, sizeof...(T)> items;

    auto ii = input;
    auto ip = pattern;

    ((__extract_escape(input, pattern), items.push(extractor<T, IU>::from(input))), ...);
/*
    while (ii.has_next() and ip.has_next()) {
      auto start = ii;

      auto ci = ii.next();
      auto cp = ip.next();

      if (cp == ci)
        continue;

      else if (cp == extract_joker<IU>) {
        auto offset = 1;

        if (ip.has_next()) {
          auto limit = ip.next();

          while (ii.has_next() && (ci = ii.next()) != limit) {
            offset += 1;
          }
        } else {
          while (ii.has_next()) {
            ii.next();
            offset += 1;
          }
        }

        items.push(limit_iterator<I>(start, offset));
      }
    }
*/
    if (items.len() == sizeof...(T)) {
      auto iitems = items.iter();
      ((__extract_one_to<IU>(iitems.next(), t)), ...);
    }
  }
}

}  // namespace n

#endif
