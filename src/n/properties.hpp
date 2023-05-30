#ifndef __n_properties_hpp__
#define __n_properties_hpp__

#include <n/algorithm.hpp>
#include <n/string.hpp>
#include <n/vector.hpp>

namespace n {
template <character C>
struct property {
  string<C> _key;
  string<C> _val;
};

template <character C>
vector<property<C>> parse_properties(const string<C>& _data) {
  vector<property<C>> properties;

  auto idata = _data.iter();

  while (idata.has_next()) {
    
    auto idx = find(idata, '\n');
    string<C> line(idx);
    
    auto iline = line.iter();
    auto idx_eq = find(iline, '=');

    if (idx_eq != size_t(-1)) {
      string<C> key(idx_eq);

      while (iline.as_next()) {
        auto cv = iline.next();
        key.push(cv);
        
        if (cv == '=') {
          break;
        }
  
      }
    }

    while (iline.has_next()) {
      
    }
    
  }

  return properties;
}

template <character C>
class properties_reader {};
}  // namespace n

#endif
