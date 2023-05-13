#ifndef __n_clon_hpp__
#define __n_clon_hpp__

#include <n/string.hpp>
#include <n/utils.hpp>
#include <n/variant.hpp>

namespace n {

enum class clon_node_type : size_t {
  object = 0,
  string = 1,
  number = 2,
  boolean = 3,
  unknown = 4
};

struct clon_string {
  ext_string<char> _data;

  string_view<char> data() const { return string_view<char>(_data.iter()); }
};

struct clon_number {
  double _data;
};

struct clon_boolean {
  bool _data;
};

struct clon_object {
  size_t _data;
};

struct clon_unknown {};

class clon_node {
  using num = clon_number;
  using str = clon_string;
  using boo = clon_boolean;
  using obj = clon_object;
  using unk = clon_unknown;

 private:
  variant<obj, str, num, boo, unk> _val;

 public:
 public:
  clon_node_type type() const {
    return static_cast<clon_node_type>(_val.index());
  }

  const clon_number& number() const { return _val.get<clon_number>(); }
  const clon_string& string() const { return _val.get<clon_string>(); }
  const clon_boolean& boolean() const { return _val.get<clon_boolean>(); }
  const clon_object& object() const { return _val.get<clon_object>(); }
};

}  // namespace n

#endif
