#ifndef __n_vector_hpp__
#define __n_vector_hpp__

#include <assert.h>

#include <n/maybe.hpp>
#include <n/utils.hpp>

namespace n {

using size_t = unsigned long long;

template <typename T>
class vector_iterator {
 private:
  T *_begin = nullptr;
  T *_end = nullptr;

 public:
  constexpr vector_iterator() = default;
  constexpr vector_iterator(T *begin, size_t n)
      : _begin(begin), _end(begin + n) {}

 public:
  constexpr bool has_next() const { return _begin != _end; }
  constexpr T &next() { return *(_begin++); }
};

template <typename T, size_t N>
class static_vector {
 private:
  T _data[N];
  size_t _size = 0;

 public:
  constexpr ~static_vector() = default;
  constexpr static_vector() = default;

  // move
  constexpr static_vector(static_vector &&) = default;
  constexpr static_vector &operator=(static_vector &&) = default;

  // copy
  constexpr static_vector(const static_vector &) = default;
  constexpr static_vector &operator=(const static_vector &) = default;

 public:
  constexpr auto size() const { return _size; }
  constexpr auto empty() const { return _size == 0; }
  constexpr auto full() const { return _size == N; }

 public:
  constexpr auto iter() { return vector_iterator<T>(_data._data, _size); }

  constexpr auto iter() const {
    return vector_iterator<const T>(_data._data, _size);
  }

  /* constexpr auto riter() {
     return reverse_pointer_iterator<T>(_data._data + _size - 1,
                                        _data._data - 1);
   }

   constexpr auto riter() const {
     return reverse_pointer_iterator<const T>(_data._data + _size - 1,
                                              _data._data - 1);
   }*/

  constexpr T &operator[](size_t i) {
    assert(i < _size);
    return _data[i];
  }

  constexpr const T &operator[](size_t i) const {
    assert(i < _size);
    return _data[i];
  }

 public:
  constexpr void push(const T &t) {
    if (!full()) {
      _data[_size++] = t;
    }
  }

  constexpr void push(T &&t) {
    if (!full()) {
      _data[_size++] = transfer(t);
    }
  }

  constexpr maybe<T> pop() {
    if (_size != 0) {
      return maybe<T>(transfer(_data[_size--]));
    } else {
      return maybe<T>();
    }
  }

  constexpr void clear() { _size = 0; }
};

}  // namespace n

#endif
