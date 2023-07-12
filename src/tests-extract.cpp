#include <n/extract.hpp>
#include <n/tests.hpp>

#include "n/extract-core.hpp"

bool operator==(const n::string<char>& s, const char* s1) {
  return n::equal(s.iter(), n::pointer_iterator(s1, strlen(s1)));
}

void test_extract_char() {
  n::maybe<char> mc;
  n::extract(n::str("input").iter(), "$", mc);
  N_TEST_ASSERT_TRUE(mc.has());
  N_TEST_ASSERT_EQUALS(mc.get(), 'i');
}

// Test extract for string
void test_extract_string() {
  n::maybe<n::string<char>> ms;
  n::extract(n::str("\"input\"").iter(), "$", ms);
  N_TEST_ASSERT_TRUE(ms.has());
  N_TEST_ASSERT_EQUALS(ms.get(), "input");
}

// Test extract for unsigned integral
void test_extract_unsigned_integral() {
  n::maybe<unsigned int> mui;
  auto res = n::extract(n::str("123").iter(), "$", mui);
  N_TEST_ASSERT_EQUALS(res, n::extract_rc::ok);
  N_TEST_ASSERT_TRUE(mui.has());
  N_TEST_ASSERT_EQUALS(mui.get(), 123u);
}

// Test extract for signed integral
void test_extract_signed_integral() {
  n::maybe<int> mi;
  n::extract(n::str("-456").iter(), "$", mi);
  N_TEST_ASSERT_TRUE(mi.has());
  N_TEST_ASSERT_EQUALS(mi.get(), -456);
}

// Test extract for bool
void test_extract_bool() {
  n::maybe<bool> mb;
  n::extract(n::str("true").iter(), "$", mb);
  N_TEST_ASSERT_TRUE(mb.has());
  N_TEST_ASSERT_TRUE(mb.get());

  n::extract(n::str("false").iter(), "$", mb);
  N_TEST_ASSERT_TRUE(mb.has());
  N_TEST_ASSERT_FALSE(mb.get());
}

void test_extract_specific_pattern1() {
  n::maybe<int> age;
  auto res=  n::extract(n::str("j'ai 12 ans").iter(), "j'ai $ ans", age);
  
  N_TEST_ASSERT_EQUALS(res, n::extract_rc::ok);
  N_TEST_ASSERT_TRUE(age.has());
  N_TEST_ASSERT_EQUALS(age.get(), 12);
}

void test_extract_specific_pattern2() {
  n::maybe<int> age;
  n::maybe<n::string<char>> s;
  auto res = n::extract(n::str("j'ai 12 ans et \"demi\"!").iter(),
                        "j'ai $ ans et $!", age, s);
  N_TEST_ASSERT_EQUALS(res, n::extract_rc::ok);
  N_TEST_ASSERT_TRUE(age.has());
  N_TEST_ASSERT_EQUALS(age.get(), 12);

  N_TEST_ASSERT_TRUE(s.has());
  N_TEST_ASSERT_EQUALS(s.get(), "demi");
}

void test_extract_specific_pattern3() {
  n::maybe<int> age;
  n::maybe<n::string<char>> name;
  auto res = n::extract(n::str("j'ai 12 ans, je m'appelle \"Bob\" !!").iter(),
                        "j'ai $ ans, je m'appelle $ !!", age, name);
  printf("%d\n", (int)res);

  N_TEST_ASSERT_EQUALS(res, n::extract_rc::ok);

  N_TEST_ASSERT_TRUE(age.has());
  N_TEST_ASSERT_EQUALS(age.get(), 12);

  N_TEST_ASSERT_TRUE(name.has());
  N_TEST_ASSERT_EQUALS(name.get(), "Bob");
}

int main() {
  N_TEST_SUITE("n_extract.hpp Tests")
  N_TEST_REGISTER(test_extract_char)
  N_TEST_REGISTER(test_extract_string)
  N_TEST_REGISTER(test_extract_unsigned_integral)
  N_TEST_REGISTER(test_extract_signed_integral)
  N_TEST_REGISTER(test_extract_bool)
  N_TEST_REGISTER(test_extract_specific_pattern1)
  N_TEST_REGISTER(test_extract_specific_pattern2)
  N_TEST_REGISTER(test_extract_specific_pattern3)
  N_TEST_RUN_SUITE
}
