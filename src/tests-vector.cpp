#include <n/tests.hpp>
#include <n/vector.hpp>

void test_vector_creation() {
  n::vector<int> v;
  N_TEST_ASSERT_TRUE(v.empty());
  N_TEST_ASSERT_EQUALS(v.len(), 0);
}

void test_vector_push_pop() {
  n::vector<int> v;
  v.push(5);
  N_TEST_ASSERT_FALSE(v.empty());
  N_TEST_ASSERT_EQUALS(v.len(), 1);
  N_TEST_ASSERT_EQUALS(v.pop(), 5);
  N_TEST_ASSERT_TRUE(v.empty());
}

void test_vector_copy() {
  n::vector<int> v;
  v.push(5);
  n::vector<int> v_copy = v;
  N_TEST_ASSERT_FALSE(v_copy.empty());
  N_TEST_ASSERT_EQUALS(v_copy.len(), 1);
  N_TEST_ASSERT_EQUALS(v_copy.pop(), 5);
}

void test_vector_move() {
  n::vector<int> v;
  v.push(5);
  n::vector<int> v_moved = n::move(v);
  N_TEST_ASSERT_TRUE(v.empty());
  N_TEST_ASSERT_FALSE(v_moved.empty());
  N_TEST_ASSERT_EQUALS(v_moved.len(), 1);
  N_TEST_ASSERT_EQUALS(v_moved.pop(), 5);
}

void test_vector_resize() {
  n::vector<int> v(1);
  v.push(5);
  N_TEST_ASSERT_EQUALS(v.len(), 1);
  v.push(6);
  N_TEST_ASSERT_EQUALS(v.len(), 2);
}

void test_vector_clear() {
  n::vector<int> v;
  v.push(5);
  v.push(6);
  N_TEST_ASSERT_EQUALS(v.len(), 2);
  v.clear();
  N_TEST_ASSERT_TRUE(v.empty());
  N_TEST_ASSERT_EQUALS(v.len(), 0);
}

void test_vector_iterator() {
  n::vector<int> v;
  v.push(5);
  v.push(6);
  v.push(7);
  int sum = 0;
  for (auto iter = v.iter(); iter.has_next();) {
    sum += iter.next();
  }
  N_TEST_ASSERT_EQUALS(sum, 18);
}

void test_vector_const_iterator() {
  n::vector<int> v;
  v.push(5);
  v.push(6);
  v.push(7);
  int sum = 0;
  for (auto iter = v.iter(); iter.has_next();) {
    sum += iter.next();
  }
  N_TEST_ASSERT_EQUALS(sum, 18);
}

void test_clear() {
  n::vector<int> v;
  v.push(1);
  v.push(2);
  v.push(3);

  N_TEST_ASSERT_EQUALS(v.len(), 3);

  v.clear();

  N_TEST_ASSERT_EQUALS(v.len(), 0);
}

int main() {
  N_TEST_SUITE("n::vector tests");
  N_TEST_REGISTER(test_vector_creation);
  N_TEST_REGISTER(test_vector_push_pop);
  N_TEST_REGISTER(test_vector_copy);
  N_TEST_REGISTER(test_vector_move);
  N_TEST_REGISTER(test_vector_resize);
  N_TEST_REGISTER(test_vector_clear);
  N_TEST_REGISTER(test_vector_iterator);
  N_TEST_REGISTER(test_vector_const_iterator);
  N_TEST_REGISTER(test_vector_clear);

  N_TEST_RUN_SUITE
}
