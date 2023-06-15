#ifndef __n_extract_hpp__
#define __n_extract_hpp__

#include <n/array.hpp>
#include <n/string.hpp>
#include <n/extract-core.hpp>

namespace n {

template <character C, typename... T>
void extract(const string<C>& input, const string<C>& pattern, maybe<T>&... t) {
  if constexpr (sizeof...(T) > 0) {
    array<pointer_limit_iterator<const C>, sizeof...(T)> items;

    auto ii = input.iter();
    auto ip = pattern.iter();

    while (ii.has_next() and ip.has_next()) {
      auto start = ii;

      auto ci = ii.next();
      auto cp = ip.next();

      if (cp == ci)
        continue;

      else if (cp == '$') {
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

        items.push(pointer_limit_iterator<const C>(start, offset));
      }
    }

    if (items.len() == sizeof...(T)) {
      auto iitems = items.iter();
      ((extract_one_to(iitems.next(), t)), ...);
    }
  }
}

template <character C>
class extractor<string<C>> {
 public:
  static constexpr void to(pointer_limit_iterator<const C> input,
                           maybe<string<C>>& ms) {
    string<C> s;

    while (input.has_next()) {
      s.push(input.next());
    }

    ms = move(s);
  }
};

}  // namespace n

#endif
