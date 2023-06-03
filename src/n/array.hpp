#ifndef __n_array_hpp__
#define __n_array_hpp__

#include <n/iterator.hpp>
#include <n/result.hpp>
#include <n/utils.hpp>

namespace n {
template <typename T, size_t N>
class array {
 private:
  T _data[N];
  size_t _len = 0;

 public:
  ~array() = default;
  array() = default;

 public:
  constexpr auto iter() const { return pointer_iterator<const T>(_data, _len); }

  constexpr auto oter() { return pointer_oterator<T>(_data, _len); }

 public:
  constexpr auto len() const { return _len; }
  constexpr auto empty() const { return _len == 0; }
  constexpr auto max() const { return N; }
  constexpr auto full() const { return _len == N; }

 public:
  constexpr void push(const T& t) {
    if (not full()) {
      _data[_len++] = t;
    }
  }

  constexpr void push(T&& t) {
    if (not full()) {
      _data[_len++] = move(t);
    }
  }

  constexpr maybe<T> pop() {
    return not empty() ? maybe<T>(move(_data[--_len])) : maybe<T>();
  }
};
}  // namespace n

#endif
