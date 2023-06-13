#ifndef __n_extract_hpp__
#define __n_extract_hpp__

#include <n/string.hpp>

namespace n {
template <character C>
void extract(const string<C>& input, const string<C>& pattern,
             maybe<string<C>>& s) {
  auto iinput = input.iter();
  auto ipattern = pattern.iter();

  while (iinput.has_next() && ipattern.has_next()) {
    auto ci = iinput.next();
    auto cp = ipattern.next();

    if (cp == ci) {
      continue;
    } else if (cp == '$') {
      if (ipattern.has_next()) {
        cp = ipattern.next();
        string<char> stmp;
        stmp.push(ci);
        
        while (iinput.has_next() and (ci = iinput.next()) != cp) {
          stmp.push(ci);
        }

        s = move(stmp);
      }
    }
  }
}

template <character C, typename... T>
void extract2(const string<C>& input, const string<C>& pattern,
              maybe<T>&... t) {
  


}
}  // namespace n

#endif
