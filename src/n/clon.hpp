#ifndef __n_clon_hpp__
#define __n_clon_hpp__

#include <n/string.hpp>

namespace n
{

  enum class clon_node_type
  {
    node,
    string,
    number,
    boolean,
    unknown
  };

  class clon_node_empty
  {
  };

  class clon_node
  {
  private:
    string<char> _name;
    clon_node_type _type = clon_node_type::unknown;

    union
    {
      clon_node_empty _empty;
      clon_node *_node;
      string<char> _str;
      double _num;
      bool _bool;
    };

  public:
    constexpr ~clon_node()
    {
      if (_type == clon_node_type::string)
      {
        _str.~string<char>();
      }
    }

    constexpr clon_node() : _name(), _type(clon_node_type::unknown), _empty() {}
    constexpr clon_node(string_view<char> name, string_view<char> val)
        : _name(name), _type(clon_node_type::string), _str(val) {}
    constexpr clon_node(string_view<char> name, double val)
        : _name(name), _type(clon_node_type::number), _num(val) {}
    constexpr clon_node(string_view<char> name, bool val)
        : _name(name), _type(clon_node_type::boolean), _bool(val) {}

    constexpr clon_node(const clon_node &o)
    {
      _name = o._name;
      _type = o._type;

      switch (o._type)
      {
      case clon_node_type::node:
        _node = o._node;
        break;
      case clon_node_type::boolean:
        _bool = o._bool;
        break;
      case clon_node_type::number:
        _num = o._num;
        break;
      case clon_node_type::string:
        _str = o._str;
        break;
      case clon_node_type::unknown:
        break;
      }
    }

    constexpr clon_node &operator=(const clon_node &o)
    {
      if (this != &o)
      {
        this->~clon_node();
        *this = transfer(clon_node(o));
      }

      return *this;
    }

    constexpr clon_node(clon_node &&) = default;
    constexpr clon_node &operator=(clon_node &&) = default;

  public:
    constexpr auto type() const { return _type; }
    constexpr const auto &name() const { return _name; }
    constexpr const auto &str() const { return _str; }
    constexpr const auto &node() const { return *_node; }
    constexpr const auto &num() const { return _num; }
    constexpr const auto &bool_() const { return _bool; }
  };
} // namespace n

#endif
