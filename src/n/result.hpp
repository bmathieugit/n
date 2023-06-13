#ifndef __n_result_hpp__
#define __n_result_hpp__

#include <n/utils.hpp>

namespace n {

template <typename E>
class result_error {
 private:
  E _error;

 public:
  constexpr ~result_error() = default;
  constexpr result_error() = default;
  constexpr result_error(const E& error) : _error(error) {}
  constexpr result_error(const result_error&) = default;
  constexpr result_error(result_error&&) = default;
  constexpr result_error& operator=(const result_error&) = default;
  constexpr result_error& operator=(result_error&&) = default;

 public:
  constexpr const E& error() const { return _error; }
};

template<typename T, typename E>
class result
{
  
};

template <typename T, typename E>
class result2 {
 private:
  union {
    result_error<E> _e;
    T _t;
  };

  bool _has;

 public:
  constexpr ~result() {
    if (_has) {
      _t.~T();
    }
  }

  constexpr result(const E& e) : _has(false), _e(result_error<E>(e)) {}
  constexpr result(const T& t) : _has(true), _t(t) {}

  constexpr result(const result& r) : _has(r._has) {
    if (_has) {
      _t = r._t;
    } else {
      _e = r._e;
    }
  }

  constexpr result(result&& r) : _has(r._has) {
    if (_has) {
      _t = move(r._t);
    } else {
      _e = move(r._e);
    }
  }

  constexpr result& operator=(const result& r) {
    if (this != &r) {
      _has = r._has;

      if (_has) {
        _t = r._t;
      } else {
        _e = r._e;
      }
    }

    return *this;
  }

  constexpr result& operator=(result&& r) {
    if (this != &r) {
      _has = r._has;
      r._has = false;

      if (_has) {
        _t = move(r._t);
      } else {
        _e = move(r._e);
      }
    }

    return *this;
  }

 public:
  constexpr bool has() const { return _has; }
  constexpr const E& error() const { return _e.error(); }

  constexpr T& get() & { return _t; }
  constexpr T&& get() && { return move(_t); }
  constexpr const T& get() const& { return _t; }
  constexpr const T&& get() const&& { return move(_t); };
};

template <typename T>
class maybe {
 private:
  alignas(T) char _data[sizeof(T)];
  bool _has = false;

 public:
  constexpr ~maybe() {
    if (_has) {
      reinterpret_cast<T*>(_data)->~T();
      _has = false;
    }
  }

  constexpr maybe() = default;

  constexpr maybe(const maybe& o) {
    _has = o._has;

    if (_has) {
      new (_data) T(*reinterpret_cast<const T*>(o._data));
    }
  }

  constexpr maybe(maybe&& o) {
    _has = o._has;

    if (_has) {
      new (_data) T(move(*reinterpret_cast<T*>(o._data)));
    }
  }

  constexpr maybe(const T& t) {
    _has = true;
    new (_data) T(t);
  }

  constexpr maybe(T&& t) {
    _has = true;
    new (_data) T(move(t));
  }

  constexpr maybe& operator=(const maybe& o) {
    if (this != &o) {
      if (_has) {
        reinterpret_cast<T*>(_data)->~T();
        _has = false;
      }

      _has = o._has;

      if (_has) {
        new (_data) T(*reinterpret_cast<const T*>(o._data));
      }
    }

    return *this;
  }

  constexpr maybe& operator=(maybe&& o) {
    if (this != &o) {
      if (_has) {
        reinterpret_cast<T*>(_data)->~T();
        _has = false;
      }

      _has = o._has;

      if (_has) {
        new (_data) T(move(*reinterpret_cast<const T*>(o._data)));
      }
    }

    return *this;
  }

  constexpr maybe& operator=(const T& t) {
    if (_has) {
      reinterpret_cast<T*>(_data)->~T();
    }

    new (_data) T(t);
    _has = true;
    return *this;
  }

  constexpr maybe& operator=(T&& t) {
    if (_has) {
      reinterpret_cast<T*>(_data)->~T();
    }

    new (_data) T(move(t));
    _has = true;
    return *this;
  }

  constexpr bool has() const { return _has; }
  constexpr T& get() & { return *reinterpret_cast<T*>(_data); }
  constexpr T&& get() && { return move(*reinterpret_cast<T*>(_data)); }
  constexpr const T& get() const& { return *reinterpret_cast<T*>(_data); }
  constexpr const T&& get() const&& {
    return move(*reinterpret_cast<T*>(_data));
  }
};
}  // namespace n

#endif
