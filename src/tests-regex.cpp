#include <n/io.hpp>
#include <n/regex.hpp>

int main() {
  auto found = n::search(n::str("{'aa'a-zA-Z:0:2}"), n::str("GGaaaRaabCq"));

  if (found.has()) {
    n::printf("'$'\n", found.get());
  } else {
    n::printf("pas de match '$'\n", (int)found.err());
  }

  auto found2 = n::search(n::str("{'aa'a-zA-Z:0:1}"), n::str("GGaaaRaaaRq"));

  if (found2.has()) {
    n::printf("'$'\n", found2.get());
  } else {
    n::printf("pas de match '$'\n", (int)found2.err());
  }
  return 0;
}
