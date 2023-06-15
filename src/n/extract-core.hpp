#ifndef __n_extract_core_hpp__
#define __n_extract_core_hpp__

#include <n/iterator.hpp>
#include <n/result.hpp>
#include <n/utils.hpp>

namespace n {

template <typename T>
class extractor;

template <typename I>
concept istream = iterator<I>;

template <typename T, typename I>
concept extractable =
    istream<I> and requires(I i, maybe<T>& t) { extractor<T>::to(i, t); };

 template <iterator I, typename T>
void extract_one_to(I input, maybe<T>& t) {
  extractor<T>::to(input, t);
}



}  // namespace n

#endif
