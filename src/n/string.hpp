#ifndef __n_string_hpp__
#define __n_string_hpp__

#include <n/utils.hpp>
#include <n/vector.hpp>

namespace n {

template <character C>
class string {
 private:
  vector<C> _data;

 public:
  constexpr ~string() = default;
  constexpr string() = default;
  
  constexpr string(const C* str) {
    while (*str != '\0') {
      _data.push(*(str++));
    }
  }

  constexpr string(const string& o) : _data(o._data) {}

  constexpr string(string&& o) : _data(n::move(o._data)) {}

  constexpr string& operator=(const string& o) {
    if (this != &o) {
      _data = o._data;
    }

    return *this;
  }

  constexpr string& operator=(string&& o) {
    if (this != &o) {
      _data = n::move(o._data);
    }

    return *this;
  }

 public:
  constexpr auto iter() const { return _data.iter(); }
  constexpr auto oter() { return _data.oter(); }

 public:
  constexpr auto len() const { return _data.len() ; }
  constexpr auto empty() const { return _data.empty() ; }
  constexpr auto max() const { return _data.max() ; }
  constexpr auto full() const { return _data.full(); }

 public:
  constexpr void push(C c) {
    _data.push(c);
  }

  constexpr maybe<C> pop() {
    return _data.pop();
  }

  constexpr void clear() { _data.clear(); }
};

}  // namespace n

#endif
