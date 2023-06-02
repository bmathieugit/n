#include <n/format.hpp>
#include <n/tests.hpp>

int main() {
  n::string<char> bob("Bob");
  auto dest = n::format("coucou $", 1);
  auto idest = dest.iter();
  while (idest.has_next()) printf("%c", idest.next());
}
