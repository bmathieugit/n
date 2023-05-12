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

/**
 * @brief A non-owning view into a sequence of elements.
 *
 * This class provides a lightweight, non-owning reference to a sequence of
 * elements. It is similar in concept to a non-owning pointer or a C++17
 * std::string_view. It does not own the elements it refers to, so it does not
 * manage their lifecycle. The referenced sequence must outlive this vector_view
 * object.
 *
 * @tparam T The type of elements the vector_view refers to.
 */
template <typename T>
class vector_view {
 private:
  const T *_begin = nullptr;
  const T *_end = nullptr;

 public:
  /**
   * @brief Default destructor.
   */
  constexpr ~vector_view() = default;

  /**
   * @brief Default constructor.
   */
  constexpr vector_view() = default;

  /**
   * @brief Construct a view given begin and end pointers.
   *
   * @param begin Pointer to the first element of the sequence.
   * @param end Pointer to one past the last element of the sequence.
   */
  constexpr vector_view(const T *begin, const T *end)
      : _begin(begin), _end(end) {}

  /**
   * @brief Construct a view given begin pointer and length.
   *
   * @param begin Pointer to the first element of the sequence.
   * @param len The number of elements in the sequence.
   */
  constexpr vector_view(const T *begin, size_t len)
      : _begin(begin), _end(begin + len) {}

  /**
   * @brief Default move constructor.
   */
  constexpr vector_view(vector_view &&) = default;

  /**
   * @brief Default move assignment operator.
   */
  constexpr vector_view &operator=(vector_view &&) = default;

  /**
   * @brief Default copy constructor.
   */
  constexpr vector_view(const vector_view &) = default;

  /**
   * @brief Default copy assignment operator.
   */
  constexpr vector_view &operator=(const vector_view &) = default;

 public:
  /**
   * @brief Get an iterator to the sequence.
   *
   * @return A forward iterator to the sequence.
   */
  constexpr auto iter() const { return vector_iterator<const T>(_begin, _end); }

  /**
   * @brief Get a reverse iterator to the sequence.
   *
   * @return A reverse iterator to the sequence.
   */
  constexpr auto riter() const {
    return reverse_vector_iterator<const T>(_begin, _end);
  }

 public:
  /**
   * @brief Get the size of the sequence.
   *
   * @return The number of elements in the sequence.
   */
  constexpr auto size() const { return _end - _begin; }

  /**
   * @brief Check if the sequence is empty.
   *
   * @return True if the sequence is empty, false otherwise.
   */
  constexpr auto empty() const { return size() == 0; }
};
/**
 * @brief A compile-time-fixed-size vector that stores its elements in static storage.
 *
 * This class is a simple fixed-size array with a stack discipline. It maintains
 * an array of elements of type T in static storage, and provides methods to
 * push and pop elements from the array. The capacity of the array is determined
 * by the template parameter N.
 *
 * @tparam T The type of elements the static_vector holds.
 * @tparam N The maximum number of elements the static_vector can hold.
 */
template <typename T, size_t N>
class static_vector {
 private:
  T _data[N];
  size_t _size = 0;

 public:
  /**
   * @brief Default destructor.
   */
  constexpr ~static_vector() = default;

  /**
   * @brief Default constructor.
   */
  constexpr static_vector() = default;

  /**
   * @brief Default move constructor.
   */
  constexpr static_vector(static_vector &&) = default;

  /**
   * @brief Default move assignment operator.
   */
  constexpr static_vector &operator=(static_vector &&) = default;

  /**
   * @brief Default copy constructor.
   */
  constexpr static_vector(const static_vector &) = default;

  /**
   * @brief Default copy assignment operator.
   */
  constexpr static_vector &operator=(const static_vector &) = default;

  /**
   * @brief Construct a static_vector from an iterator.
   *
   * This constructor will insert elements from the provided iterator into
   * the static_vector until it is full.
   *
   * @param i An iterator pointing to a sequence of elements.
   */
  constexpr static_vector(iterator auto i) {
    while (i.has_next() && !full()) {
      push(i.next());
    }
  }

 public:
  /**
   * @brief Get the maximum capacity of the static_vector.
   *
   * @return The maximum number of elements the static_vector can hold.
   */
  constexpr auto max() const { return N; }

  /**
   * @brief Get the current size of the static_vector.
   *
   * @return The number of elements currently in the static_vector.
   */
  constexpr auto size() const { return _size; }

  /**
   * @brief Check if the static_vector is empty.
   *
   * @return True if the static_vector is empty, false otherwise.
   */
  constexpr auto empty() const { return _size == 0; }

  /**
   * @brief Check if the static_vector is full.
   *
   * @return True if the static_vector is full, false otherwise.
   */
  constexpr auto full() const { return _size == N; }

 public:
  /**
   * @brief Get an iterator to the elements.
   *
   * @return A forward iterator to the elements.
   */
  constexpr auto iter() { return vector_iterator<T>(_data, _data + _size); }

  /**
   * @brief Get a const iterator to the elements.
   *
   * @return A const forward iterator to the elements.
   */
  constexpr auto iter() const {
    return vector_iterator<const T>(_data, _data + _size);
  }

  /**
   * @brief Get a reverse iterator to the elements.
   *
   * @return A reverse iterator to the elements.
   */
  constexpr auto riter() {
    return reverse_vector_iterator<T>(_data, _data + _size);
  }

  /**
   * @brief Get a const reverse iterator to the elements.
   *
   * @return A const reverse iterator to the elements.
   */
  constexpr auto riter() const {
    return reverse_vector_iterator<const T>(_data, _data + _size);
  }

  /**
   * @brief Access element at specific position.
   *
   * @param i Position of the element to return.
   * @return Reference to the requested element.
   */
  constexpr T &operator[](size_t i) {
    assert(i < _size);
    return _data[i];
  }

  /**
   * @brief Access element at specific position (const version).
   *
   * @param i Position of the element to return.
   * @return Const reference to the requested element.
   */
  constexpr const T &operator[](size_t i) const {
    assert(i < _size);
    return _data[i];
  }

 public:
  /**
   * @brief Push an element into the static_vector.
   *
   * @param t The element to be pushed.
   */
  constexpr void push(const T &t) {
    if (!full()) {
      _data[_size++] = t;
    }
  }

  /**
   * @brief Push an element into the static_vector (rvalue version).
   *
   * @param t The element to be pushed.
   */
  constexpr void push(T &&t) {
    if (!full()) {
      _data[_size++] = transfer(t);
    }
  }

  /**
   * @brief Pop an element from the static_vector.
   *
   * @return A maybe<T> object containing the popped element, or an empty
   * maybe<T> if the static_vector was empty.
   */
  constexpr maybe<T> pop() {
    if (_size != 0) {
      return maybe<T>(transfer(_data[--_size]));
    } else {
      return maybe<T>();
    }
  }

  /**
   * @brief Clear the static_vector.
   *
   * This does not actually delete the elements in the array, it just resets
   * the size of the static_vector to 0.
   */
  constexpr void clear() { _size = 0; }
};

/**
 * @brief A dynamic array class with fixed runtime size.
 *
 * This class provides a dynamic array implementation with basic operations
 * such as insertion, deletion, and retrieval.
 *
 * @tparam T The type of elements stored in the vector.
 */
template <typename T>
class vector {
 private:
  size_t _size = 0;   /**< The current size of the vector. */
  size_t _max = 0;    /**< The maximum capacity of the vector. */
  T *_data = nullptr; /**< The pointer to the underlying data. */

 public:
  /**
   * @brief Destructor.
   *
   * Cleans up the dynamically allocated memory.
   */
  constexpr ~vector() { delete[] transfer(_data); }

  /**
   * @brief Default constructor.
   *
   * Constructs an empty vector.
   */
  constexpr vector() = default;

  /**
   * @brief Constructor with a specified maximum capacity.
   *
   * Constructs a vector with a maximum capacity.
   *
   * @param max The maximum capacity of the vector.
   */
  constexpr explicit vector(size_t max)
      : _size{0}, _max{max}, _data{new T[_max]} {}

  /**
   * @brief Move constructor.
   *
   * Constructs a vector by moving the contents from another vector.
   *
   * @param o The vector to be moved.
   */
  constexpr vector(vector &&o) = default;

  /**
   * @brief Move assignment operator.
   *
   * Moves the contents from another vector to the current vector.
   *
   * @param o The vector to be moved.
   * @return A reference to the current vector.
   */
  constexpr vector &operator=(vector &&o) = default;

  /**
   * @brief Copy constructor.
   *
   * Constructs a vector by copying the contents from another vector.
   *
   * @param o The vector to be copied.
   */
  constexpr vector(const vector &o) : vector(o._max) {
    auto io = o.iter();

    while (io.has_next()) {
      push(io.next());
    }
  }

  /**
   * @brief Copy assignment operator.
   *
   * Copies the contents from another vector to the current vector.
   *
   * @param o The vector to be copied.
   * @return A reference to the current vector.
   */
  constexpr vector &operator=(const vector &o) {
    if (this != &o) {
      delete[] transfer(_data);
      *this = transfer(vector(o));
    }

    return *this;
  }

 public:
  /**
   * @brief Returns an iterator to the beginning of the vector.
   *
   * @return An iterator to the beginning of the vector.
   */
  constexpr auto iter() { return vector_iterator<T>(_data, _data + _size); }

  /**
   * @brief Returns a constant iterator to the beginning of the vector.
   *
   * @return A constant iterator to the beginning of the vector.
   */
  constexpr auto iter() const {
    return vector_iterator<const T>(_data, _data + _size);
  }

  /**
   * @brief Returns a reverse iterator to the beginning of the vector.
   *
   * @return A reverse iterator to the beginning of the vector.
   */
  constexpr auto riter() {
    return reverse_vector_iterator<T>(_data, _data + _size);
  }

  /**
   * @brief Returns a constant reverse iterator to the beginning of the vector.
   *
   * @return A constant reverse iterator to the beginning of the vector.
   */
  constexpr auto riter() const {
    return reverse_vector_iterator<const T>(_data, _data + _size);
  }

  /**
   * @brief Accesses the element at the specified index.
   *
   * @param i The index of the element.
   * @return A reference to the element at the specified index.
   */
  constexpr T &operator[](size_t i) {
    assert(i < _size);
    return _data[i];
  }

  /**
   * @brief Accesses the element at the specified index (constant version).
   *
   * @param i The index of the element.
   * @return A constant reference to the element at the specified index.
   */
  constexpr const T &operator[](size_t i) const {
    assert(i < _size);
    return _data[i];
  }

 public:
  /**
   * @brief Returns the maximum capacity of the vector.
   *
   * @return The maximum capacity of the vector.
   */
  constexpr auto max() const { return _max; }

  /**
   * @brief Checks if the vector is empty.
   *
   * @return True if the vector is empty, false otherwise.
   */
  constexpr bool empty() const { return _size == 0; }

  /**
   * @brief Checks if the vector is full.
   *
   * @return True if the vector is full, false otherwise.
   */
  constexpr bool full() const { return _size == _max; }

  /**
   * @brief Returns the current size of the vector.
   *
   * @return The current size of the vector.
   */
  constexpr auto size() const { return _size; }

 public:
  /**
   * @brief Inserts an element at the end of the vector.
   *
   * @param t The element to be inserted.
   */
  constexpr void push(const T &t) {
    if (!full()) {
      _data[_size++] = t;
    }
  }

  /**
   * @brief Moves an element to the end of the vector.
   *
   * @param t The element to be moved.
   */
  constexpr void push(T &&t) {
    if (!full()) {
      _data[_size++] = transfer(t);
    }
  }

  /**
   * @brief Removes and returns the last element of the vector.
   *
   * @return An optional containing the last element of the vector if it is not
   * empty, or an empty optional if the vector is empty.
   */
  constexpr maybe<T> pop() {
    if (_size != 0) {
      return maybe<T>(transfer(_data[--_size]));
    } else {
      return maybe<T>();
    }
  }

  /**
   * @brief Clears the vector.
   *
   * Removes all elements from the vector, resetting its size to zero.
   */
  constexpr void clear() { _size = 0; }
};

/**
 * @brief An extendable vector class.
 *
 * This class extends the functionality of the vector class by adding features
 * such as automatic resizing and transfer operations.
 *
 * @tparam T The type of elements stored in the extended vector.
 */
template <typename T>
class ext_vector {
 private:
  vector<T> _data; /**< The underlying vector to store the elements. */

 public:
  /**
   * @brief Destructor.
   *
   * Cleans up the dynamically allocated memory.
   */
  constexpr ~ext_vector() = default;

  /**
   * @brief Default constructor.
   *
   * Constructs an empty extended vector.
   */
  constexpr ext_vector() = default;

  /**
   * @brief Constructor with a specified maximum capacity.
   *
   * Constructs an extended vector with a specified maximum capacity.
   *
   * @param max The maximum capacity of the extended vector.
   */
  constexpr explicit ext_vector(size_t max) : _data(max) {}

  /**
   * @brief Move constructor.
   *
   * Constructs an extended vector by moving the contents from another extended
   * vector.
   *
   * @param o The extended vector to be moved.
   */
  constexpr ext_vector(ext_vector &&) = default;

  /**
   * @brief Move assignment operator.
   *
   * Moves the contents from another extended vector to the current extended
   * vector.
   *
   * @param o The extended vector to be moved.
   * @return A reference to the current extended vector.
   */
  constexpr ext_vector &operator=(ext_vector &&) = default;

  /**
   * @brief Copy constructor.
   *
   * Constructs an extended vector by copying the contents from another extended
   * vector.
   *
   * @param o The extended vector to be copied.
   */
  constexpr ext_vector(const ext_vector &) = default;

  /**
   * @brief Copy assignment operator.
   *
   * Copies the contents from another extended vector to the current extended
   * vector.
   *
   * @param o The extended vector to be copied.
   * @return A reference to the current extended vector.
   */
  constexpr ext_vector &operator=(const ext_vector &) = default;

 public:
  /**
   * @brief Returns an iterator to the beginning of the extended vector.
   *
   * @return An iterator to the beginning of the extended vector.
   */
  constexpr auto iter() { return _data.iter(); }

  /**
   * @brief Returns a constant iterator to the beginning of the extended vector.
   *
   * @return A constant iterator to the beginning of the extended vector.
   */
  constexpr auto iter() const { return _data.iter(); }

  /**
   * @brief Returns a reverse iterator to the beginning of the extended vector.
   *
   * @return A reverse iterator to the beginning of the extended vector.
   */
  constexpr auto riter() { return _data.riter(); }

  /**
   * @brief Returns a constant reverse iterator to the beginning of the extended
   * vector.
   *
   * @return A constant reverse iterator to the beginning of the extended
   * vector.
   */
  constexpr auto riter() const { return _data.riter(); }

  /**
   * @brief Accesses the element at the specified index.
   *
   * @param i The index of the element.
   * @return A reference to the element at the specified index.
   */
  constexpr T &operator[](size_t i) {
    assert(i < size());
    return _data[i];
  }

  /**
   * @brief Accesses the element at the specified index (constant version).
   *
   * @param i The index of the element.
   * @return A constant reference to the element at the specified index.
   */
  constexpr const T &operator[](size_t i) const {
    assert(i < size());
    return _data[i];
  }

 public:
  /**
   * @brief Returns the maximum capacity of the extended vector.
   *
   * @return The maximum capacity of the extended vector.
   */
  constexpr auto max() const { return _data.max(); }

  /**
   * @brief Returns the current size of the extended vector.
   *
   * @return The current size of the extended vector.
   */
  constexpr auto size() const { return _data.size(); }

  /**
   * @brief Checks if the extended vector is empty.
   *
   * @return True if the extended vector is empty, false otherwise.
   */
  constexpr auto empty() const { return _data.empty(); }

  /**
   * @brief Checks if the extended vector is full.
   *
   * @return True if the extended vector is full, false otherwise.
   */
  constexpr auto full() const { return _data.full(); }

 public:
  /**
   * @brief Inserts an element at the end of the extended vector.
   *
   * If the extended vector is full, it automatically resizes itself before
   * inserting the element.
   *
   * @param t The element to be inserted.
   */
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

  /**
   * @brief Moves an element to the end of the extended vector.
   *
   * If the extended vector is full, it automatically resizes itself before
   * inserting the element.
   *
   * @param t The element to be moved.
   */
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

  /**
   * @brief Removes and returns the last element of the extended vector.
   *
   * @return An optional containing the last element of the extended vector if
   * it is not empty, or an empty optional if the extended vector is empty.
   */
  constexpr auto pop() { return _data.pop(); }

  /**
   * @brief Clears the extended vector.
   *
   * Removes all elements from the extended vector, resetting its size to zero.
   */
  constexpr void clear() { _data.clear(); }
};
}  // namespace n

#endif
