#include <n/io.hpp>
#include <n/tests.hpp>

int main() {
  n::string<char> bob = n::str("bob");
  n::printf("coucou $", bob);

  return 0;
}
