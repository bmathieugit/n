#ifndef __n_algorithm_hpp__
#define __n_algorithm_hpp__

#include <n/utils.hpp>

namespace n {

/**
 * @brief for each items behind the iterator, apply the function f
 *
 * @tparam I iterator type
 * @tparam F function type
 * @param i iterator
 * @param f function to apply
 */
template <typename I, typename F>
constexpr void for_each(I i, F &&f) {
  while (i.has_next()) relay<F>(f)(i.next());
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
template <typename I, typename P>
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
 * @brief
 *
 * @tparam I
 * @tparam T
 * @param i
 * @param t
 *
 * @return
 */
template <typename I, typename T>
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
 * @brief
 *
 * @tparam I0
 * @tparam I1
 * @param i0
 * @param i1
 *
 * @return
 */
template <typename I0, typename I1>
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
template <typename I0, typename I1>
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
template <typename I, typename T>
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
template <typename I, typename P>
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
template <typename I, typename P>
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
template <typename I, typename P>
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
 * @brief
 *
 * @tparam I
 * @tparam P
 * @param i
 * @param pred
 *
 * @return
 */
template <typename I, typename P>
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
template <typename I, typename T>
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
template <typename I0, typename I1>
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
