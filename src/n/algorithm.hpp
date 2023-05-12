#ifndef __n_algorithm_hpp__
#define __n_algorithm_hpp__

#include <n/utils.hpp>

namespace n {

/**
 * @brief Applies a given function to each element in a range defined by an
 * iterator.
 *
 * @tparam I An iterator type that defines the range to traverse.
 * @tparam F A function type that accepts an element in the range as an
 * argument.
 * @param i An instance of the iterator that defines the range to traverse.
 * @param f The function to apply to each element in the range.
 *
 * @note The function must be a unary function, that is, it must accept exactly
 * one argument of the type pointed to by the iterator.
 */
template <iterator I, typename F>
constexpr void for_each(I i, F &&f) {
  while (i.has_next()) relay<F>(f)(i.next());
}

/**
 * @brief Find the index of the first element in a range that satisfies a
 * predicate.
 *
 * @tparam I An iterator type.
 * @tparam P A predicate type.
 * @param i An iterator to the beginning of the range.
 * @param pred A predicate function or function object.
 * @return The index of the first element in the range that satisfies the
 * predicate, or size_t(-1) if none do.
 *
 * This function searches the range starting at the given iterator `i` for the
 * first element that satisfies the given predicate `pred`, and returns the
 * index of that element. If no elements satisfy the predicate, the function
 * returns `size_t(-1)`.
 *
 * The predicate `pred` should take one argument of the type pointed to by the
 * iterator, and should return a value convertible to bool. If `pred(*i)`
 * returns `true`, the search terminates and the index of `*i` is returned.
 *
 * @note This function assumes that the range pointed to by the iterator `i` has
 * a valid `has_next()` and `next()` member functions that behave as expected
 * for an iterator.
 */
template <iterator I, typename P>
constexpr size_t find(I i, P &&pred) {
  size_t idx = size_t(-1);
  bool fnd = false;

  while (i.has_next()) {
    ++idx;
    if (relay<P>(pred)(i.next())) {
      fnd = true;
      break;
    }
  }

  return fnd ? idx : size_t(-1);
}

/**
 * @brief Find the index of the first occurrence of a value in a range.
 *
 * @tparam I An iterator type.
 * @tparam T A type to compare against.
 * @param i An iterator to the beginning of the range.
 * @param t The value to search for.
 * @return The index of the first occurrence of the value in the range, or
 * size_t(-1) if the value is not found.
 *
 * This function searches the range starting at the given iterator `i` for the
 * first occurrence of the given value `t`, and returns the index of that
 * occurrence. If the value is not found in the range, the function returns
 * `size_t(-1)`.
 *
 * The value `t` is compared to the elements in the range using the `==`
 * operator. The iterator `i` should point to a range of elements that are
 * comparable to the type `T` using this operator.
 *
 * @note This function assumes that the range pointed to by the iterator `i` has
 * a valid `has_next()` and `next()` member functions that behave as expected
 * for an iterator.
 */
template <iterator I, typename T>
constexpr size_t find(I i, const T &t) {
  size_t idx = size_t(-1);
  bool fnd = false;

  while (i.has_next()) {
    ++idx;

    if (i.next() == t) {
      fnd = true;
      break;
    }
  }

  return fnd ? idx : size_t(-1);
}

/**
 * @brief Test whether two ranges are equal.
 *
 * @tparam I0 An iterator type for the first range.
 * @tparam I1 An iterator type for the second range.
 * @param i0 An iterator to the beginning of the first range.
 * @param i1 An iterator to the beginning of the second range.
 * @return `true` if the two ranges are equal, `false` otherwise.
 *
 * This function tests whether the two ranges pointed to by the given iterators
 * `i0` and `i1` are equal. Two ranges are considered equal if they have the
 * same number of elements and each corresponding element in the ranges is
 * equal. The function returns `true` if the ranges are equal and `false`
 * otherwise.
 *
 * The function assumes that the two ranges have the same number of elements. If
 * the two ranges have different lengths, the behavior is undefined.
 *
 * The iterators `i0` and `i1` should point to ranges of elements that are
 * comparable using the `==` operator.
 *
 * @note This function assumes that the ranges pointed to by the iterators `i0`
 * and `i1` have valid `has_next()` and `next()` member functions that behave as
 * expected for an iterator.
 */
template <iterator I0, iterator I1>
constexpr bool equals(I0 i0, I1 i1) {
  while (i0.has_next() && i1.has_next() && i0.next() == i1.next())
    ;

  return !i0.has_next() && !i1.has_next();
}

/**
 * @brief
 *
 * @tparam I0
 * @tparam I1
 * @param i0
 * @param i1
 *
 * @return
 */
template <iterator I0, iterator I1>
constexpr bool starts_with(I0 i0, I1 i1) {
  while (i0.has_next() && i1.has_next() && i0.next() == i1.next())
    ;

  return !i1.has_next();
}

/**
 * @brief
 *
 * @tparam I
 * @tparam T
 * @param i
 * @param t
 *
 * @return
 */
template <iterator I, typename T>
constexpr size_t count(I i, const T &t) {
  size_t c = 0;

  for_each(i, [&c, &t](auto &&item) {
    if (item == t) ++c;
  });

  return c;
}

/**
 * @brief
 *
 * @tparam I
 * @tparam P
 * @param i
 * @param pred
 *
 * @return
 */
template <iterator I, typename P>
constexpr size_t count(I i, P &&pred) {
  size_t c = 0;

  for_each(i, [&c, &pred](auto &&item) {
    if (relay<P>(pred)(item)) ++c;
  });

  return c;
}

/**
 * @brief
 *
 * @tparam I
 * @tparam P
 * @param i
 * @param pred
 *
 * @return
 */
template <iterator I, typename P>
constexpr bool all_of(I i, P &&pred) {
  bool res = true;

  for_each(i, [&res, &pred](auto &&item) {
    if (!relay<P>(pred)(item)) {
      res = false;
      return;
    }
  });

  return res;
}

/**
 * @brief
 *
 * @tparam I
 * @tparam P
 * @param i
 * @param pred
 *
 * @return
 */
template <iterator I, typename P>
constexpr bool any_of(I i, P &&pred) {
  bool res = false;

  for_each(i, [&res, &pred](auto &&item) {
    if (relay<P>(pred)(item)) {
      res = true;
      return;
    }
  });

  return res;
}











  /**
 * @brief Checks if none of the elements in the range satisfy the predicate.
 * 
 * The function template accepts a range defined by a pair of iterators and a unary predicate. 
 * It applies the predicate to each element in the range. If the predicate returns `true` for any 
 * element, the function returns `false`. If the predicate never returns `true`, the function returns `true`.
 * 
 * @tparam I Iterator type, which must satisfy the iterator concept.
 * @tparam P Predicate type, which should be invocable with the value type of the iterator.
 * 
 * @param i An iterator to the beginning of the range to examine.
 * @param pred A unary predicate which returns a `bool` convertible value. 
 *             The signature of the predicate function should be logically equivalent to the following: `bool pred(const Type &a);`
 * 
 * @return `false` if the predicate `pred` returns `true` for any element in the range, `true` otherwise.
 */
template <iterator I, typename P>
constexpr bool none_of(I i, P &&pred) {
  bool res = true;

  for_each(i, [&res, &pred](auto &&item) {
    if (relay<P>(pred)(item)) {
      res = false;
      return;
    }
  });

  return res;
}

/**
 * @brief if iterator i contains the item t then return true,
 * false otherwise
 *
 * @tparam I iterator type
 * @tparam T item type
 * @param i iterator where to search the item t
 * @param t item to search
 *
 * @return true if t is contained into i
 */
template <iterator I, typename T>
constexpr bool contains(I i, const T &t) {
  return any_of(i, [&t](auto &&item) { return item == t; });
}

/**
 * @brief if the content of i0 starts with the content of i1,
 * skip this content in i0 and return the tail of i0.
 *
 * @tparam I0 i0 iterator type
 * @tparam I1 i1 iterator type
 * @param i0 iterator where to skip the prefix i1
 * @param i1 iterator on the prefix to skip
 *
 * @return the tail of i0 after skipping i1
 */
template <iterator I0, iterator I1>
constexpr I0 skip(I0 i0, I1 i1) {
  if (starts_with(i0, i1)) {
    while (i0.has_next() && i1.has_next()) {
      i0.next();
      i1.next();
    }
  }

  return i0;
}

}  // namespace n

#endif
