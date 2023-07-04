#ifndef __n_regex_hpp__
#define __n_regex_hpp__

#include <n/extract.hpp>
#include <n/io.hpp>
#include <n/iterator.hpp>
#include <n/result.hpp>
#include <n/string.hpp>
#include <n/utils.hpp>

namespace n {
// TODO: renommer en match_rc
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
  string<C> ls;
};

template <character C>
class extractor<rxlist<C>, C> {
 public:
  template <istream_fragment<C> I>
  static constexpr size_t to(I i, maybe<rxlist<C>>& m) {
    auto iinit = i;
    size_t l = 0;

    while (i.has_next()) {
      auto icopy = i;
      auto c = icopy.next();
      size_t s = 0;
      if (c == '\'') {
        maybe<rxsqstring<C>> tmp;
        s = extractor<rxsqstring<C>, C>::to(i, tmp);
      } else {
        maybe<rxinterval<C>> tmp;
        s = extractor<rxinterval<C>, C>::to(i, tmp);
      }

      if (s != 0) {
        i = advance(i, s);
        l += s;
      } else {
        break;
      }
    }

    if (l != 0) {
      rxlist<C> ls;
      copy<C>(limit_iterator<I>(iinit, l), ls.ls.oter());
      m = move(ls);
    }

    return l;
  }
};

template <character C>
result<string<C>, match_error> search_sqstring(iterator auto ils,
                                               iterator auto iin) {
  using erc = extract_rc;

  maybe<rxsqstring<C>> msqs;
  maybe<tail_extract<C>> mtail;

  if (extract(ils, "$$", msqs, mtail) == erc::analyse_ok) {
    if (starts_with(iin, msqs.get().ls.iter())) {
      string<C> res;
      copy<C>(msqs.get().ls.iter(), res.oter());
      return move(res);
    } else {
      return match_error::dont_match;
    }
  }

  return match_error::regex_parsing_error;
}

template <character C>
result<string<C>, match_error> search_interval(iterator auto ils,
                                               iterator auto iin) {
  using erc = extract_rc;

  maybe<rxinterval<C>> minter;
  maybe<tail_extract<C>> mtail;

  if (extract(ils, "$$", minter, mtail) == erc::analyse_ok) {
    if (iin.has_next()) {
      auto c = iin.next();
      auto first = minter.get().first;
      auto last = minter.get().last;

      if (first <= c and c <= last) {
        string<C> res;
        res.push(c);
        return move(res);
      } else {
        return match_error::dont_match;
      }
    } else {
      return match_error::dont_match;
    }
  }

  return match_error::regex_parsing_error;
}

template <character C>
result<string<C>, match_error> search_sequence(iterator auto iseq,
                                               iterator auto iin) {
  string<C> res;
  bool one_match = false;

  while (iseq.has_next()) {
    auto iseqcp = iseq;

    if (iseqcp.has_next() and iseqcp.next() == '\'') {
      auto localres = search_sqstring<C>(iseq, iin);

      if (localres.has()) {
        auto len = localres.get().len();
        iin = advance(iin, len);
        iseq = advance(iseq, len + 2);
        copy<C>(localres.get().iter(), res.oter());
        one_match = true;
      } else if (localres.err() == match_error::regex_parsing_error) {
        return match_error::regex_parsing_error;
      }
    } else {
      auto localres = search_interval<C>(iseq, iin);

      if (localres.has()) {
        auto len = localres.get().len();
        iin = advance(iin, len);
        iseq = advance(iseq, len + 2);
        copy<C>(localres.get().iter(), res.oter());
        one_match = true;
      } else if (localres.err() == match_error::regex_parsing_error) {
        return match_error::regex_parsing_error;
      }
    }

    if (not one_match) {
      break;
    }

    one_match = false;
  }

  if (res.len() == 0) {
    return match_error::dont_match;
  } else {
    return res;
  }
}

template <character C>
result<string<C>, match_error> search_expression(iterator auto irx,
                                                 iterator auto iin) {
  maybe<size_t> min;
  maybe<size_t> max;
  maybe<rxlist<C>> lst;

  bool parsed = false;

  using erc = extract_rc;

  if (extract(irx, "{$:$:$}", lst, min, max) == erc::analyse_ok) {
    parsed = true;
  } else if (extract(irx, "{$:$:}", lst, min) == erc::analyse_ok) {
    max = size_t(-1);
    parsed = true;
  } else if (extract(irx, "{$::$}", lst, max) == erc::analyse_ok) {
    min = 0;
    parsed = true;
  }

  if (parsed and lst.has() and min.has() and max.has()) {
    auto mn = min.get();
    auto mx = max.get();
    auto ils = lst.get().ls.iter();

    if (mn == 0 and mx == 0) {
      return string<C>();
    } else if (mn <= mx) {
      string<C> res;
      size_t cnt = 0;

      while (ils.has_next() and cnt <= mx and iin.has_next()) {
        auto localres = search_sequence<C>(ils, iin);

        if (localres.has()) {
          auto len = localres.get().len();
          iin = advance(iin, len);
          copy<C>(localres.get().iter(), res.oter());
          cnt += 1;
        } else if (localres.err() == match_error::dont_match) {
          if (cnt == 0) {
            iin.next();
          } else {
            break;
          }
        } else if (localres.err() == match_error::regex_parsing_error) {
          return match_error::regex_parsing_error;
        }
      }

      if (mn <= cnt and cnt <= mx) {
        return move(res);
      } else {
        return match_error::dont_match;
      }
    } else {
      return match_error::min_more_max;
    }
  } else {
    return match_error::regex_parsing_error;
  }

  return match_error::dont_match;
}

template <character C>
result<string<C>, match_error> search(const string<C>& rx,
                                      const string<C>& input) {
  return search_expression<C>(rx.iter(), input.iter());
}
}  // namespace n

#endif
