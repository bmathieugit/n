#ifndef __n_variant_hpp__
#define __n_variant_hpp__

#include <n/utils.hpp>
#include <new>

namespace n {
/**
 * @brief A meta-function that determines the size of the largest type in a
 * list of types.
 *
 * The function uses template recursion to compare the sizes of all types
 * in the list, and returns the size of the largest one.
 *
 * @tparam T0 The first type in the list.
 * @tparam T The rest of the types in the list.
 *
 * @return The size of the largest type in the list.
 */
template <typename T0, typename... T>
constexpr auto largest = sizeof(T0) > largest<T...> ? sizeof(T0)
                                                    : largest<T...>;

/**
 * @brief Base case for the largest meta-function.
 *
 * This function is called when there is only one type in the list.
 *
 * @tparam T0 The type in the list.
 *
 * @return The size of the type.
 */
template <typename T0>
constexpr auto largest<T0> = sizeof(T0);

/**
 * @brief A meta-function that determines the index of a type in a list of
 * types.
 *
 * The function uses template recursion to find the first occurrence of the
 * specified type in the list, and returns its index. If the type is not in the
 * list, the function returns -1.
 *
 * @tparam U The type to find.
 * @tparam T0 The first type in the list.
 * @tparam T The rest of the types in the list.
 *
 * @return The index of the type in the list, or size_t(-1) if the type is not
 * in the list.
 */
template <typename U, typename T0, typename... T>
constexpr size_t indexof =
    same_as<U, T0> ? 0 : (indexof<U, T...> == -1 ? -1 : indexof<U, T...> + 1);

/**
 * @brief Base case for the indexof meta-function.
 *
 * This function is called when there are only two types in the list.
 *
 * @tparam U The type to find.
 * @tparam T0 The other type in the list.
 *
 * @return The index of the type in the list, or size_t(-1) if the type is not
 * in the list.
 */
template <typename U, typename T0>
constexpr size_t indexof<U, T0> = same_as<U, T0> ? 0 : size_t(-1);

/**
 * @brief `variant` class capable of storing an object of any type specified by
 * the variadic template parameter.
 *
 * @tparam T List of possible types that the `variant` class can store.
 */
template <typename... T>
class variant {
 private:
  /**
   * @brief Storage space for the current object of the variant. Its size is
   * determined by the largest type among the possible types.
   */
  char _storage[largest<T...>];

  /**
   * @brief Index of the type currently stored in the variant. If equal to -1,
   * this means that the variant is empty.
   */
  size_t _index = size_t(-1);

 private:
  /**
   * @brief Destroys the object currently stored in the variant.
   *
   * @tparam U0 The first type among the possible types.
   * @tparam U The other possible types.
   */
  template <typename U0, typename... U>
  void destroy() {
    if (_index == indexof<U0, T...>) {
      (*(reinterpret_cast<U0 *>(_storage))).~U0();
    } else if constexpr (sizeof...(U) > 0) {
      destroy<U...>();
    }
  }

  /**
   * @brief Copies the object `u` into the storage of the variant.
   *
   * @tparam U Type of the object to be copied.
   * @param u The object to be copied into the variant.
   */
  template <typename U>
  void copy(const U &u) {
    if (_index != size_t(-1)) {
      destroy<T...>();
    }

    new (_storage) rm_cvref<U>(u);
    _index = indexof<rm_cvref<U>, T...>;
  }

  /**
   * @brief Copies the object from the variant `o` into the current variant.
   *
   * @tparam U0 The first type among the possible types.
   * @tparam U The other possible types.
   * @param o The variant to be copied.
   */
  template <typename U0, typename... U>
  void copy_o(const variant &o) {
    if (_index != size_t(-1)) {
      destroy<T...>();
    }

    auto i = o.index();

    if (i == indexof<U0, T...>) {
      new (_storage) U0(o.get<U0>());
      _index = i;
    } else if constexpr (sizeof...(U) > 0) {
      copy_o<U...>(o);
    }
  }

  /**
   * @brief Moves the object `u` into the storage of the variant.
   *
   * @tparam U Type of the object to be moved.
   * @param u The object to be moved into the variant.
   */
  template <typename U>
  void move(U &&u) {
    if (_index != size_t(-1)) {
      destroy<T...>();
    }

    new (_storage) rm_cvref<U>(transfer(u));
    _index = indexof<rm_cvref<U>, T...>;
  }

  /**
   * @brief Moves the object from the variant `o` into the current variant.
   *
   * @param o The variant to be moved.
   */
  void move_o(variant &&o) {
    if (_index != size_t(-1)) {
      destroy<T...>();
    }

    _index = o.index();

    for (int i = 0; i < largest<T...>; ++i) {
      _storage[i] = o._storage[i];
      o._storage[i] = 0;
    }
  }

 public:
  /**
   * @brief Destructor of the `variant` class.
   * Destroys the object currently stored in the variant, if an object is
   * present.
   */
  constexpr ~variant() {
    if (_index != size_t(-1)) {
      destroy<T...>();
    }
  }

  /**
   * @brief Default constructor of the `variant` class.
   */
  constexpr variant() = default;

  /**
   * @brief Copy constructor of the `variant` class.
   *
   * @param o The variant to be copied.
   */
  constexpr variant(const variant &o) { copy_o<T...>(o); }

  /**
   * @brief Move constructor of the `variant` class.
   *
   * @param o The variant to be moved.
   */
  constexpr variant(variant &&o) { move_o(transfer(o)); }

  /**
   * @brief Copy assignment operator of the `variant` class.
   *
   * @param o The variant to be copied.
   * @return Returns a reference to the current variant.
   */
  constexpr variant &operator=(const variant &o) {
    if (this != &o) {
      copy_o<T...>(o);
    }

    return *this;
  }

  /**
   * @brief Move assignment operator of the `variant` class.
   *
   * @param o The variant to be moved.
   * @return Returns a reference to the current variant.
   */
  constexpr variant &operator=(variant &&o) {
    if (this != &o) {
      move_o(transfer(o));
    }

    return *this;
  }

  /**
   * @brief Returns the index of the type currently stored in the variant.
   *
   * @return Index of the type currently stored in the variant.
   */
  constexpr auto index() const { return _index; }

  /**
   * @brief Indicates whether the variant is empty.
   *
   * @return Returns `true` if the variant is empty, `false` otherwise.
   */
  constexpr auto empty() const { return _index == size_t(-1); }

  /**
   * @brief Copies the object `u` into the variant.
   *
   * @tparam U Type of the object to be copied.
   * @param u The object to be copied into the variant.
   */
  template <typename U>
  constexpr void set(const U &u) {
    copy(u);
  }

  /**
   * @brief Moves the object `u` into the variant.
   *
   * @tparam U Type of the object to be moved.
   * @param u The object to be moved into the variant.
   */
  template <typename U>
  constexpr void set(U &&u) {
    move(transfer(u));
  }

  /**
   * @brief Returns a reference to the object stored in the variant.
   *
   * @tparam U Type of the object to be returned.
   * @return Returns a reference to the object stored in the variant.
   */
  template <typename U>
  constexpr U &get() {
    return *reinterpret_cast<U *>(_storage);
  }

  /**
   * @brief Returns a constant reference to the object stored in the variant.
   *
   * @tparam U Type of the object to be returned.
   * @return Returns a constant reference to the object stored in the variant.
   */
  template <typename U>
  constexpr const U &get() const {
    return *reinterpret_cast<const U *>(_storage);
  }
};
}  // namespace n

#endif
