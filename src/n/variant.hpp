#ifndef __n_variant_hpp__
#define __n_variant_hpp__

#include <n/utils.hpp>
#include <new>

namespace n
{
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
  constexpr auto largest = sizeof(T0) > largest<T...> ? sizeof(T0) : largest<T...>;

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
   * @brief A meta-function that determines the index of a type in a list of types.
   *
   * The function uses template recursion to find the first occurrence of the
   * specified type in the list, and returns its index. If the type is not in the
   * list, the function returns -1.
   *
   * @tparam U The type to find.
   * @tparam T0 The first type in the list.
   * @tparam T The rest of the types in the list.
   *
   * @return The index of the type in the list, or size_t(-1) if the type is not in the list.
   */
  template <typename U, typename T0, typename... T>
  constexpr size_t indexof = same_as<U, T0> ? 0 : (indexof<U, T...> == -1 ? -1 : indexof<U, T...> + 1);

  /**
   * @brief Base case for the indexof meta-function.
   *
   * This function is called when there are only two types in the list.
   *
   * @tparam U The type to find.
   * @tparam T0 The other type in the list.
   *
   * @return The index of the type in the list, or size_t(-1) if the type is not in the list.
   */
  template <typename U, typename T0>
  constexpr size_t indexof<U, T0> = same_as<U, T0> ? 0 : size_t(-1);

  template <typename... T>
  class alignas(T...) variant
  {
  private:
    char _storage[largest<T...>];
    size_t _index = size_t(-1);

  private:
    template <typename U0, typename... U>
    void destroy()
    {
      if (_index == indexof<U0, T...>)
      {
        (*(reinterpret_cast<U0 *>(_storage))).~U0();
      }
      else if constexpr (sizeof...(U) > 0)
      {
        destroy<U...>();
      }
    }

    template <typename U>
    void copy(const U &u)
    {
      if (_index != size_t(-1))
      {
        destroy<T...>();
      }

      new (_storage) rm_cvref<U>(u);
    }

    template <typename U0, typename... U>
    void copy_o(const variant &o)
    {
      if (_index != size_t(-1))
      {
        destroy<T...>();
      }

      auto i = o.index();

      if (i == indexof<U0, T...>)
      {
        new (_storage) U0(o.get<U0>());
      }
      else if constexpr (sizeof...(U) > 0)
      {
        copy_o<U...>(o);
      }
    }

    template <typename U>
    void move(U &&u)
    {
      if (_index != size_t(-1))
      {
        destroy<T...>();
      }

      new (_storage) rm_cvref<U>(transfer(u));
    }

    template <typename... U>
    void move_o(variant &&o)
    {
      if (_index != size_t(-1))
      {
        destroy<T...>();
      }

      auto i = o.index();

      for (int i = 0; i < largest<T...>; ++i)
      {
        _storage[i] = o._storage[i];
        o._storage[i] = 0;
      }
    }

  public:
    constexpr ~variant()
    {
      if (_index != size_t(-1))
      {
        destroy<T...>();
      }
    }
    constexpr variant() = default;

    constexpr variant(const variant &o)
    {
      copy_o<T...>(o);
      _index = o.index();
    }

    constexpr variant(variant &&o)
    {
      move_o<T...>(transfer(o));
      _index = o.index();
    }

    constexpr variant &operator=(const variant &o)
    {
      if (this != &o)
      {
        copy_o<T...>(o);
      } 

      return *this;
    }

    constexpr variant &operator=(variant &&o)
    {
      if (this != &o)
      {
        move_o<T...>(transfer(o));
      }

      return *this;
    }

  public:
    constexpr auto index() const { return _index; }
    constexpr auto empty() const { return _index == size_t(-1); }

  public:
    template<typename U>
    constexpr void set(const U& u){
      copy(u);
      _index = indexof<rm_cvref<U>, T...>;
    }

    template<typename U>
    constexpr void set(U&& u){
      move(transfer(u));
      _index = indexof<rm_cvref<U>, T...>;
    }
    template <typename U>
    constexpr U &get()
    {
      return *reinterpret_cast<U *>(_storage);
    }

    template <typename U>
    constexpr const U &get() const
    {
      return *reinterpret_cast<const U *>(_storage);
    }
  };
}

#endif