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

template<character C>
class cstring_iterator{
  private:
    const C* _begin;
  public:
    constexpr cstring_iterator(const C* begin): _begin(begin) {
    }

  public:
    constexpr bool has_next() const {return *_begin != '\0';}
    constexpr C next() {return *(_begin++);}
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
constexpr void transfer(I i, O o) {
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
}  // namespace n
#endif
