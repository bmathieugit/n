#ifndef __n_vector_hpp__
#define __n_vector_hpp__
#include <stdio.h>

#include <n/iterator.hpp>
#include <n/result.hpp>
#include <n/utils.hpp>

namespace n {

template <typename T>
class vector;

template <typename T>
class vector_oterator {
 private:
  vector<T>& _v;

 public:
  constexpr vector_oterator(vector<T>& v) : _v(v) {}

 public:
  constexpr void sext(const T& t) { _v.push(t); }
  constexpr void sext(T&& t) { _v.push(move(t)); }
};

template <typename T>
class vector {
 private:
  T* _data = nullptr;
  size_t _max = 0;
  size_t _len = 0;

 public:
  constexpr ~vector() {
    delete[] _data;
    _data = nullptr;
    _max = 0;
    _len = 0;
  }

  constexpr vector() = default;

  constexpr vector(size_t max)
      : _data(new T[max == 0 ? 10 : max]), _max(max == 0 ? 10 : max), _len(0) {}

  constexpr vector(const vector& o) : vector(o.len()) {
    copy<T>(o.iter(), oter());
  }

  constexpr vector(vector&& o) : _data(o._data), _max(o._max), _len(o._len) {
    o._data = nullptr;
    o._max = 0;
    o._len = 0;
  }

  constexpr vector& operator=(const vector& o) {
    if (this != &o) {
      delete[] _data;

      if (o._len != 0) _data = new T[o._len];
      copy<T>(o.iter(), oter());
    }

    return *this;
  }

  constexpr vector& operator=(vector&& o) {
    if (this != &o) {
      auto td = _data;
      auto tm = _max;
      auto tl = _len;

      _data = o._data;
      _max = o._max;
      _len = o._len;

      o._data = td;
      o._max = tm;
      o._len = tl;
    }

    return *this;
  }

 public:
  constexpr auto iter() const { return pointer_iterator<const T>(_data, _len); }

  constexpr auto oter() { return vector_oterator<T>(*this); }

 public:
  constexpr auto len() const { return _len; }
  constexpr auto empty() const { return _len == 0; }
  constexpr auto max() const { return _max; }
  constexpr auto full() const { return _len == _max; }

 public:
  constexpr void push(const T& t) {
    if (full()) {
      auto tmp = new T[_max * 2 + 10];
      pointer_iterator<T> idata(_data, _data + _len);
      pointer_oterator<T> otmp(tmp, tmp + _len);
      transfer<T>(idata, otmp);
      delete[] _data;
      _data = tmp;
      _max = _max * 2 + 10;
    }

    _data[_len] = t;
    _len += 1;
  }

  constexpr void push(T&& t) {
    if (full()) {
      auto tmp = new T[_max * 2 + 10];
      pointer_iterator<T> idata(_data, _data + _len);
      pointer_oterator<T> otmp(tmp, tmp + _len);
      transfer<T>(idata, otmp);
      delete[] _data;
      _data = tmp;
      _max = _max * 2 + 10;
    }

    _data[_len] = move(t);
    _len += 1;
  }

  constexpr maybe<T> pop() {
    return not empty() ? maybe<T>(move(_data[--_len])) : maybe<T>();
  }

  constexpr void clear() { _len = 0; }
};

}  // namespace n

#endif
