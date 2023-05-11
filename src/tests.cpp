#include <assert.h>
#include <stdio.h>

#include <n/algorithm.hpp>
#include <n/maybe.hpp>
#include <n/string.hpp>
#include <n/utils.hpp>
#include <n/vector.hpp>

#include "n/format.hpp"

namespace test::algo {
void should_find_an_item() {
  n::static_vector<int, 3> v;
  v.push(1);
  v.push(2);
  v.push(3);
  auto i = n::find(v.iter(), [](int i) { return i == 2; });
  assert(i == 1);
}

void should_apply_function_foreach_item() {
  n::static_vector<int, 3> v;
  v.push(1);
  v.push(2);
  v.push(3);

  n::for_each(v.iter(), [](int& i) { i += 2; });

  auto j = 3;
  auto i = v.iter();

  while (i.has_next()) {
    assert(i.next() == j);
    ++j;
  }
}
}  // namespace test::algo

namespace test::static_vector {
void should_be_empty() {
  n::static_vector<int, 10> emptyv;
  assert(emptyv.empty());
}

void should_contain_one_item() {
  n::static_vector<int, 10> one;
  one.push(1);
  assert(one.size() == 1);
}

void should_contain_three_item() {
  n::static_vector<int, 10> three;
  three.push(0);
  three.push(1);
  three.push(2);
  assert(three.size() == 3);
}

void should_be_full() {
  n::static_vector<int, 4> four;
  four.push(0);
  four.push(0);
  four.push(0);
  four.push(0);
  assert(four.full());
}

void should_be_popable() {
  n::static_vector<int, 4> four;
  four.push(0);
  four.push(2);
  assert(four.size() == 2);
  four.pop();
  assert(four.size() == 1);
}

void should_be_iterable() {
  n::static_vector<int, 4> four;
  four.push(1);
  four.push(2);
  four.push(3);
  four.push(4);

  int expected = 1;
  auto ifour = four.iter();

  while (ifour.has_next()) {
    assert(ifour.next() == expected);
    expected++;
  }
}

void should_be_reverse_iterable() {
  n::static_vector<int, 4> four;
  four.push(1);
  four.push(2);
  four.push(3);
  four.push(4);

  int expected = 4;
  auto ifour = four.riter();

  while (ifour.has_next()) {
    assert(ifour.next() == expected);
    expected--;
  }
}

}  // namespace test::static_vector

namespace test::vector {
void should_be_empty() {
  n::vector<int> emptyv;
  assert(emptyv.empty());
}

void should_contain_one_item() {
  n::vector<int> one(10);
  one.push(1);
  assert(one.size() == 1);
}

void should_contain_three_item() {
  n::vector<int> three(10);
  three.push(0);
  three.push(1);
  three.push(2);
  assert(three.size() == 3);
}

void should_be_full() {
  n::vector<int> four(4);
  four.push(0);
  four.push(0);
  four.push(0);
  four.push(0);
  assert(four.full());
}

void should_be_popable() {
  n::vector<int> four(4);
  four.push(0);
  four.push(2);
  assert(four.size() == 2);
  four.pop();
  assert(four.size() == 1);
}

void should_be_iterable() {
  n::vector<int> four(4);
  four.push(1);
  four.push(2);
  four.push(3);
  four.push(4);

  int expected = 1;
  auto ifour = four.iter();

  while (ifour.has_next()) {
    assert(ifour.next() == expected);
    expected++;
  }
}

void should_be_reverse_iterable() {
  n::vector<int> four(4);
  four.push(1);
  four.push(2);
  four.push(3);
  four.push(4);

  int expected = 4;
  auto ifour = four.riter();

  while (ifour.has_next()) {
    assert(ifour.next() == expected);
    expected--;
  }
}
}  // namespace test::vector

namespace test::ext_vector {
void should_be_empty() {
  n::ext_vector<int> emptyv;
  assert(emptyv.empty());
}

void should_contain_one_item() {
  n::ext_vector<int> one;
  one.push(1);
  assert(one.size() == 1);
}

void should_contain_three_item() {
  n::ext_vector<int> three;
  three.push(0);
  three.push(1);
  three.push(2);
  assert(three.size() == 3);
}

void should_be_full() {
  n::ext_vector<int> four(4);
  four.push(0);
  four.push(0);
  four.push(0);
  four.push(0);
  assert(four.full());
}

void should_be_popable() {
  n::ext_vector<int> four;
  four.push(0);
  four.push(2);
  assert(four.size() == 2);
  four.pop();
  assert(four.size() == 1);
}

void should_be_iterable() {
  n::ext_vector<int> four;
  four.push(1);
  four.push(2);
  four.push(3);
  four.push(4);

  int expected = 1;
  auto ifour = four.iter();

  while (ifour.has_next()) {
    assert(ifour.next() == expected);
    expected++;
  }
}

void should_be_reverse_iterable() {
  n::ext_vector<int> four;
  four.push(1);
  four.push(2);
  four.push(3);
  four.push(4);

  int expected = 4;
  auto ifour = four.riter();

  while (ifour.has_next()) {
    assert(ifour.next() == expected);
    expected--;
  }
}
}  // namespace test::ext_vector

namespace test::static_string {
void should_be_empty() {
  n::static_string<char, 4> four;
  assert(four.empty());
}

void should_be_full() {
  n::static_string<char, 4> four;
  four.push('a');
  four.push('a');
  four.push('a');
  four.push('a');
  assert(four.size() == 4);
  assert(four.full());
}

void should_have_three_items() {
  n::static_string<char, 4> four;
  four.push('a');
  four.push('a');
  four.push('a');
  assert(four.size() == 3);
}

void should_be_popable() {
  n::static_string<char, 4> four;
  four.push('a');
  four.push('a');
  assert(four.size() == 2);
  four.pop();
  assert(four.size() == 1);
  four.pop();
  assert(four.size() == 0);
  assert(four.empty());
}

void should_not_overflow_on_push() {
  n::static_string<char, 3> s;
  s.push('c');
  s.push('c');
  s.push('c');
  assert(s.size() == 3);
  assert(s.full());
  s.push('c');
  assert(s.full());
  assert(s.size() == 3);
}
}  // namespace test::static_string

namespace test::string {
void should_be_empty() {
  n::string<char> four;
  assert(four.empty());
}

void should_be_full() {
  n::string<char> four(4);
  four.push('a');
  four.push('a');
  four.push('a');
  four.push('a');
  assert(four.size() == 4);
  assert(four.full());
}

void should_have_three_items() {
  n::string<char> four(4);
  four.push('a');
  four.push('a');
  four.push('a');
  assert(four.size() == 3);
}

void should_be_popable() {
  n::string<char> four(4);
  four.push('a');
  four.push('a');
  assert(four.size() == 2);
  four.pop();
  assert(four.size() == 1);
  four.pop();
  assert(four.size() == 0);
  assert(four.empty());
}

void should_not_overflow_on_push() {
  n::string<char> s(3);
  s.push('c');
  s.push('c');
  s.push('c');
  assert(s.size() == 3);
  assert(s.full());
  s.push('c');
  assert(s.full());
  assert(s.size() == 3);
}
}  // namespace test::string

namespace test::ext_string {
void should_be_empty() {
  n::ext_string<char> four;
  assert(four.empty());
}

void should_be_full() {
  n::ext_string<char> four(4);
  four.push('a');
  four.push('a');
  four.push('a');
  four.push('a');
  assert(four.size() == 4);
  assert(four.full());
}

void should_have_three_items() {
  n::ext_string<char> four(4);
  four.push('a');
  four.push('a');
  four.push('a');
  assert(four.size() == 3);
}

void should_be_popable() {
  n::ext_string<char> four(4);
  four.push('a');
  four.push('a');
  assert(four.size() == 2);
  four.pop();
  assert(four.size() == 1);
  four.pop();
  assert(four.size() == 0);
  assert(four.empty());
}

void should_not_overflow_on_push() {
  n::ext_string<char> s(3);
  s.push('c');
  s.push('c');
  s.push('c');
  assert(s.size() == 3);
  assert(s.full());
  s.push('c');
  assert(!s.full());
  assert(s.size() == 4);
}
}  // namespace test::ext_string

#include <n/tests.hpp>

namespace test::maybe {
void should_be_empty() {
  n::maybe<int> empty;
  N_TEST_ASSERT_FALSE(empty.has());
}

void should_be_valued() {
  n::maybe<int> valued = 1;
  N_TEST_ASSERT_TRUE(valued.has());
  N_TEST_ASSERT_EQUALS(valued.get(), 1);
}

void should_map_new_value() {
  n::maybe<int> init = 2;
  auto mapped = init.map([](const auto& i) { return i * 2; });
  N_TEST_ASSERT_TRUE(mapped.has());
  N_TEST_ASSERT_EQUALS(mapped.get(), 4);
}
}  // namespace test::maybe

int main() {

  N_TEST_SUITE("suite de test pour la classe maybe<T>");
  N_TEST_REGISTER(test::maybe::should_be_empty);
  N_TEST_REGISTER(test::maybe::should_be_valued);
  N_TEST_REGISTER(test::maybe::should_map_new_value);
  N_TEST_RUN_SUITE;

  test::algo::should_find_an_item();
  test::algo::should_apply_function_foreach_item();
  test::maybe::should_be_empty();
  test::maybe::should_be_valued();
  test::maybe::should_map_new_value();

  test::static_vector::should_be_empty();
  test::static_vector::should_contain_one_item();
  test::static_vector::should_contain_three_item();
  test::static_vector::should_be_full();
  test::static_vector::should_be_popable();
  test::static_vector::should_be_iterable();
  test::static_vector::should_be_reverse_iterable();

  test::vector::should_be_empty();
  test::vector::should_contain_one_item();
  test::vector::should_contain_three_item();
  test::vector::should_be_full();
  test::vector::should_be_popable();
  test::vector::should_be_iterable();
  test::vector::should_be_reverse_iterable();

  test::ext_vector::should_be_empty();
  test::ext_vector::should_contain_one_item();
  test::ext_vector::should_contain_three_item();
  test::ext_vector::should_be_full();
  test::ext_vector::should_be_popable();
  test::ext_vector::should_be_iterable();
  test::ext_vector::should_be_reverse_iterable();

  test::static_string::should_be_empty();
  test::static_string::should_be_full();
  test::static_string::should_have_three_items();
  test::static_string::should_be_popable();
  test::static_string::should_not_overflow_on_push();

  test::string::should_be_empty();
  test::string::should_be_full();
  test::string::should_have_three_items();
  test::string::should_be_popable();
  test::string::should_not_overflow_on_push();

  test::ext_string::should_be_empty();
  test::ext_string::should_be_full();
  test::ext_string::should_have_three_items();
  test::ext_string::should_be_popable();
  test::ext_string::should_not_overflow_on_push();
  
}
