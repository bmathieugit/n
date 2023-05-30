#include <n/string.hpp>
#include <n/tests.hpp>

void test_empty() {
  n::static_string<char, 5> v;
  N_TEST_ASSERT_TRUE(v.empty());
  N_TEST_ASSERT_EQUALS(v.size(), 0);
  N_TEST_ASSERT_EQUALS(v.max(), 5);
}

void test_push_and_size() {
  n::static_string<char, 5> v;
  v.push(1);
  N_TEST_ASSERT_FALSE(v.empty());
  N_TEST_ASSERT_EQUALS(v.size(), 1);
  v.push(2);
  N_TEST_ASSERT_EQUALS(v.size(), 2);
}

void test_push_and_index_operator() {
  n::static_string<char, 5> v;
  v.push(1);
  v.push(2);
  N_TEST_ASSERT_EQUALS(v[0], 1);
  N_TEST_ASSERT_EQUALS(v[1], 2);
}

/*
TODO: fixer ce test
   void test_push_move() {
  n::static_string<std::unique_ptr<int>, 5> v;
  v.push(std::make_unique<int>(1));
  N_TEST_ASSERT_EQUALS(*v[0], 1);
}*/

void test_pop() {
  n::static_string<char, 5> v;
  v.push(1);
  auto item = v.pop();
  N_TEST_ASSERT_TRUE(item.has());
  auto i = item.get();
  N_TEST_ASSERT_EQUALS(i, 1);
  N_TEST_ASSERT_TRUE(v.empty());
}

void test_clear() {
  n::static_string<char, 5> v;
  v.push(1);
  v.push(2);
  v.clear();
  N_TEST_ASSERT_TRUE(v.empty());
  N_TEST_ASSERT_EQUALS(v.size(), 0);
}

void test_iterator() {
  n::static_string<char, 5> v;
  v.push(1);
  v.push(2);
  v.push(3);
  int sum = 0;
  for (auto it = v.iter(); it.has_next();) {
    sum += it.next();
  }
  N_TEST_ASSERT_EQUALS(sum, 6);
}

void test_reverse_iterator() {
  n::static_string<char, 5> v;
  v.push(1);
  v.push(2);
  v.push(3);
  int sum = 0;
  for (auto it = v.riter(); it.has_next();) {
    sum += it.next();
  }
  N_TEST_ASSERT_EQUALS(sum, 6);
}

//////////////////////////////
namespace vec {
void test_empty() {
  n::string<char> v(5);
  N_TEST_ASSERT_TRUE(v.empty());
  N_TEST_ASSERT_EQUALS(v.size(), 0);
  N_TEST_ASSERT_EQUALS(v.max(), 5);
}

void test_push_and_size() {
  n::string<char> v(5);
  v.push(1);
  N_TEST_ASSERT_FALSE(v.empty());
  N_TEST_ASSERT_EQUALS(v.size(), 1);
  v.push(2);
  N_TEST_ASSERT_EQUALS(v.size(), 2);
}

void test_push_and_index_operator() {
  n::string<char> v(5);
  v.push(1);
  v.push(2);
  N_TEST_ASSERT_EQUALS(v[0], 1);
  N_TEST_ASSERT_EQUALS(v[1], 2);
}

/*
  TODO: fixer ce test
   void test_push_move() {
  n::string<std::unique_ptr<int>> v(5);
  v.push(std::make_unique<int>(1));
  N_TEST_ASSERT_EQUALS(*v[0], 1);
}
*/
void test_pop() {
  n::string<char> v(5);
  v.push(1);
  auto item = v.pop();
  N_TEST_ASSERT_TRUE(item.has());
  N_TEST_ASSERT_EQUALS(item.get(), 1);
  N_TEST_ASSERT_TRUE(v.empty());
}

void test_clear() {
  n::string<char> v(5);
  v.push(1);
  v.push(2);
  v.clear();
  N_TEST_ASSERT_TRUE(v.empty());
  N_TEST_ASSERT_EQUALS(v.size(), 0);
}

void test_iterator() {
  n::string<char> v(5);
  v.push(1);
  v.push(2);
  v.push(3);
  int sum = 0;
  for (auto it = v.iter(); it.has_next();) {
    sum += it.next();
  }
  N_TEST_ASSERT_EQUALS(sum, 6);
}

void test_reverse_iterator() {
  n::string<char> v(5);
  v.push(1);
  v.push(2);
  v.push(3);
  int sum = 0;
  for (auto it = v.riter(); it.has_next();) {
    sum += it.next();
  }
  N_TEST_ASSERT_EQUALS(sum, 6);
}
}  // namespace vec

namespace ext {
void test_empty() {
  n::ext_string<char> v(5);
  N_TEST_ASSERT_TRUE(v.empty());
  N_TEST_ASSERT_EQUALS(v.size(), 0);
  N_TEST_ASSERT_EQUALS(v.max(), 5);
}

void test_push_and_size() {
  n::ext_string<char> v(5);
  v.push(1);
  N_TEST_ASSERT_FALSE(v.empty());
  N_TEST_ASSERT_EQUALS(v.size(), 1);
  v.push(2);
  N_TEST_ASSERT_EQUALS(v.size(), 2);
}

void test_push_and_index_operator() {
  n::ext_string<char> v(5);
  v.push(1);
  v.push(2);
  N_TEST_ASSERT_EQUALS(v[0], 1);
  N_TEST_ASSERT_EQUALS(v[1], 2);
}
/* TODO fixer ce test
void test_push_move() {
  ext::ext_string<std::unique_ptr<int>> v(5);
  v.push(std::make_unique<int>(1));
  N_TEST_ASSERT_EQUALS(*v[0], 1);
}*/

void test_pop() {
  n::ext_string<char> v(5);
  v.push(1);
  auto item = v.pop();
  N_TEST_ASSERT_TRUE(item.has());
  N_TEST_ASSERT_EQUALS(item.get(), 1);
  N_TEST_ASSERT_TRUE(v.empty());
}

void test_clear() {
  n::ext_string<char> v(5);
  v.push(1);
  v.push(2);
  v.clear();
  N_TEST_ASSERT_TRUE(v.empty());
  N_TEST_ASSERT_EQUALS(v.size(), 0);
}

void test_iterator() {
  n::ext_string<char> v(5);
  v.push(1);
  v.push(2);
  v.push(3);
  int sum = 0;
  for (auto it = v.iter(); it.has_next();) {
    sum += it.next();
  }
  N_TEST_ASSERT_EQUALS(sum, 6);
}

void test_reverse_iterator() {
  n::ext_string<char> v(5);
  v.push(1);
  v.push(2);
  v.push(3);
  int sum = 0;
  for (auto it = v.riter(); it.has_next();) {
    sum += it.next();
  }
  N_TEST_ASSERT_EQUALS(sum, 6);
}

void test_push_when_full() {
  n::ext_string<char> v(2);
  v.push(1);
  v.push(2);
  v.push(3);
  N_TEST_ASSERT_EQUALS(v.size(), 3);
  N_TEST_ASSERT_EQUALS(v.max(), 14);
}

}  // namespace ext

void limited_string_iterator_test() {
  // Création d'un itérateur sur une chaîne de caractères limitée
  n::string_iterator<const char> str = "Hello, World!";
  n::str::limited_string_iterator<const char> lit(str, 6);

  // Test de la méthode has_next()
  N_TEST_ASSERT_TRUE(lit.has_next());

  // Test de la méthode next()
  N_TEST_ASSERT_EQUALS(lit.next(), 'H');
  N_TEST_ASSERT_EQUALS(lit.next(), 'e');
  N_TEST_ASSERT_EQUALS(lit.next(), 'l');
  N_TEST_ASSERT_EQUALS(lit.next(), 'l');
  N_TEST_ASSERT_EQUALS(lit.next(), 'o');
  N_TEST_ASSERT_EQUALS(lit.next(), ',');

  // Le caractère ' ' est atteint, l'itération s'arrête
  N_TEST_ASSERT_FALSE(lit.has_next());
}

void string_splitter_test() {
  // Création d'un objet string_splitter
  n::string_iterator<const char> str = "Hello, World! h";
  n::str::string_splitter<const char> splitter(str, ' ');

  // Test de la méthode has_next()
  N_TEST_ASSERT_TRUE(splitter.has_next());

  // Test de la méthode next()
  auto lit = splitter.next();
  N_TEST_ASSERT_TRUE(lit.has_next());
  N_TEST_ASSERT_EQUALS(lit.next(), 'H');
  N_TEST_ASSERT_EQUALS(lit.next(), 'e');
  N_TEST_ASSERT_EQUALS(lit.next(), 'l');
  N_TEST_ASSERT_EQUALS(lit.next(), 'l');
  N_TEST_ASSERT_EQUALS(lit.next(), 'o');
  N_TEST_ASSERT_EQUALS(lit.next(), ',');
  N_TEST_ASSERT_FALSE(lit.has_next());

  lit = splitter.next();
  N_TEST_ASSERT_TRUE(lit.has_next());
  N_TEST_ASSERT_EQUALS(lit.next(), 'W');
  N_TEST_ASSERT_EQUALS(lit.next(), 'o');
  N_TEST_ASSERT_EQUALS(lit.next(), 'r');
  N_TEST_ASSERT_EQUALS(lit.next(), 'l');
  N_TEST_ASSERT_EQUALS(lit.next(), 'd');
  N_TEST_ASSERT_EQUALS(lit.next(), '!')
  N_TEST_ASSERT_FALSE(lit.has_next());

  lit = splitter.next();
  N_TEST_ASSERT_EQUALS(lit.next(), 'h');
  N_TEST_ASSERT_FALSE(lit.has_next());

  // Il n'y a plus de parties de la chaîne à splitter
  N_TEST_ASSERT_FALSE(splitter.has_next());
}

int main() {
  N_TEST_SUITE("StaticString Tests")
  N_TEST_REGISTER(test_empty);
  N_TEST_REGISTER(test_push_and_size);
  N_TEST_REGISTER(test_push_and_index_operator);
  // TODO: Fixer ce test N_TEST_REGISTER(test_push_move);
  N_TEST_REGISTER(test_pop);
  N_TEST_REGISTER(test_clear);
  N_TEST_REGISTER(test_iterator);
  N_TEST_REGISTER(test_reverse_iterator);
  N_TEST_RUN_SUITE

  N_TEST_SUITE("String Tests")
  N_TEST_REGISTER(vec::test_empty);
  N_TEST_REGISTER(vec::test_push_and_size);
  N_TEST_REGISTER(vec::test_push_and_index_operator);
  // TODO fixer ce test N_TEST_REGISTER(vec::test_push_move);
  N_TEST_REGISTER(vec::test_pop);
  N_TEST_REGISTER(vec::test_clear);
  N_TEST_REGISTER(vec::test_iterator);
  N_TEST_REGISTER(vec::test_reverse_iterator);
  N_TEST_RUN_SUITE

  N_TEST_SUITE("ExtString Tests")
  N_TEST_REGISTER(ext::test_empty);
  N_TEST_REGISTER(ext::test_push_and_size);
  N_TEST_REGISTER(ext::test_push_and_index_operator);
  // TODO fixer ce test N_TEST_REGISTER(test_push_move);
  N_TEST_REGISTER(ext::test_pop);
  N_TEST_REGISTER(ext::test_clear);
  N_TEST_REGISTER(ext::test_iterator);
  N_TEST_REGISTER(ext::test_reverse_iterator);
  N_TEST_REGISTER(ext::test_push_when_full);
  N_TEST_RUN_SUITE

  N_TEST_SUITE("String Splitter Tests")
  N_TEST_REGISTER(limited_string_iterator_test)
  N_TEST_REGISTER(string_splitter_test)
  N_TEST_RUN_SUITE
}
