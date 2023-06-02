#ifndef __n_vector_hpp__
#define __n_vector_hpp__

namespace n {

template <typename T, typename U>
constexpr bool __same_as = false;

template <typename T>
constexpr bool __same_as<T, T> = true;

template <typename T, typename U>
concept same_as = __same_as<T, U>;

template <typename T>
struct __rm_ref {
  using type = T;
};

template <typename T>
struct __rm_ref<T&> {
  using type = T;
};

template <typename T>
struct __rm_ref<T&&> {
  using type = T;
};

template <typename T>
using rm_ref = typename __rm_ref<T>::type;

template <typename T>
constexpr rm_ref<T>&& move(T&& t) {
  return static_cast<rm_ref<T>&&>(t);
}

template <typename T>
constexpr T&& relay(rm_ref<T>& t) noexcept {
  return static_cast<T&&>(t);
}

template <typename T>
constexpr T&& relay(rm_ref<T>&& t) noexcept {
  return static_cast<T&&>(t);
}

template <typename I>
concept __iterator_has_next = requires(I i) {
                                { i.has_next() } -> same_as<bool>;
                              };

template <typename I, typename T>
concept __iterator_next = requires(I i) {
                            { i.next() } -> same_as<const T&>;
                          };

template <typename I, typename T>
concept iterator = __iterator_has_next<I> and __iterator_next<I, T>;

template <typename O, typename T>
concept oterator = requires(O o, T t) { o.sext(t); };

template <typename T, iterator<T> I, oterator<T> O>
constexpr void copy(I i, O o) {
  while (i.has_next()) {
    o.sext(i.next());
  }
}

template <typename T, iterator<T> I, oterator<T> O>
constexpr void transfer(I i, O o) {
  while (i.has_next()) {
    o.sext(move(i.next()));
  }
}

}  // namespace n

namespace n {

using size_t = unsigned long long;

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
class vector_iterator {
 private:
  mutable const T* _b = nullptr;
  const T* _e = nullptr;

 public:
  constexpr vector_iterator(const T* b, const T* e) : _b(b), _e(e) {}

 public:
  constexpr bool has_next() const { return _b != _e; }
  constexpr const T& next() const { return *(_b++); }
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

  constexpr vector(size_t max) : _data(new T[max]), _max(max), _len(0) {}

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

      _data = new T[o._len];
      copy<T>(o.iter(), oter());
    }

    return *this;
  }

  constexpr vector& operator=(vector&& o) {
    if (this != &o) {
      delete[] _data;

      _data = o._data;
      _max = o._max;
      _len = o._len;
      o._data = nullptr;
      o._max = 0;
      o._len = 0;
    }

    return *this;
  }

 public:
  constexpr auto iter() const {
    return vector_iterator<T>(_data, _data + _len);
  }

  constexpr auto oter() { return vector_oterator<T>(*this); }

 public:
  constexpr auto len() const { return _len; }
  constexpr auto empty() const { return _len == 0; }
  constexpr auto max() const { return _max; }
  constexpr auto full() const { return _len == _max; }

 public:
  constexpr void push(const T& t) {
    if (_data == nullptr) {
      _data = new T[10];
    } else if (full()) {
      auto tmp = _data;
      _data = new T[_max * 2];

      for (size_t i = 0; i < _len; ++i) {
        _data[i] = move(tmp[i]);
      }

      delete[] tmp;
    }

    _data[_len++] = t;
  }

  constexpr void push(T&& t) {
    if (_data == nullptr) {
      _data = new T[10];
    } else if (full()) {
      auto tmp = _data;
      _data = new T[_max * 2];

      for (size_t i = 0; i < _len; ++i) {
        _data[i] = move(tmp[i]);
      }

      delete[] tmp;
    }

    _data[_len++] = move(t);
  }

  constexpr T&& pop() {
    return move(_data[--_len]);
  }

  constexpr void clear() {
    _len = 0;
  }
};

}  // namespace n

#endif
