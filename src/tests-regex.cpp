#include <n/io.hpp>
#include <n/regex.hpp>

int main() {
  auto found = n::search(n::str("{'aa'a-zA-Z:0:2}"), n::str("GGaaaRaabCq"));

  if (found.has()) {
    ::printf("'%lu'\n", found.get().len());
  } else {
    ::printf("pas de match '%d'\n", (int)found.err());
  }

 return 0;
}
