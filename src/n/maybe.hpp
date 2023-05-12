#ifndef __n_maybe_hpp__
#define __n_maybe_hpp__

#include <n/utils.hpp>

namespace n {
/**
 * @brief Empty structure used within the union of the maybe class.
 */
struct maybe_empty {};

/**
 * @brief Template class representing a value that may or may not be present.
 *
 * @tparam T The type of the value potentially contained within the object.
 */
template <typename T>
class maybe {
 private:
  /**
   * @brief Flag indicating whether the value is present.
   */
  bool _has = false;

  /**
   * @brief Union holding either an instance of T or an instance of maybe_empty.
   */
  union {
    T _t;
    maybe_empty _e;
  };

 public:
  /**
   * @brief Destructor. Destroys the instance of T if it's present.
   */
  constexpr ~maybe() {
    if (_has) {
      _t.~T();
    }
  }

  /**
   * @brief Default constructor. Does not contain a value.
   */
  constexpr maybe() : _has(false), _e() {}

  /**
   * @brief Constructor taking a constant reference to T.
   * Initializes _t with the given value and sets _has to true.
   *
   * @param t The value to be held by the maybe object.
   */
  constexpr maybe(const T &t) : _has(true), _t(t) {}

  /**
   * @brief Move constructor taking a rvalue reference to T.
   * Initializes _t by moving the given value and sets _has to true.
   *
   * @param t The value to be moved into the maybe object.
   */
  constexpr maybe(T &&t) : _has(true), _t(transfer(t)) {}

  /**
   * @brief Copy constructor.
   *
   * @param o The maybe object to be copied.
   */
  constexpr maybe(const maybe &o) : _has(o._has) {
    if (_has) {
      _t = o._t;
    }
  }

  /**
   * @brief Move constructor.
   *
   * @param o The maybe object to be moved.
   */
  constexpr maybe(maybe &&o) : _has(transfer(o._has)) {
    if (_has) {
      _t = transfer(o._t);
    }
  }

  /**
   * @brief Copy assignment operator.
   * Assigns the given value to _t and sets _has to true.
   *
   * @param t The value to be assigned.
   * @return Reference to the current object.
   */
  constexpr maybe &operator=(const T &t) {
    _has = true;
    _t = t;
    return *this;
  }

  /**
   * @brief Move assignment operator.
   * Moves the given value to _t and sets _has to true.
   *
   * @param t The value to be moved.
   * @return Reference to the current object.
   */
  constexpr maybe &operator=(T &&t) {
    _has = true;
    _t = transfer(t);
    return *this;
  }

  /**
   * @brief Assignment operator.
   * Assigns the value held by the given maybe object to _t and sets _has
   * accordingly.
   *
   * @param o The maybe object.
   * @return Reference to the current object.
   */
  constexpr maybe &operator=(maybe o) {
    _has = transfer(o._has);

    if (_has) {
      _t = transfer(o._t);
    }

    return *this;
  }

  /**
   * @brief Checks if the object holds a value.
   *
   * @return true if the object holds a value, false otherwise.
   */
  constexpr bool has() const { return _has; }

  /**
   * @brief Returns the held value.
   *
   * @return Reference to the held value.
   */
  constexpr T &get() & { return _t; }

  /**
   * @brief Returns the held value.
   *
   * @return Rvalue reference to the held value.
   */
  constexpr T &&get() && { return _t; }

  /**
   * @brief Returns the held value.
   *
   * @return Constant reference to the held value.
   */
  constexpr const T &get() const & { return _t; }

  /**
   * @brief Returns the held value.
   *
   * @return Constant rvalue reference to the held value.
   */
  constexpr const T &&get() const && { return _t; }

  /**
   * @brief Applies a function to the held value and returns the result wrapped
   * in a maybe object.
   *
   * @tparam F The type of the function to be applied.
   * @param f The function to be applied.
   * @return A maybe object holding the result of the function application.
   */
  template <typename F>
  constexpr auto map(F &&f) & {
    using TR = T &;
    using U = decltype(relay<F>(f)(fakeval<TR>()));
    using MU = maybe<U>;

    return _has ? MU(relay<F>(f)(cast<TR>(_t))) : MU();
  }

  /**
   * @brief Applies a function to the held value and returns the result wrapped
   * in a maybe object.
   *
   * @tparam F The type of the function to be applied.
   * @param f The function to be applied.
   * @return A maybe object holding the result of the function application.
   */
  template <typename F>
  constexpr auto map(F &&f) const & {
    using CTR = const T &;
    using U = decltype(relay<F>(f)(fakeval<CTR>()));
    using MU = maybe<U>;

    return _has ? MU(relay<F>(f)(cast<CTR>(_t))) : MU();
  }

  /**
   * @brief Applies a function to the held value and returns the result wrapped
   * in a maybe object.
   *
   * @tparam F The type of the function to be applied.
   * @param f The function to be applied.
   * @return A maybe object holding the result of the function application.
   */
  template <typename F>
  constexpr auto map(F &&f) && {
    using TRR = T &&;
    using U = decltype(relay<F>(f)(fakeval<TRR>()));
    using MU = maybe<U>;

    return _has ? MU(relay<F>(f)(cast<TRR>(_t))) : MU();
  }
  /**
   * @brief Applies a function to the held value and returns the result wrapped
   * in a maybe object.
   *
   * @tparam F The type of the function to be applied.
   * @param f The function to be applied.
   * @return A maybe object holding the result of the function application.
   */
  template <typename F>
  constexpr auto map(F &&f) const && {
    using CTRR = const T &&;
    using U = decltype(relay<F>(f)(fakeval<CTRR>()));
    using MU = maybe<U>;

    return _has ? MU(relay<F>(f)(cast<CTRR>(_t))) : MU();
  }
};

}  // namespace n

#endif
