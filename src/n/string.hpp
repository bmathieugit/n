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

}  // namespace n

#endif
