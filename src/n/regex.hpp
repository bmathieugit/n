#ifndef __n_regex_hpp__
#define __n_regex_hpp__

#include <n/extract.hpp>
#include <n/io.hpp>
#include <n/iterator.hpp>
#include <n/result.hpp>
#include <n/string.hpp>
#include <n/utils.hpp>
namespace n {
enum class match_error : int { dont_match, regex_parsing_error, min_more_max };

template <character C>
struct rxsqstring {
  string<char> ls;
};

template <character C>
class extractor<rxsqstring<C>, C> {
 public:
  template <istream_fragment<C> i>
  static constexpr size_t to(i input, maybe<rxsqstring<C>>& mls) {
    rxsqstring<C> ls;
    size_t l = 0;

    if (input.has_next()) {
      auto c = input.next();

      if (c != '\'') {
        return 0;
      } else {
        ++l;
      }

      while (input.has_next()) {
        c = input.next();

        if (c != '\'') {
          ++l;
          ls.ls.push(c);
        } else {
          break;
        }
      }

      if (c == '\'') {
        ++l;

        mls = move(ls);
        return l;
      }
    }

    return 0;
  }
};

template <character C>
struct rxinterval {
  C first;
  C last;
};

template <character C>
class extractor<rxinterval<C>, C> {
 public:
  template <istream_fragment<C> I>
  static constexpr size_t to(I i, maybe<rxinterval<C>>& m) {
    rxinterval<C> inter;
    C first = '\0';
    C minus = '\0';
    C last = '\0';

    if (i.has_next()) {
      first = i.next();
    }

    if (i.has_next()) {
      minus = i.next();
    }

    if (i.has_next()) {
      last = i.next();
    }

    if (first != '\0' and minus == '-' and last != '\0') {
      inter.first = first;
      inter.last = last;

      m = move(inter);
      return 3;
    }

    return 0;
  }
};

template <character C>
struct rxlist {
  variant<rxsqstring<C>, rxinterval<C>> ls;
};

template <character C>
class extractor<rxlist<C>, C> {
 public:
  template <istream_fragment<C> I>
  static constexpr size_t to(I i, maybe<rxlist<C>>& m) {
    size_t s = 0;
    rxlist<C> ls;

    if (i.has_next()) {
      auto itmp = i;
      auto tmp = itmp.next();

      if (tmp == '\'') {
        maybe<rxsqstring<C>> msqs;
        s = extractor<rxsqstring<C>, C>::to(i, msqs);

        if (msqs.has()) {
          ls.ls = move(msqs.get());
          m = move(ls);
        }
      } else {
        maybe<rxinterval<C>> minter;
        s = extractor<rxinterval<C>, C>::to(i, minter);

        if (minter.has()) {
          ls.ls = move(minter.get());
          m = move(ls);
        }
      }
    }

    return s;
  }
};

template <character C>
result<string<C>, match_error> search_expression(iterator auto irx,
                                                 iterator auto iin) {
  maybe<size_t> min;
  maybe<size_t> max;
  maybe<rxlist<C>> lst;

  bool parsed = false;

  using ee = extract_error;

  if (extract(irx, "{$:$:$}", lst, min, max) == ee::analyse_ok) {
    parsed = true;
  } else if (extract(irx, "{$:$:}", lst, min) == ee::analyse_ok) {
    max = size_t(-1);
    parsed = true;
  } else if (extract(irx, "{$::$}", lst, max) == ee::analyse_ok) {
    min = 0;
    parsed = true;
  }

  if (parsed and lst.has() and min.has() and max.has()) {
    auto mn = min.get();
    auto mx = max.get();
    auto ils = lst.get().ls.get().iter();

    if (mn == 0 and mx == 0) {
      return string<C>();
    } else if (mn <= mx) {
      string<C> tmp;
      size_t cnt = 0;

      while (starts_with(iin, ils) and cnt != mx) {
        cnt += 1;
        copy<C>(ils, tmp.oter());
        iin = advance(iin, lst.get().ls.get().len());
      }

      if (mn <= cnt and cnt <= mx) {
        return move(tmp);
      } else {
        return match_error::dont_match;
      }
    } else {
      return match_error::min_more_max;
    }
  } else {
    return match_error::regex_parsing_error;
  }
}

template <character C>
result<string<C>, match_error> search(const string<C>& rx,
                                      const string<C>& input) {
  return search_expression<C>(rx.iter(), input.iter());
}
}  // namespace n

#endif
