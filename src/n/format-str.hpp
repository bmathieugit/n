

#include <n/format-core.hpp

namespace n{
template <>
constexpr char format_joker<char> = '$';

template <>
constexpr wchar_t format_joker<wchar_t> = L'$';

template<character C>
class format_pattern_iterator<C> : public cstring_iterator<C>{
  public:
    format_pattern_iterator(const C* s) : cstring_iterator<C>(s){}
};

template <formattable<char, string<char>>... T>
constexpr string<char> format(const char* fmt, const T&... t) {
  return __format<char, string<char>>(format_pattern_iterator<char>(fmt), t...);
}

template <ostream<char> O, formattable<char, O>... T>
constexpr void format_to(O& dest, const char* fmt, const T&... t) {
  return __format_to(dest, format_pattern_iterator<char>(fmt), t...);
}

} 
