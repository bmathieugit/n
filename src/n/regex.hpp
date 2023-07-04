#ifndef __n_regex_hpp__
#define __n_regex_hpp__

#include <n/extract.hpp>
#include <n/io.hpp>
#include <n/iterator.hpp>
#include <n/result.hpp>
#include <n/string.hpp>
#include <n/utils.hpp>

namespace n {

enum class match_rc : int { dont_match, regex_parsing_error, min_more_max };

template <character C>
using string_slice = limit_iterator<pointer_iterator<const C>>;

template <character C>
struct rxsqstring {
  string_slice<C> sqs;
};

template <character C>
class extractor<rxsqstring<C>, C> {
 public:
  template <istream_fragment<C> I>
  static constexpr size_t to(I i, maybe<rxsqstring<C>>& msqs) {
    auto icp = i;
    rxsqstring<C> sqs;
    size_t l = 0;

    if (i.has_next()) {
      auto c = i.next();

      if (c != '\'') {
        return 0;
      } else {
        ++l;
      }

      while (i.has_next()) {
        c = i.next();

        if (c != '\'') {
          ++l;
        } else {
          break;
        }
      }

      if (c == '\'') {
        ++l;
        icp.next();
        sqs.sqs = string_slice<C>(icp, l - 2);
        msqs = move(sqs);
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
  string_slice<C> ls;
};

template <character C>
class extractor<rxlist<C>, C> {
 public:
  template <istream_fragment<C> I>
  static constexpr size_t to(I i, maybe<rxlist<C>>& m) {
    auto icp = i;
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
      ls.ls = string_slice<C>(icp, l);
      m = move(ls);
    }

    return l;
  }
};

template <character C>
constexpr result<size_t, match_rc> search_sqstring(iterator auto ils, iterator auto iin) {
  using erc = extract_rc;

  maybe<rxsqstring<C>> msqs;
  maybe<tail_extract<C>> mtail;

  if (extract(ils, "$$", msqs, mtail) == erc::analyse_ok) {
    if (starts_with(iin, msqs.get().sqs)) {
      return msqs.get().sqs.len();
    } else {
      return match_rc::dont_match;
    }
  }

  return match_rc::regex_parsing_error;
}

template <character C>
constexpr result<size_t, match_rc> search_interval(iterator auto ils, iterator auto iin) {
  using erc = extract_rc;

  maybe<rxinterval<C>> minter;
  maybe<tail_extract<C>> mtail;

  if (extract(ils, "$$", minter, mtail) == erc::analyse_ok) {
    if (iin.has_next()) {
      auto c = iin.next();
      auto first = minter.get().first;
      auto last = minter.get().last;

      if (first <= c and c <= last) {
        return 1;
      } else {
        return match_rc::dont_match;
      }
    } else {
      return match_rc::dont_match;
    }
  }

  return match_rc::regex_parsing_error;
}

template <character C>
constexpr result<size_t, match_rc> search_sequence(iterator auto iseq,
                                         iterator auto iin) {
  bool one_match = false;
  size_t l = 0;

  while (iseq.has_next()) {
    auto iseqcp = iseq;

    if (iseqcp.has_next() and iseqcp.next() == '\'') {
      auto localres = search_sqstring<C>(iseq, iin);

      if (localres.has()) {
        auto len = localres.get();
        iin = advance(iin, len);
        iseq = advance(iseq, len + 2);
        l += len;
        one_match = true;
      } else if (localres.err() == match_rc::regex_parsing_error) {
        return match_rc::regex_parsing_error;
      }
    } else {
      auto localres = search_interval<C>(iseq, iin);

      if (localres.has()) {
        iin = advance(iin, 1);
        iseq = advance(iseq, 3);
        l += 1;
        one_match = true;
      } else if (localres.err() == match_rc::regex_parsing_error) {
        return match_rc::regex_parsing_error;
      }
    }

    if (not one_match) {
      break;
    }

    one_match = false;
  }

  if (l == 0) {
    return match_rc::dont_match;
  } else {
    return l;
  }
}

template <character C>
constexpr result<string_slice<C>, match_rc> search_expression(iterator auto irx,
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
    auto ils = lst.get().ls;

    if (mn == 0 and mx == 0) {
      return string_slice<C>();
    } else if (mn <= mx) {
      auto iincp = iin;
      size_t offset = 0;
      size_t size = 0;
      size_t cnt = 0;

      while (ils.has_next() and cnt < mx and iin.has_next()) {
        auto localres = search_sequence<C>(ils, iin);

        if (localres.has()) {
          auto len = localres.get();
          iin = advance(iin, len);
          cnt += 1;
          size += len;
        } else if (localres.err() == match_rc::dont_match) {
          if (cnt == 0) {
            offset += 1;
            iin.next();
          } else {
            break;
          }
        } else if (localres.err() == match_rc::regex_parsing_error) {
          return match_rc::regex_parsing_error;
        }
      }

      if (mn <= cnt and cnt <= mx) {
        return string_slice<C>(advance(iincp, offset), size);
      } else {
        return match_rc::dont_match;
      }
    } else {
      return match_rc::min_more_max;
    }
  } else {
    return match_rc::regex_parsing_error;
  }

  return match_rc::dont_match;
}

template <character C>
constexpr result<string_slice<C>, match_rc> search(const string<C>& rx,
                                   const string<C>& input) {
  return search_expression<C>(rx.iter(), input.iter());
}
}  // namespace n

#endif
