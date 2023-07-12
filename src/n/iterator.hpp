#ifndef __n_iterator_hpp__
#define __n_iterator_hpp__

#include <n/utils.hpp>

namespace n {

template <typename I>
concept __iterator_has_next = requires(I i) {
                                { i.has_next() } -> same_as<bool>;
                              };

template <typename I>
concept __iterator_next = requires(I i) { i.next(); };

template <typename I>
concept iterator = __iterator_has_next<I> and __iterator_next<I>;

template <typename O, typename T>
concept oterator = requires(O o, T t) { o.sext(t); };

}  // namespace n

namespace n {
template <typename T>
class pointer_iterator {
 private:
  T* _begin;
  T* _end;

 public:
  constexpr pointer_iterator() : _begin(nullptr), _end(nullptr) {}
  constexpr pointer_iterator(T* begin, T* end) : _begin(begin), _end(end) {}
  constexpr pointer_iterator(T* begin, size_t end)
      : _begin(begin), _end(begin + end) {}

 public:
  constexpr bool has_next() const { return _begin != _end; }
  constexpr T& next() { return *(_begin++); }
};

template <typename T>
class pointer_oterator {
 private:
  T* _begin;
  T* _end;

 public:
  constexpr pointer_oterator() : _begin(nullptr), _end(nullptr) {}
  constexpr pointer_oterator(T* begin, T* end) : _begin(begin), _end(end) {}
  constexpr pointer_oterator(T* begin, size_t end)
      : _begin(begin), _end(begin + end) {}

 public:
  constexpr void sext(const T& t) {
    if (_begin != _end) *(_begin++) = t;
  }

  constexpr void sext(T&& t) {
    if (_begin != _end) *(_begin++) = move(t);
  }
};

template <iterator I>
  requires default_constructible<I>
class limit_iterator {
 private:
  I _it;
  size_t _limit;

 public:
  constexpr limit_iterator() : _it(), _limit(0) {}
  constexpr limit_iterator(I it, size_t limit) : _it(move(it)), _limit(limit) {}
  constexpr limit_iterator(limit_iterator i, size_t limit): _it(move(i._it)), _limit(limit) {}

 public:
  constexpr bool has_next() const { return _limit != 0 and _it.has_next(); }

  constexpr auto next() -> decltype(auto) {
    _limit -= 1;
    return _it.next();
  }

  constexpr auto len() const { return _limit; }
};

template <typename T, oterator<T> O>
  requires default_constructible<O>
class limit_oterator {
 private:
  O _ot;
  size_t _limit;

 public:
  constexpr limit_oterator() : _ot(), _limit(0) {}
  constexpr limit_oterator(O ot, size_t limit) : _ot(move(ot)), _limit(limit) {}

 public:
  constexpr auto sext(const T& t) -> decltype(auto) {
    if (_limit != 0) {
      _limit -= 1;
      return _ot.sext(t);
    }
  }

  constexpr auto sext(T&& t) -> decltype(auto) {
    if (_limit != 0) {
      _limit -= 1;
      return _ot.sext(move(t));
    }
  }

  constexpr auto len() const { return _limit; }
};

template <character C>
class cstring_iterator {
 private:
  const C* _begin;

 public:
  constexpr cstring_iterator(const C* begin) : _begin(begin) {}

 public:
  constexpr bool has_next() const { return *_begin != '\0'; }
  constexpr C next() { return *(_begin++); }
};

}  // namespace n

namespace n {

template <typename T, iterator I, oterator<T> O>
constexpr void copy(I i, O o) {
  while (i.has_next()) {
    o.sext(i.next());
  }
}

template <typename T, iterator I, oterator<T> O>
constexpr void move(I i, O o) {
  while (i.has_next()) {
    o.sext(move(i.next()));
  }
}

template <iterator I0, iterator I1>
constexpr bool equal(I0 i0, I1 i1) {
  while (i0.has_next() and i1.has_next())
    if (i0.next() != i1.next()) return false;

  return !i0.has_next() and !i1.has_next();
}

template <typename T, iterator I, oterator<T> O>
  requires has_equals_operator<T, T>
constexpr void copy_until(I i, O o, const T& del) {
  while (i.has_next()) {
    const auto& item = i.next();
    if (item == del) {
      break;
    } else {
      o.sext(item);
    }
  }
}

template <typename T, iterator I, oterator<T> O>
  requires has_equals_operator<T, T>
constexpr void move_until(I i, O o, const T& del) {
  while (i.has_next()) {
    auto&& item = move(i.next());
    if (item == del) {
      break;
    } else {
      o.sext(move(item));
    }
  }
}

template <iterator I>
constexpr I advance(I i, size_t n) {
  while (n != 0 and i.has_next()) {
    i.next();
    n -= 1;
  }

  return i;
}

template <iterator I0, iterator I1>
constexpr bool starts_with(I0 i0, I1 i1) {
  while (i0.has_next() and i1.has_next()) {
    if (i0.next() != i1.next()) {
      return false;
    }
  }

  return true;
}

}  // namespace n
#endif
