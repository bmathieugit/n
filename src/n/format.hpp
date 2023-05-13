#ifndef __n_format_hpp__
#define __n_format_hpp__

#include <stdio.h>

#include <n/string.hpp>
#include <n/utils.hpp>

namespace n {

/**
 * @brief An undefined template class for formatter objects.
 *
 * This class should be specialized for types that can be formatted.
 * It's used to define the formattable concept.
 *
 * @tparam T The type to be formatted.
 */
template <typename T>
class formatter;

/**
 * @brief Concept to define if a type is formattable.
 *
 * A type is formattable if a formatter specialization exists for this type.
 *
 * @tparam T The type to check for formattability.
 */
template <typename T>
concept formattable = requires { formatter<T>{}; };

/**
 * @brief Concept to define if a type has the push method (i.e., is a stream).
 *
 * A type is considered a stream (in this context) if it has a push method
 * that can take either a char or wchar_t parameter.
 *
 * @tparam T The type to check for stream-ness.
 */
template <typename T>
concept ostream =
    requires(T t) { t.push('c'); } || requires(T t) { t.push(wchar_t('c')); };

/**
 * @brief Formats one object to an ostream.
 *
 * This function uses a formatter to format an object and push it to an ostream.
 * It uses the ostream's push method and the formatter's to method.
 *
 * @tparam O The type of the ostream. Must satisfy the ostream concept.
 * @tparam T The type of the object to be formatted. Must satisfy the
 * formattable concept.
 * @param o The ostream to which the object will be formatted.
 * @param t The object to be formatted.
 */
template <ostream O, formattable T>
constexpr void format_one_to(O &o, const T &t) {
  formatter<rm_cvref<T>>::to(o, t);
}

namespace impl {

/**
 * @brief Formats one object to an ostream, character by character until a
 * special character '$' is found.
 *
 * This function iterates over a string view and formats each character to an
 * ostream. When it encounters a '$' character, it stops formatting characters
 * and formats the object t to the ostream.
 *
 * @tparam O The type of the ostream. Must satisfy the ostream concept.
 * @tparam I The type of the iterator for the string view.
 * @tparam T The type of the object to be formatted. Must satisfy the
 * formattable concept.
 * @param o The ostream to which the object will be formatted.
 * @param ifmt An iterator to the string view to be formatted.
 * @param t The object to be formatted.
 */
template <ostream O, typename I, formattable T>
constexpr void format_to_one_by_one(O &o, I &ifmt, const T &t) {
  while (ifmt.has_next()) {
    auto c = ifmt.next();

    if (c == '$') {
      break;
    } else {
      format_one_to(o, c);
    }
  }

  format_one_to(o, t);
}

/**
 * @brief Formats a series of objects to an ostream according to a format
 * string.
 *
 * This function uses a variadic template to accept an arbitrary number of
 * objects to format. It formats each object to the ostream one by one until it
 * has formatted all objects.
 *
 * @tparam C The character type of the string view.
 * @tparam O The type of the ostream. Must satisfy the ostream concept.
 * @tparam T The types of the objects to be formatted. Must satisfy the
 * formattable concept.
 * @param o The ostream to which the objects will be formatted.
 * @param fmt A string iterator containing the format string.
 * @param t The objects to be formatted.
 */
template <character C, ostream O, formattable... T>
constexpr void format_to(O &o, string_iterator<C> fmt, const T &...t) {
  auto ifmt = fmt;

  (format_to_one_by_one(o, ifmt, t), ...);
  format_one_to(o, ifmt);
}

}  // namespace impl

/**
 * @brief Overload of format_to function for char string views.
 *
 * This function calls the impl::format_to function with the given parameters.
 *
 * @tparam O The type of the ostream. Must satisfy the ostream concept.
 * @tparam T The types of the objects to be formatted. Must satisfy the
 * formattable concept.
 * @param o The ostream to which the objects will be formatted.
 * @param fmt A string view of type char containing the format string.
 * @param t The objects to be formatted.
 */
template <ostream O, formattable... T>
constexpr void format_to(O &o, string_iterator<char> fmt, const T &...t) {
  impl::format_to(o, fmt, t...);
}

/**
 * @brief Overload of format_to function for wchar_t string views.
 *
 * This function calls the impl::format_to function with the given parameters.
 *
 * @tparam O The type of the ostream. Must satisfy the ostream concept.
 * @tparam T The types of the objects to be formatted. Must satisfy the
 * formattable concept.
 * @param o The ostream to which the objects will be formatted.
 * @param fmt A string iterator of type wchar_t containing the format string.
 * @param t The objects to be formatted.
 */
template <ostream O, formattable... T>
constexpr void format_to(O &o, string_iterator<wchar_t> fmt, const T &...t) {
  impl::format_to(o, fmt, t...);
}

/**
 * @brief Overload of format_to function for const char string iterator.
 *
 * This function calls the impl::format_to function with the given parameters.
 *
 * @tparam O The type of the ostream. Must satisfy the ostream concept.
 * @tparam T The types of the objects to be formatted. Must satisfy the
 * formattable concept.
 * @param o The ostream to which the objects will be formatted.
 * @param fmt A string view of type char containing the format string.
 * @param t The objects to be formatted.
 */
template <ostream O, formattable... T>
constexpr void format_to(O &o, string_iterator<const char> fmt, const T &...t) {
  impl::format_to(o, fmt, t...);
}

/**
 * @brief Overload of format_to function for const wchar_t string iterator.
 *
 * This function calls the impl::format_to function with the given parameters.
 *
 * @tparam O The type of the ostream. Must satisfy the ostream concept.
 * @tparam T The types of the objects to be formatted. Must satisfy the
 * formattable concept.
 * @param o The ostream to which the objects will be formatted.
 * @param fmt A string iterator of type wchar_t containing the format string.
 * @param t The objects to be formatted.
 */
template <ostream O, formattable... T>
constexpr void format_to(O &o, string_iterator<const wchar_t> fmt, const T &...t) {
  impl::format_to(o, fmt, t...);
}
/**
 * @brief Overload of format_to function for wchar_t string views.
 *
 * This function calls the impl::format_to function with the given parameters.
 *
 * @tparam O The type of the ostream. Must satisfy the ostream concept.
 * @tparam T The types of the objects to be formatted. Must satisfy the
 * formattable concept.
 * @param o The ostream to which the objects will be formatted.
 * @param fmt A string view of type wchar_t containing the format string.
 * @param t The objects to be formatted.
 */
template <character C>
class formatter<C> {
 public:
  /**
   * @brief Overload of format_to function for wchar_t string views.
   *
   * This function calls the impl::format_to function with the given parameters.
   *
   * @tparam O The type of the ostream. Must satisfy the ostream concept.
   * @tparam T The types of the objects to be formatted. Must satisfy the
   * formattable concept.
   * @param o The ostream to which the objects will be formatted.
   * @param fmt A string view of type wchar_t containing the format string.
   * @param t The objects to be formatted.
   */
  template <ostream O>
  constexpr static void to(O &os, C c) {
    os.push(c);
  }
};

/**
 * @brief Formatter specialization for signed integral types.
 *
 * This formatter specialization formats a signed integral number as a decimal
 * string.
 *
 * @tparam I The signed integral type to be formatted.
 */
template <signed_integral I>
class formatter<I> {
 public:
  /**
   * @brief Formats a signed integral number to an ostream.
   *
   * @tparam O The type of the ostream. Must satisfy the ostream concept.
   * @param o The ostream to which the number will be formatted.
   * @param i The signed integral number to be formatted.
   */
  template <ostream O>
  constexpr static void to(O &o, I i) {
    static_vector<char, 20> tbuff;

    if (i == 0) {
      tbuff.push('0');
    } else {
      const bool neg = i < 0;
      I tmp = neg ? -i : i;

      while (tmp != 0) {
        tbuff.push("0123456789"[tmp % 10]);
        tmp /= 10;
      }

      if (neg) {
        tbuff.push('-');
      }
    }

    format_one_to(o, tbuff.riter());
  }
};

/**
 * @brief Formatter specialization for unsigned integral types.
 *
 * This formatter specialization formats an unsigned integral number as a
 * decimal string.
 *
 * @tparam I The unsigned integral type to be formatted.
 */
template <unsigned_integral I>
class formatter<I> {
 public:
  /**
   * @brief Formats an unsigned integral number to an ostream.
   *
   * @tparam O The type of the ostream. Must satisfy the ostream concept.
   * @param o The ostream to which the number will be formatted.
   * @param i The unsigned integral number to be formatted.
   */
  template <ostream O>
  constexpr static void to(O &o, I i) {
    static_vector<char, 20> tbuff;

    if (i == 0) {
      tbuff.push('0');
    } else {
      while (i != 0) {
        tbuff.push("0123456789"[i % 10]);
        i /= 10;
      }
    }

    format_one_to(o, tbuff.riter());
  }
};

/**
 * @brief Formatter specialization for floating point types.
 *
 * This formatter specialization formats a floating point number to an ostream
 * as a decimal string with four decimal places.
 *
 * @tparam F The floating point type to be formatted.
 */
template <floating_point F>
class formatter<F> {
 public:
  /**
   * @brief Formats a floating point number to an ostream.
   *
   * @tparam O The type of the ostream. Must satisfy the ostream concept.
   * @param o The ostream to which the number will be formatted.
   * @param d The floating point number to be formatted.
   */
  template <ostream O>
  constexpr static void to(O &o, F d) {
    size_t i = static_cast<size_t>(d);
    size_t e = static_cast<size_t>((d - i) * 10'000.0);

    format_one_to(o, i);
    format_one_to(o, '.');
    format_one_to(o, e);
  }
};

/**
 * @brief Formatter specialization for boolean types.
 *
 * This formatter specialization formats a boolean value to an ostream as either
 * "true" or "false".
 */
template <>
class formatter<bool> {
 public:
  /**
   * @brief Formats a boolean value to an ostream.
   *
   * @tparam O The type of the ostream. Must satisfy the ostream concept.
   * @param o The ostream to which the boolean value will be formatted.
   * @param b The boolean value to be formatted.
   */
  template <ostream O>
  constexpr static void to(O &o, bool b) {
    constexpr string_iterator<const char> _true = "true";
    constexpr string_iterator<const char> _false = "false";
    format_one_to(o, b ? _true : _false);
  }
};

/**
 * @brief Formatter specialization for iterator types.
 *
 * This formatter specialization formats the elements in the sequence
 * represented by an iterator to an ostream.
 *
 * @tparam I The iterator type to be formatted.
 */
template <iterator I>
class formatter<I> {
 public:
  /**
   * @brief Formats the elements in the sequence represented by an iterator to
   * an ostream.
   *
   * @tparam O The type of the ostream. Must satisfy the ostream concept.
   * @param o The ostream to which the elements will be formatted.
   * @param i The iterator to the sequence to be formatted.
   */
  template <ostream O>
  constexpr static void to(O &o, I i) {
    while (i.has_next()) {
      format_one_to(o, i.next());
    }
  }
};

/**
 * @brief Formatter specialization for character pointers.
 *
 * This formatter specialization formats a C-style string (null-terminated
 * character array) to an ostream.
 *
 * @tparam C The character type of the string.
 */
template <character C>
class formatter<C *> {
 public:
  /**
   * @brief Formats a C-style string to an ostream.
   *
   * @tparam O The type of the ostream. Must satisfy the ostream concept.
   * @param o The ostream to which the string will be formatted.
   * @param s The C-style string to be formatted.
   */
  template <ostream O>
  constexpr static void to(O &o, C *s) {
    format_one_to(o, string_iterator<C>(s));
  }
};

/**
 * @brief Formatter specialization for character arrays.
 *
 * This formatter specialization formats a character array to an ostream.
 *
 * @tparam C The character type of the array.
 * @tparam N The size of the array.
 */
template <character C, size_t N>
class formatter<C[N]> {
 public:
  /**
   * @brief Formats a character array to an ostream.
   *
   * @tparam O The type of the ostream. Must satisfy the ostream concept.
   * @param o The ostream to which the array will be formatted.
   * @param s The character array to be formatted.
   */
  template <ostream O>
  constexpr static void to(O &o, const C (&s)[N]) {
    format_one_to(o, string_iterator<const C>(s));
  }
};

/**
 * @brief Formatter specialization for const character pointers.
 *
 * This formatter specialization formats a const C-style string (null-terminated
 * character array) to an ostream.
 *
 * @tparam C The character type of the string.
 */
template <character C>
class formatter<const C *> {
 public:
  /**
   * @brief Formats a const C-style string to an ostream.
   *
   * @tparam O The type of the ostream. Must satisfy the ostream concept.
   * @param o The ostream to which the string will be formatted.
   * @param s The const C-style string to be formatted.
   */
  template <ostream O>
  constexpr static void to(O &o, const C *s) {
    format_one_to(o, string_iterator<const C>(s));
  }
};

/**
 * @brief Formatter specialization for const character arrays.
 *
 * This formatter specialization formats a const character array to an ostream.
 *
 * @tparam C The character type of the array.
 * @tparam N The size of the array.
 */
template <character C, size_t N>
class formatter<const C[N]> {
 public:
  /**
   * @brief Formats a const character array to an ostream.
   *
   * @tparam O The type of the ostream. Must satisfy the ostream concept.
   * @param o The ostream to which the array will be formatted.
   * @param s The const character array to be formatted.
   */
  template <ostream O>
  constexpr static void to(O &o, const C (&s)[N]) {
    format_one_to(o, string_iterator<const C>(s));
  }
};

/**
 * @brief Formatter specialization for pointers.
 * This formatter specialization formats a pointer to an ostream.
 *
 * @tparam P The type of the pointer. Must satisfy the pointer_eq concept.
 */
template <pointer_eq P>
class formatter<P> {
 public:
  /**
   * @brief Formats a pointer to an ostream.
   *
   * The pointer is formatted as its memory address.
   *
   * @tparam O The type of the ostream. Must satisfy the ostream concept.
   * @param os The ostream to which the pointer will be formatted.
   * @param p The pointer to be formatted.
   */
  template <ostream O>
  constexpr static void to(O &os, P p) {
    format_one_to(os, (size_t)(void *)(p));
  }
};

}  // namespace n
#endif
