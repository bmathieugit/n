#ifndef __n_string_hpp__
#define __n_string_hpp__

#include <n/utils.hpp>
#include <n/vector.hpp>

namespace n {
template <typename C>
concept characater = same_as<C, char> || same_as<C, wchar_t>;

template <typename C, size_t N>
class static_string {
 private:
  static_vector<C, N + 1> _data;

 public:
  constexpr static_string() = default;

  // move
  constexpr static_string(static_string&&) = default;
  constexpr static_string& operator=(static_string&&) = default;

  // copy
  constexpr static_string(const static_string&) = default;
  constexpr static_string& operator=(const static_string&) = default;

 public:
  constexpr size_t size() const { return _data.size(); }
  constexpr bool empty() const { return _data.empty(); }
  constexpr bool full() const { return _data.size() == N; }

  constexpr void push(C c) {
    if (!full()) _data.push(c);
  }

  constexpr maybe<C> pop() { return _data.pop(); }
};
}  // namespace n

#endif
