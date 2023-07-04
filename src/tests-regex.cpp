#include <n/io.hpp>
#include <n/regex.hpp>

int main() {
  auto found = n::search(n::str("{'aa'a-zA-Z:0:2}"), n::str("aaaRaaabb"));

  if (found.has()) {
    n::printf("'$'\n", found.get());
  } else {
    n::printf("pas de match '$'\n", (int)found.err());
  }

  return 0;
}
