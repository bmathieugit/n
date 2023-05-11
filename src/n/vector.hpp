#ifndef __n_vector_hpp__
#define __n_vector_hpp__

#include <assert.h>

#include <n/maybe.hpp>
#include <n/utils.hpp>

namespace n {
/**
 * @brief An iterator over a range of elements stored contiguously in memory.
 *
 * @tparam T The type of the elements stored in the range.
 */
template <typename T>
class vector_iterator {
 private:
  T *_begin = nullptr; /**< A pointer to the beginning of the range. */
  T *_end = nullptr;   /**< A pointer to the end of the range. */

 public:
  /**
   * @brief Construct an empty iterator.
   *
   * This constructor creates an iterator with no associated range.
   */
  constexpr vector_iterator() = default;

  /**
   * @brief Construct an iterator for a given range.
   *
   * @param begin A pointer to the beginning of the range.
   * @param end A pointer to the end of the range.
   *
   * This constructor creates an iterator that is associated with the range of
   * elements in memory beginning at the address pointed to by `begin` and
   * ending at the address pointed to by `end`.
   *
   * @note The behavior is undefined if `begin` is greater than `end`.
   */
  constexpr vector_iterator(T *begin, T *end) : _begin(begin), _end(end) {}

 public:
  /**
   * @brief Check whether there are more elements in the range.
   *
   * @return `true` if there are more elements in the range, `false` otherwise.
   *
   * This function returns `true` if there are more elements in the range that
   * can be accessed using the `next()` function, and `false` otherwise.
   */
  constexpr bool has_next() const { return _begin != _end; }

  /**
   * @brief Get the next element in the range.
   *
   * @return A reference to the next element in the range.
   *
   * This function returns a reference to the next element in the range, and
   * advances the internal pointer to the next element in the range.
   *
   * @note The behavior is undefined if there are no more elements in the range.
   */
  constexpr T &next() { return *(_begin++); }
};

/**
 * @brief An iterator over a range of elements stored contiguously in memory, in
 * reverse order.
 *
 * @tparam T The type of the elements stored in the range.
 */
template <typename T>
class reverse_vector_iterator {
 private:
  T *_begin = nullptr; /**< A pointer to the beginning of the range. */
  T *_end = nullptr;   /**< A pointer to the end of the range. */

 public:
  /**
   * @brief Construct an empty iterator.
   *
   * This constructor creates an iterator with no associated range.
   */
  constexpr reverse_vector_iterator() = default;

  /**
   * @brief Construct an iterator for a given range.
   *
   * @param begin A pointer to the beginning of the range.
   * @param end A pointer to the end of the range.
   *
   * This constructor creates an iterator that is associated with the range of
   * elements in memory beginning at the address pointed to by `begin` and
   * ending at the address pointed to by `end`, but iterates over the range in
   * reverse order.
   *
   * @note The behavior is undefined if `begin` is greater than `end`.
   */
  constexpr reverse_vector_iterator(T *begin, T *end)
      : _begin(begin == nullptr ? nullptr : begin - 1),
        _end(end == nullptr ? nullptr : end - 1) {}

 public:
  /**
   * @brief Check whether there are more elements in the range.
   *
   * @return `true` if there are more elements in the range, `false` otherwise.
   *
   * This function returns `true` if there are more elements in the range that
   * can be accessed using the `next()` function, and `false` otherwise.
   */
  constexpr bool has_next() const { return _begin != _end; }

  /**
   * @brief Get the next element in the range, in reverse order.
   *
   * @return A reference to the next element in the range, in reverse order.
   *
   * This function returns a reference to the next element in the range, in
   * reverse order, and advances the internal pointer to the next element in the
   * range.
   *
   * @note The behavior is undefined if there are no more elements in the range.
   */
  constexpr T &next() { return *(_end--); }
};

template <typename T>
class vector_view {
 private:
  const T *_begin = nullptr;
  const T *_end = nullptr;

 public:
  constexpr ~vector_view() = default;
  constexpr vector_view() = default;
  constexpr vector_view(const T *begin, const T *end)
      : _begin(begin), _end(end) {}
  constexpr vector_view(const T *begin, size_t len)
      : _begin(begin), _end(begin + len) {}

  // move
  constexpr vector_view(vector_view &&) = default;
  constexpr vector_view &operator=(vector_view &&) = default;

  // copy
  constexpr vector_view(const vector_view &) = default;
  constexpr vector_view &operator=(const vector_view &) = default;

 public:
  constexpr auto iter() const { return vector_iterator<const T>(_begin, _end); }
  constexpr auto riter() const {
    return reverse_vector_iterator<const T>(_begin, _end);
  }

 public:
  constexpr auto size() const { return _end - _begin; }
  constexpr auto empty() const { return size() == 0; }
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

  // iterator
  constexpr static_vector(iterator auto i) {
    while (i.has_next() && !full()) {
      push(i.next());
    }
  }

 public:
  constexpr auto max() const { return N; }
  constexpr auto size() const { return _size; }
  constexpr auto empty() const { return _size == 0; }
  constexpr auto full() const { return _size == N; }

 public:
  constexpr auto iter() { return vector_iterator<T>(_data, _data + _size); }
  constexpr auto iter() const {
    return vector_iterator<const T>(_data, _data + _size);
  }

  constexpr auto riter() {
    return reverse_vector_iterator<T>(_data, _data + _size);
  }

  constexpr auto riter() const {
    return reverse_vector_iterator<const T>(_data, _data + _size);
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
      return maybe<T>(transfer(_data[--_size]));
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

  // iterator
  constexpr vector(iterator auto i) {
    while (i.has_next() && !full()) push(i.next());
  }

 public:
  constexpr auto iter() { return vector_iterator<T>(_data, _data + _size); }

  constexpr auto iter() const {
    return vector_iterator<const T>(_data, _data + _size);
  }

  constexpr auto riter() {
    return reverse_vector_iterator<T>(_data, _data + _size);
  }

  constexpr auto riter() const {
    return reverse_vector_iterator<const T>(_data, _data + _size);
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
  constexpr auto max() const { return _max; }
  constexpr bool empty() const { return _size == 0; }
  constexpr bool full() const { return _size == _max; }
  constexpr auto size() const { return _size; }

 public:
  constexpr void push(const T &t) {
    if (!full()) {
      _data[_size++] = t;
      _size += 1;
    }
  }

  constexpr void push(T &&t) {
    if (!full()) {
      _data[_size++] = transfer(t);
    }
  }

  constexpr maybe<T> pop() {
    if (_size != 0) {
      return maybe<T>(transfer(_data[--_size]));
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

  // iterator
  constexpr ext_vector(iterator auto i) {
    while (i.has_next() && !full()) push(i.next());
  }

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
  constexpr auto max() const { return _data.max(); }
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
