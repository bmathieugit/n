#include <n/string.hpp>
#include <n/tests.hpp>

void test_string_create() {
  auto s = n::str("Hello");
  N_TEST_ASSERT_EQUALS(s.len(), 5);
}

void test_string_copy_constructor() {
  auto s = n::str("Hello");
  n::string<char> s2 = s;
  N_TEST_ASSERT_EQUALS(s2.len(), 5);
}

void test_string_move_constructor() {
  auto  s = n::str("Hello");
  n::string<char> s3 = n::move(s);
  N_TEST_ASSERT_EQUALS(s3.len(), 5);
}

void test_string_copy_assignment() {
  auto s = n::str("Hello");
  n::string<char> s2;
  s2 = s;
  N_TEST_ASSERT_EQUALS(s2.len(), 5);
}

void test_string_move_assignment() {
  auto  s = n::str("Hello");
  n::string<char> s3;
  s3 = n::move(s);
  N_TEST_ASSERT_EQUALS(s3.len(), 5);
}

void test_string_len() {
  auto s = n::str("Hello");
  N_TEST_ASSERT_EQUALS(s.len(), 5);
}

void test_string_empty() {
  n::string<char> s;
  N_TEST_ASSERT_TRUE(s.empty());
}

void test_string_full() {
  auto s=n::str("Hello");
  N_TEST_ASSERT_FALSE(s.full());
}

void test_string_push() {
  n::string<char> s;
  s.push('H');
  s.push('i');
  N_TEST_ASSERT_EQUALS(s.len(), 2);
}

void test_string_pop() {
  auto s = n::str("Hi");
  char last_char = s.pop().get();
  N_TEST_ASSERT_EQUALS(last_char, 'i');
  N_TEST_ASSERT_EQUALS(s.len(), 1);
}

void test_string_clear() {
  auto s=n::str("Clear me");
  s.clear();
  N_TEST_ASSERT_TRUE(s.empty());
}

int main() {
  N_TEST_SUITE("N String Test Suite")
  N_TEST_REGISTER(test_string_create)
  N_TEST_REGISTER(test_string_copy_constructor)
  N_TEST_REGISTER(test_string_move_constructor)
  N_TEST_REGISTER(test_string_copy_assignment)
  N_TEST_REGISTER(test_string_move_assignment)
  N_TEST_REGISTER(test_string_len)
  N_TEST_REGISTER(test_string_empty)
  N_TEST_REGISTER(test_string_full)
  N_TEST_REGISTER(test_string_push)
  N_TEST_REGISTER(test_string_pop)
  N_TEST_REGISTER(test_string_clear)
  N_TEST_RUN_SUITE
  return 0;
}
