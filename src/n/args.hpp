#ifndef __n_args_hpp__
#define __n_args_hpp__

#include <n/algorithm.hpp>
#include <n/string.hpp>
#include <n/utils.hpp>

namespace n {

template <typename S, typename C>
concept string_like =
    same_as<S, string<C>> || same_as<S, ext_string<C>> || same_as<S, const C*>;

enum class argument_type { str, inum, fnum, flag };

template <character C>
class argument_parser {
 private:
  string<C> _program;
  string<C> _prolog;
  string<C> _epilog;

 public:
  argument_parser(const string_like<C> auto& program,
                  const string_like<C> auto& prolog,
                  const string_like<C> auto& epilog)
      : _program(str::len(program)),
        _prolog(str::len(prolog)),
        _epilog(str::len(epilog)) {
    str::copy(program, _program);
    str::copy(prolog, _prolog);
    str::copy(epilog, _epilog);
  }

 public:
  argument_parser& add_argument(const string_like<C> auto& name,
                                const string_like<C> auto& description,
                                bool required = false,
                                argument_type type = argument_type::str);
  bool parse(int argc, char** argv);

  const string<C>& str() const;
  const int& inum() const;
  const float& fnum() const;
  const bool& flag() const;
};
}  // namespace n

#endif
