#include <n/algorithm.hpp>
#include <n/format.hpp>
#include <n/tests.hpp>



void test_char_formatter() {
  n::static_string<char, 10> ss;
  n::format_to(ss, "abc $", 'x');
  N_TEST_ASSERT_TRUE(
      n::equals(ss.iter(), n::string_iterator<const char>("abc x")));
}

void test_signed_int_formatter() {
  n::static_string<char, 10> ss;
  n::format_to(ss, "abc $", -123);
  N_TEST_ASSERT_TRUE(
      n::equals(ss.iter(), n::string_iterator<const char>("abc -123")));
}

void test_unsigned_int_formatter() {
  n::static_string<char, 10> ss;
  n::format_to(ss, "abc $", 123u);
  N_TEST_ASSERT_TRUE(
      n::equals(ss.iter(), n::string_iterator<const char>("abc 123")));
}

void test_bool_formatter() {
  n::static_string<char, 10> ss;
  n::format_to(ss, "abc $", true);
  N_TEST_ASSERT_TRUE(
      n::equals(ss.iter(), n::string_iterator<const char>("abc true")));
}

void test_iterator_formatter() {
  n::string_iterator<const char> vec("abc");
  n::static_string<char, 10> ss;
  n::format_to(ss, "abc $", vec);
  N_TEST_ASSERT_TRUE(
      n::equals(ss.iter(), n::string_iterator<const char>("abc abc")));
}

void test_string_view_formatter() {
  n::static_string<char, 10> ss;
  n::format_to(ss, "abc $", n::string_iterator<const char>("def"));
  N_TEST_ASSERT_TRUE(
      n::equals(ss.iter(), n::string_iterator<const char>("abc def")));
}

void test_pointer_formatter() {
  int* x = nullptr;
  n::static_string<char, 10> ss;
  n::format_to(ss, "abc $", x);
  N_TEST_ASSERT_TRUE(
      n::equals(ss.iter(), n::string_iterator<const char>("abc 0")));
}

int main() {
  N_TEST_SUITE("Formatter Tests")
  N_TEST_REGISTER(test_char_formatter);
  N_TEST_REGISTER(test_signed_int_formatter);
  N_TEST_REGISTER(test_unsigned_int_formatter);
  N_TEST_REGISTER(test_bool_formatter);
  N_TEST_REGISTER(test_iterator_formatter);
  N_TEST_REGISTER(test_string_view_formatter);
  N_TEST_REGISTER(test_pointer_formatter);
  N_TEST_RUN_SUITE
}
