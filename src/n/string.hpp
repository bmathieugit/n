#ifndef __n_string_hpp__
#define __n_string_hpp__

#include <string.h>

#include <n/utils.hpp>
#include <n/vector.hpp>

namespace n {

/**
 * @brief A concept to ensure that a type is either a `char`, `wchar_t`, `const
 * char`, or `const wchar_t`.
 *
 * @tparam C The type to be checked.
 */
template <typename C>
concept character = same_as<C, char> || same_as<C, wchar_t> ||
                    same_as<C, const char> || same_as<C, const wchar_t>;

/**
 * @brief A class representing an iterator for a string.
 *
 * @tparam C The type of the character in the string. Must satisfy the
 * `character` concept.
 */
template <character C>
class string_iterator : public vector_iterator<C> {
 private:
  /**
   * @brief Returns the length of the string pointed by `s`.
   *
   * @param s Pointer to the null-terminated string.
   * @return The length of the string.
   */
  constexpr size_t __len(const C* s) {
    auto c = s;

    if (c != nullptr)
      while (*c != '\0') ++c;

    return c - s;
  }

 public:
  /// @brief Default constructor.
  constexpr string_iterator() = default;

  /**
   * @brief Constructor accepting a pointer to the beginning and the end of a
   * string.
   *
   * @param begin Pointer to the beginning of the string.
   * @param end Pointer to the end of the string.
   */
  constexpr string_iterator(C* begin, C* end)
      : vector_iterator<C>(begin, end) {}

  /**
   * @brief Constructor accepting a pointer to a null-terminated string.
   *
   * @param begin Pointer to the null-terminated string.
   */
  constexpr string_iterator(C* begin)
      : vector_iterator<C>(begin, begin + __len(begin)) {}
};

/**
 * @brief A class representing a statically sized string.
 *
 * @tparam C The type of the character in the string. Must satisfy the
 * `character` concept.
 * @tparam N The size of the string.
 */
template <character C, size_t N>
using static_string = static_vector<C, N>;

/**
 * @brief A class representing a dynamically sized string.
 *
 * @tparam C The type of the character in the string. Must satisfy the
 * `character` concept.
 */
template <character C>
using string = vector<C>;

/**
 * @brief A class representing an externally sized string.
 *
 * @tparam C The type of the character in the string. Must satisfy the
 * `character` concept.
 */
template <character C>
using ext_string = ext_vector<C>;

namespace str {

/**
 * @brief Compute the length of a static_string
 *
 * This function returns the size of a static_string.
 *
 * @tparam C The character type of the string.
 * @tparam N The size of the string.
 * @param s The static_string whose size is to be returned.
 * @return The size of the static_string.
 */
template <character C, size_t N>
constexpr size_t len(const static_string<C, N>& s) {
  return s.size();
}

/**
 * @brief Compute the length of a string
 *
 * This function returns the size of a string.
 *
 * @tparam C The character type of the string.
 * @param s The string whose size is to be returned.
 * @return The size of the string.
 */
template <character C>
constexpr size_t len(const string<C>& s) {
  return s.size();
}

/**
 * @brief Compute the length of an ext_string
 *
 * This function returns the size of an ext_string.
 *
 * @tparam C The character type of the string.
 * @param s The ext_string whose size is to be returned.
 * @return The size of the ext_string.
 */
template <character C>
constexpr size_t len(const ext_string<C>& s) {
  return s.size();
}

/**
 * @brief Compute the length of a C-style string
 *
 * This function returns the size of a C-style string, defined as a pointer to
 * characters. The string must be null-terminated.
 *
 * @tparam C The character type of the string.
 * @param s The C-style string whose size is to be returned.
 * @return The size of the C-style string.
 */
template <character C>
constexpr size_t len(const C* s) {
  auto c = s;

  if (c != nullptr)
    while (*c != '\0') ++c;

  return c - s;
}

constexpr void copy(const auto & src, auto& dest) {
  iterator auto i = src.iter();
  while (i.has_next()) {
    dest.push(i.next());
  }
}
}  // namespace str

}  // namespace n

#endif
