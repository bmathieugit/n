#include <n/tests.hpp>
#include <n/vector.hpp>

void test_empty() {
  n::static_vector<int, 5> v;
  N_TEST_ASSERT_TRUE(v.empty());
  N_TEST_ASSERT_EQUALS(v.size(), 0);
  N_TEST_ASSERT_EQUALS(v.max(), 5);
}

void test_push_and_size() {
  n::static_vector<int, 5> v;
  v.push(1);
  N_TEST_ASSERT_FALSE(v.empty());
  N_TEST_ASSERT_EQUALS(v.size(), 1);
  v.push(2);
  N_TEST_ASSERT_EQUALS(v.size(), 2);
}

void test_push_and_index_operator() {
  n::static_vector<int, 5> v;
  v.push(1);
  v.push(2);
  N_TEST_ASSERT_EQUALS(v[0], 1);
  N_TEST_ASSERT_EQUALS(v[1], 2);
}

/*
TODO: fixer ce test
   void test_push_move() {
  n::static_vector<std::unique_ptr<int>, 5> v;
  v.push(std::make_unique<int>(1));
  N_TEST_ASSERT_EQUALS(*v[0], 1);
}*/

void test_pop() {
  n::static_vector<int, 5> v;
  v.push(1);
  auto item = v.pop();
  N_TEST_ASSERT_TRUE(item.has());
  auto i = item.get();
  N_TEST_ASSERT_EQUALS(i, 1);
  N_TEST_ASSERT_TRUE(v.empty());
}

void test_clear() {
  n::static_vector<int, 5> v;
  v.push(1);
  v.push(2);
  v.clear();
  N_TEST_ASSERT_TRUE(v.empty());
  N_TEST_ASSERT_EQUALS(v.size(), 0);
}

void test_iterator() {
  n::static_vector<int, 5> v;
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
  n::static_vector<int, 5> v;
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
  n::vector<int> v(5);
  N_TEST_ASSERT_TRUE(v.empty());
  N_TEST_ASSERT_EQUALS(v.size(), 0);
  N_TEST_ASSERT_EQUALS(v.max(), 5);
}

void test_push_and_size() {
  n::vector<int> v(5);
  v.push(1);
  N_TEST_ASSERT_FALSE(v.empty());
  N_TEST_ASSERT_EQUALS(v.size(), 1);
  v.push(2);
  N_TEST_ASSERT_EQUALS(v.size(), 2);
}

void test_push_and_index_operator() {
  n::vector<int> v(5);
  v.push(1);
  v.push(2);
  N_TEST_ASSERT_EQUALS(v[0], 1);
  N_TEST_ASSERT_EQUALS(v[1], 2);
}

/*
  TODO: fixer ce test
   void test_push_move() {
  n::vector<std::unique_ptr<int>> v(5);
  v.push(std::make_unique<int>(1));
  N_TEST_ASSERT_EQUALS(*v[0], 1);
}
*/
void test_pop() {
  n::vector<int> v(5);
  v.push(1);
  auto item = v.pop();
  N_TEST_ASSERT_TRUE(item.has());
  N_TEST_ASSERT_EQUALS(item.get(), 1);
  N_TEST_ASSERT_TRUE(v.empty());
}

void test_clear() {
  n::vector<int> v(5);
  v.push(1);
  v.push(2);
  v.clear();
  N_TEST_ASSERT_TRUE(v.empty());
  N_TEST_ASSERT_EQUALS(v.size(), 0);
}

void test_iterator() {
  n::vector<int> v(5);
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
  n::vector<int> v(5);
  v.push(1);
  v.push(2);
  v.push(3);
  int sum = 0;
  for (auto it = v.riter(); it.has_next();) {
    sum += it.next();
  }
  N_TEST_ASSERT_EQUALS(sum, 6);
}
}
int main() {
  N_TEST_SUITE("StaticVector Tests")
  N_TEST_REGISTER(test_empty);
  N_TEST_REGISTER(test_push_and_size);
  N_TEST_REGISTER(test_push_and_index_operator);
  // TODO: Fixer ce test N_TEST_REGISTER(test_push_move);
  N_TEST_REGISTER(test_pop);
  N_TEST_REGISTER(test_clear);
  N_TEST_REGISTER(test_iterator);
  N_TEST_REGISTER(test_reverse_iterator);
  N_TEST_RUN_SUITE

  N_TEST_SUITE("Vector Tests")
  N_TEST_REGISTER(vec::test_empty);
  N_TEST_REGISTER(vec::test_push_and_size);
  N_TEST_REGISTER(vec::test_push_and_index_operator);
  // TODO fixer ce test N_TEST_REGISTER(vec::test_push_move);
  N_TEST_REGISTER(vec::test_pop);
  N_TEST_REGISTER(vec::test_clear);
  N_TEST_REGISTER(vec::test_iterator);
  N_TEST_REGISTER(vec::test_reverse_iterator);
  N_TEST_RUN_SUITE
}
