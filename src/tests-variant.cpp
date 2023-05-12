#include <n/variant.hpp>
#include <n/tests.hpp>
#include <string>

void test_variant_default_constructor() {
  n::variant<int, double, std::string> v;
  N_TEST_ASSERT_TRUE(v.empty());
}

void test_variant_set_int() {
  n::variant<int, double, std::string> v;
  v.set(42);
  N_TEST_ASSERT_FALSE(v.empty());
  N_TEST_ASSERT_EQUALS(v.get<int>(), 42);
}

void test_variant_set_double() {
  n::variant<int, double, std::string> v;
  v.set(42.5);
  N_TEST_ASSERT_FALSE(v.empty());
  N_TEST_ASSERT_EQUALS(v.get<double>(), 42.5);
}

void test_variant_set_string() {
  n::variant<int, double, std::string> v;
  v.set(std::string("test"));
  N_TEST_ASSERT_FALSE(v.empty());
  N_TEST_ASSERT_EQUALS(v.get<std::string>(), std::string("test"));
}

void test_variant_copy_constructor() {
  n::variant<int, double, std::string> v1;
  v1.set(42);
  n::variant<int, double, std::string> v2(v1);
  N_TEST_ASSERT_FALSE(v2.empty());
  N_TEST_ASSERT_EQUALS(v2.get<int>(), 42);
}

void test_variant_copy_assignment() {
  n::variant<int, double, std::string> v1;
  v1.set(42);
  n::variant<int, double, std::string> v2;
  v2 = v1;
  N_TEST_ASSERT_FALSE(v2.empty());
  N_TEST_ASSERT_EQUALS(v2.get<int>(), 42);
}

void test_variant_move_constructor() {
  n::variant<int, double, std::string> v1;
  v1.set(42);
  n::variant<int, double, std::string> v2(std::move(v1));
  N_TEST_ASSERT_FALSE(v2.empty());
  N_TEST_ASSERT_EQUALS(v2.get<int>(), 42);
}

void test_variant_move_assignment() {
  n::variant<int, double, std::string> v1;
  v1.set(42);
  n::variant<int, double, std::string> v2;
  v2 = std::move(v1);
  N_TEST_ASSERT_FALSE(v2.empty());
  N_TEST_ASSERT_EQUALS(v2.get<int>(), 42);
}

void test_variant_index() {
  n::variant<int, double, std::string> v;
  v.set(42.5);
  N_TEST_ASSERT_EQUALS(v.index(), 1);
}
int main()
{
  N_TEST_SUITE("Variant Tests")
{
  N_TEST_REGISTER(test_variant_default_constructor);
  N_TEST_REGISTER(test_variant_set_int);
  N_TEST_REGISTER(test_variant_set_double);
  N_TEST_REGISTER(test_variant_set_string);
  N_TEST_REGISTER(test_variant_copy_constructor);
  N_TEST_REGISTER(test_variant_copy_assignment);
  N_TEST_REGISTER(test_variant_move_constructor);
  N_TEST_REGISTER(test_variant_move_assignment);
  N_TEST_REGISTER(test_variant_index);
}
N_TEST_RUN_SUITE
}