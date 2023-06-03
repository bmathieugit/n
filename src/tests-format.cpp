#include <stdio.h>

#include <n/format.hpp>
#include <n/iterator.hpp>
#include <n/tests.hpp>

bool operator==(const n::string<char>& s, const char* s1) {
  return n::equal(s.iter(), n::pointer_iterator(s1, strlen(s1)));
}

void test_format_string() {
  n::string<char> result = n::format("Hello, $!", "world");
  N_TEST_ASSERT_EQUALS(result, "Hello, world!");

  result = n::format("The answer is $", 42);
  N_TEST_ASSERT_EQUALS(result, "The answer is 42");

  result = n::format("$ is $ years old", "John", 30);

  N_TEST_ASSERT_EQUALS(result, "John is 30 years old");
}

void test_format_to_string() {
  n::string<char> str;
  n::format_to(str, "Hello, $!", "world");
  N_TEST_ASSERT_EQUALS(str, "Hello, world!");

  n::format_to(str, "The answer is $", 42);
  N_TEST_ASSERT_EQUALS(str, "Hello, world!The answer is 42");

  n::format_to(str, "$ is $ years old", "John", 30);
  N_TEST_ASSERT_EQUALS(str,
                       "Hello, world!The answer is 42John is 30 years old");
}

void test_format_signed_integral() {
  n::string<char> result = n::format("$", -42);
  N_TEST_ASSERT_EQUALS(result, "-42");

  result = n::format("$", 0);
  N_TEST_ASSERT_EQUALS(result, "0");

  result = n::format("$", 123456789);
  N_TEST_ASSERT_EQUALS(result, "123456789");
}

void test_format_unsigned_integral() {
  n::string<char> result = n::format("$", 42u);
  N_TEST_ASSERT_EQUALS(result, "42");

  result = n::format("$", 0u);
  N_TEST_ASSERT_EQUALS(result, "0");

  result = n::format("$", 123456789u);
  N_TEST_ASSERT_EQUALS(result, "123456789");
}

void test_format_bool() {
  n::string<char> result = n::format("$", true);
  N_TEST_ASSERT_EQUALS(result, "true");

  result = n::format("$", false);
  N_TEST_ASSERT_EQUALS(result, "false");
}

int main() {
  N_TEST_SUITE("Format Library Tests")
  N_TEST_REGISTER(test_format_string)
  N_TEST_REGISTER(test_format_to_string)
  N_TEST_REGISTER(test_format_signed_integral)
  N_TEST_REGISTER(test_format_unsigned_integral)
  N_TEST_REGISTER(test_format_bool)
  N_TEST_RUN_SUITE
}
