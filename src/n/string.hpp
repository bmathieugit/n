#ifndef __n_string_hpp__
#define __n_string_hpp__

#include <n/utils.hpp>
#include <n/vector.hpp>

namespace n {

template <typename C>
concept character = same_as<C, char> || same_as<C, wchar_t>;

template <character C>
class string_view : public vector_view<C> {
 private:
  constexpr size_t __len(const C* s) {
    auto c = s;

    if (c != nullptr)
      while (*c != '\0') ++c;

    return c - s;
  }

 public:
  constexpr ~string_view() = default;
  constexpr string_view() = default;
  constexpr string_view(const C* begin)
      : vector_view<C>(begin, begin + __len(begin)) {}
  constexpr string_view(const C* begin, const C* end)
      : vector_view<C>(begin, end) {}
  constexpr string_view(const C* begin, size_t len)
      : vector_view<C>(begin, begin + len) {}

  // move
  constexpr string_view(string_view&&) = default;
  constexpr string_view& operator=(string_view&&) = default;

  // copy
  constexpr string_view(const string_view&) = default;
  constexpr string_view& operator=(const string_view&) = default;
};

template<character C, size_t N>
using static_string = static_vector<C, N>;

template <character C>
using string =  vector<C>;

template <character C>
using ext_string = ext_vector<C>;

}  // namespace n

#endif
