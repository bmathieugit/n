#include <n/algorithm.hpp>
#include <n/format.hpp>
#include <n/iofile.hpp>
#include <n/tests.hpp>

void test_char_formatter() {
  n::ext_string<char> ss;
  n::format_to(ss, "abc $", 'x');
  N_TEST_ASSERT_TRUE(
      n::equals(ss.iter(), n::string_view<char>("abc x").iter()));
}

void test_signed_int_formatter() {
  n::ext_string<char> ss;
  n::format_to(ss, "abc $", -123);
  N_TEST_ASSERT_TRUE(
      n::equals(ss.iter(), n::string_view<char>("abc -123").iter()));
}

void test_unsigned_int_formatter() {
  n::ext_string<char> ss;
  n::format_to(ss, "abc $", 123u);
  N_TEST_ASSERT_TRUE(
      n::equals(ss.iter(), n::string_view<char>("abc 123").iter()));
}

void test_bool_formatter() {
  n::ext_string<char> ss;
  n::format_to(ss, "abc $", true);
  N_TEST_ASSERT_TRUE(
      n::equals(ss.iter(), n::string_view<char>("abc true").iter()));
}

void test_iterator_formatter() {
  n::string_view<char> vec("abc");
  n::ext_string<char> ss;
  n::format_to(ss, "abc $", vec.iter());
  N_TEST_ASSERT_TRUE(
      n::equals(ss.iter(), n::string_view<char>("abc abc").iter()));
}

void test_string_view_formatter() {
  n::ext_string<char> ss;
  n::format_to(ss, "abc $", n::string_view<char>("def"));
  N_TEST_ASSERT_TRUE(
      n::equals(ss.iter(), n::string_view<char>("abc def").iter()));
}

void test_pointer_formatter() {
  int* x = nullptr;
  n::ext_string<char> ss;
  n::format_to(ss, "abc $", x);
  N_TEST_ASSERT_TRUE(
      n::equals(ss.iter(), n::string_view<char>("abc 0").iter()));
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
