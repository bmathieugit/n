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
struct rxlist {
  string<char> ls;
};

template <unsigned_integral UI>
struct rxmin {
  UI min;
};

template <unsigned_integral UI>
struct rxmax {
  UI max;
};

template <character C>
class extractor<rxlist<C>, C> {
 public:
  template <istream_fragment<C> i>
  static constexpr size_t to(i input, maybe<rxlist<C>>& mls) {
    rxlist<C> ls;
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
result<string<C>, match_error> search_expression(iterator auto irx,
                                                 iterator auto iin) {
  C c = '\0';

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

  // Il faudrait maintenant pouvoir gérer les intervals.
  // C'est à dire les expressions du type : {a-z:0:1}
  // Puis il faudrait gérer la combinaison des deux : {a-z'aa':0:10}.
  // La seconde expression signifie : suite de lettre entre a et z 
  // ou bien de couple de aa le tout entre 0 et 10 fois
  // On pourrait isoler chaque class de caractère dans un item à part 
  // dans un vector, puis itérer sur ce vector pour tester chaque possiblité
  // et enfin voir ce qui match le mieux. Il ne faut pas oublier qu'il faut 
  // former la plus longue réponse possible à la regex. Donc si des classes 
  // de caractère se recoupe il faut prendre celle qui donne le resultat le plus
  // long à chaque iteration. 
  //
  // Il y a une optimisation à apporter : le retour de la fonction pourrait être 
  // non pas une string mais plutot une sorte de vue sur la sousstring qui
  // correspond le plus à la regex. 

  if (parsed and lst.has() and min.has() and max.has()) {
    auto mn = min.get();
    auto mx = max.get();
    auto ils = lst.get().ls.iter();

    if (mn == 0 and mx == 0) {
      return string<C>();
    } else if (mn <= mx) {
      string<C> tmp;
      size_t cnt = 0;

      while (starts_with(iin, ils) and cnt != mx) {
        cnt += 1;
        copy<C>(ils, tmp.oter());
        iin = advance(iin, lst.get().ls.len());
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
  auto irx = rx.iter();
  auto iin = input.iter();

  return search_expression<C>(irx, iin);
}
}  // namespace n

#endif
