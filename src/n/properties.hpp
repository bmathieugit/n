#ifndef __n_properties_hpp__
#define __n_properties_hpp__

#include <n/algorithm.hpp>
#include <n/maybe.hpp>
#include <n/string.hpp>
#include <n/vector.hpp>

namespace n {
  template<iterator I, typename T>
  auto split()
}


namespace n {
template <character C>
struct property {
  string<C> _key;
  string<C> _val;
};

template <character C>
vector<property<C>> parse_properties(const string<C>& _data) {
  vector<property<C>> properties;

 return properties;
}

template <character C>
class properties_reader {};
}  // namespace n

#endif
