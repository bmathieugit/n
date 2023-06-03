#ifndef __n_utils_hpp__
#define __n_utils_hpp__

namespace n {

using size_t = unsigned long long;

template <typename T, typename U>
constexpr bool __same_as = false;

template <typename T>
constexpr bool __same_as<T, T> = true;

template <typename T, typename U>
concept same_as = __same_as<T, U>;

template <typename T>
struct __rm_ref {
  using type = T;
};

template <typename T>
struct __rm_ref<T&> {
  using type = T;
};

template <typename T>
struct __rm_ref<T&&> {
  using type = T;
};

template <typename T>
using rm_ref = typename __rm_ref<T>::type;

template <typename T>
constexpr rm_ref<T>&& move(T&& t) {
  return static_cast<rm_ref<T>&&>(t);
}

template <typename T>
constexpr T&& relay(rm_ref<T>& t) noexcept {
  return static_cast<T&&>(t);
}

template <typename T>
constexpr T&& relay(rm_ref<T>&& t) noexcept {
  return static_cast<T&&>(t);
}

template <typename T>
concept character = same_as<T, char> or same_as<T, wchar_t>;

template <character C>
constexpr size_t strlen(const C* s) {
  size_t i = 0;
  if (s != nullptr)
    while (s[i] != '\0') ++i;
  return i;
}
}  // namespace n

#endif
