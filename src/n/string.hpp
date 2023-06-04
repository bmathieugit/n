#ifndef __n_string_hpp__
#define __n_string_hpp__

#include <n/utils.hpp>
#include <n/vector.hpp>

namespace n {

template <character C>
using string = vector<C>;

template <character C>
string<C> str(const C* s) {
  string<C> str;
  
  if (s != nullptr)
    while (*s != '\0') str.push(*(s++));
  
  return str;
}

}  // namespace n

#endif
