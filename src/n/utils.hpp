#ifndef __n_utils_hpp__
#define __n_utils_hpp__

namespace n {

using size_t = unsigned long long;

namespace impl {

template <typename T, typename U>
constexpr bool same_as = false;

template <typename T>
constexpr bool same_as<T, T> = true;

template <typename T>
struct rm_ref {
  using type = T;
};

template <typename T>
struct rm_ref<T&> {
  using type = T;
};

template <typename T>
struct rm_ref<T&&> {
  using type = T;
};

template <typename T>
struct rm_const {
  using type = T;
};

template <typename T>
struct rm_const<const T> {
  using type = T;
};

template <typename T>
struct rm_volatile {
  using type = T;
};

template <typename T>
struct rm_volatile<volatile T> {
  using type = T;
};

}  // namespace impl

template <typename T, typename U>
concept same_as = impl::same_as<T, U>;

template <typename T>
using rm_ref = typename impl::rm_ref<T>::type;

template <typename T>
using rm_const = typename impl::rm_const<T>::type;

template <typename T>
using rm_volatile = typename impl::rm_volatile<T>::type;

template <typename T>
using rm_cvref = rm_ref<rm_const<rm_volatile<T>>>;

template <typename T>
using add_const = const T;

template <typename T>
using add_ref = T&;

template <typename T>
using add_cref = add_const<add_ref<T>>;

template <typename T>
using add_rref = T&&;

}  // namespace n

namespace n {

template <typename T>
constexpr rm_ref<T>&& transfer(T&& t) {
  return static_cast<rm_ref<T>&&>(t);
}

namespace impl {

template <typename T>
constexpr bool pointer_eq = false;

template <typename T>
constexpr bool pointer_eq<T*> = true;
}  // namespace impl

template <typename T>
concept pointer_eq = impl::pointer_eq<rm_ref<T>>;

template <pointer_eq T>
constexpr auto transfer(T&& t) {
  rm_cvref<T> copy = t;
  t = nullptr;
  return copy;
}

template <typename T>
concept number_eq = same_as<rm_ref<T>, char> || same_as<rm_ref<T>, short> ||
                    same_as<rm_ref<T>, int> || same_as<rm_ref<T>, long> ||
                    same_as<rm_ref<T>, long long> || same_as<rm_ref<T>, bool>;

template <number_eq T>
constexpr auto transfer(T&& t) {
  rm_cvref<T> copy = t;
  t = 0;
  return copy;
}

template <typename T>
constexpr T&& relay(rm_ref<T>& t) noexcept {
  return static_cast<T&&>(t);
}

template <typename T>
constexpr T&& relay(rm_ref<T>&& t) noexcept {
  return static_cast<T&&>(t);
}

template <typename U, typename T>
constexpr U cast(T&& t) noexcept {
  return static_cast<U>(t);
}

template <typename T>
constexpr T fakeval() noexcept;

}  // namespace n

#endif
