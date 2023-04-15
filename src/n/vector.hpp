#ifndef __n_vector_hpp__
#define __n_vector_hpp__

#include <assert.h>

#include <n/maybe.hpp>
#include <n/utils.hpp>

namespace n {


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

template <typename T>
class reverse_vector_iterator {
 private:
  T *_begin = nullptr;
  T *_end = nullptr;

 public:
  constexpr reverse_vector_iterator() = default;
  constexpr reverse_vector_iterator(T *begin, T *end)
      : _begin(begin), _end(end) {}

 public:
  constexpr bool has_next() const { return _begin != _end; }
  constexpr T &next() { return *(_begin--); }
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
  constexpr auto iter() { return vector_iterator<T>(_data, _size); }
  constexpr auto iter() const { return vector_iterator<const T>(_data, _size); }

  constexpr auto riter() {
    return reverse_vector_iterator<T>(_data + _size - 1, _data - 1);
  }

  constexpr auto riter() const {
    return reverse_vector_iterator<const T>(_data + _size - 1, _data - 1);
  }

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

template <typename T>
class vector {
 private:
  size_t _size = 0;
  size_t _max = 0;
  T *_data = nullptr;

 public:
  constexpr ~vector() = default;
  constexpr vector() = default;

  constexpr explicit vector(size_t max)
      : _size{0}, _max{max}, _data{new T[_max + 1]} {}

  // move
  constexpr vector(vector &&o) = default;
  constexpr vector &operator=(vector &&o) = default;

  // copy
  constexpr vector(const vector &o) = default;
  constexpr vector &operator=(const vector &) = default;

 public:
  constexpr auto iter() { return vector_iterator<T>(_data, _size); }

  constexpr auto iter() const { return vector_iterator<const T>(_data, _size); }

  constexpr auto riter() {
    return reverse_vector_iterator<T>(_data + _size - 1, _data - 1);
  }

  constexpr auto riter() const {
    return reverse_vector_iterator<const T>(_data + _size - 1, _data - 1);
  }

  constexpr T &operator[](size_t i) {
    assert(i < _size);
    return _data[i];
  }

  constexpr const T &operator[](size_t i) const {
    assert(i < _size);
    return _data[i];
  }

 public:
  constexpr bool empty() const { return _size == 0; }
  constexpr bool full() const { return _size == _max; }
  constexpr auto size() const { return _size; }

 public:
  constexpr void push(const T &t) {
    if (!full()) {
      _data[_size] = t;
      _size += 1;
    }
  }

  constexpr void push(T &&t) {
    if (!full()) {
      _data[_size] = transfer(t);
      _size += 1;
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

template <typename T>
class ext_vector {
 private:
  vector<T> _data;

 public:
  constexpr ~ext_vector() = default;
  constexpr ext_vector() = default;
  constexpr explicit ext_vector(size_t max) : _data(max) {}

  // move
  constexpr ext_vector(ext_vector &&) = default;
  constexpr ext_vector &operator=(ext_vector &&) = default;

  // copy
  constexpr ext_vector(const ext_vector &) = default;
  constexpr ext_vector &operator=(const ext_vector &) = default;

 public:
  constexpr auto iter() { return _data.iter(); }
  constexpr auto iter() const { return _data.iter(); }
  constexpr auto riter() { return _data.riter(); }
  constexpr auto riter() const { return _data.riter(); }

  constexpr T &operator[](size_t i) {
    assert(i < size());
    return _data[i];
  }

  constexpr const T &operator[](size_t i) const {
    assert(i < size());
    return _data[i];
  }

 public:
  constexpr auto size() const { return _data.size(); }
  constexpr auto empty() const { return _data.empty(); }
  constexpr auto full() const { return _data.full(); }

 public:
  constexpr void push(const T &t) {
    if (_data.full()) {
      vector<T> tmp = transfer(_data);
      _data = vector<T>(tmp.size() * 2 + 10);

      auto itmp = tmp.iter();

      while (itmp.has_next()) {
        _data.push(transfer(itmp.next()));
      }
    }

    _data.push(t);
  }

  constexpr void push(T &&t) {
    if (_data.full()) {
      vector<T> tmp = transfer(_data);
      _data = vector<T>(tmp.size() * 2 + 10);

      auto itmp = tmp.iter();

      while (itmp.has_next()) {
        _data.push(transfer(itmp.next()));
      }
    }

    _data.push(transfer(t));
  }

  constexpr auto pop() { return _data.pop(); }

  constexpr auto clear() { return _data.clear(); }
};

}  // namespace n

#endif
