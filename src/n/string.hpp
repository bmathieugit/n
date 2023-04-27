#ifndef __n_string_hpp__
#define __n_string_hpp__

#include <n/utils.hpp>
#include <n/vector.hpp>

namespace n {

template <typename C>
concept character = same_as<C, char> || same_as<C, wchar_t>;

template <character C, size_t N>
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
  constexpr auto iter() { return _data.iter(); }
  constexpr auto iter() const { return _data.iter(); }

  constexpr auto riter() {
    // TODO: to implement
  }

  constexpr auto riter() const {
    // TODO: to implement
  }

 public:
  constexpr size_t size() const { return _data.size(); }
  constexpr bool empty() const { return _data.empty(); }
  constexpr bool full() const { return _data.size() == N; }

  constexpr void push(C c) {
    if (!full()) _data.push(c);
  }

  constexpr maybe<C> pop() { return _data.pop(); }
};

template <character C>
class string {
 private:
  vector<C> _data = 10;

 public:
  constexpr string() = default;

  // move
  constexpr string(string&&) = default;
  constexpr string& operator=(string&&) = default;

  // copy
  constexpr string(const string&) = default;
  constexpr string& operator=(const string&) = default;

 public:
  constexpr auto iter() { return _data.iter(); }
  constexpr auto iter() const { return _data.iter(); }

  constexpr auto riter() {
    // TODO: to implement
  }

  constexpr auto riter() const {
    // TODO: to implement
  }

 public:
  constexpr auto  size() const { return _data.size(); }
  constexpr bool empty() const { return _data.empty(); }
  constexpr bool full() const { return size() == max(); }
  constexpr auto max() const { return _data.max() - 1; }

  constexpr void push(C c) {
    if (!full()) _data.push(c);
  }

  constexpr maybe<C> pop() { return _data.pop(); }
};

template <character C>
class ext_string {
 private:
  ext_vector<C> _data = 10;

 public:
  constexpr ext_string() = default;

  // move
  constexpr ext_string(ext_string&&) = default;
  constexpr ext_string& operator=(ext_string&&) = default;

  // copy
  constexpr ext_string(const ext_string&) = default;
  constexpr ext_string& operator=(const ext_string&) = default;

 public:
  constexpr auto iter() { return _data.iter(); }
  constexpr auto iter() const { return _data.iter(); }

  constexpr auto riter() {
    // TODO: to implement
  }

  constexpr auto riter() const {
    // TODO: to implement
  }

 public:
  constexpr auto max() const {return _data.max() -1;}
  constexpr auto size() const { return _data.size(); }
  constexpr bool empty() const { return _data.empty(); }
  constexpr bool full() const { return size() == max() -1; }

  constexpr void push(C c) {
    if (!full()) _data.push(c);
  }

  constexpr maybe<C> pop() { return _data.pop(); }
};


}  // namespace n

#endif
