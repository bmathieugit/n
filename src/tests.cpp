#include <assert.h>
#include <stdio.h>

#include <n/maybe.hpp>

#include "n/utils.hpp"
#include "n/vector.hpp"

namespace test::maybe {
void should_be_empty() {
  n::maybe<int> empty;
  assert(!empty.has());
}

void should_be_valued() {
  n::maybe<int> valued = 1;
  assert(valued.has());
  assert(valued.get() == 1);
}

void should_map_new_value() {
  n::maybe<int> init = 2;
  auto mapped = init.map([](const auto& i) { return i * 2; });
  assert(mapped.has());
  assert(mapped.get() == 4);
}
}  // namespace test::maybe

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

}  // namespace test::static_vector

int main() {
  test::maybe::should_be_empty();
  test::maybe::should_be_valued();
  test::maybe::should_map_new_value();

  test::static_vector::should_be_empty();
  test::static_vector::should_contain_one_item();
  test::static_vector::should_contain_three_item();
  test::static_vector::should_be_full();
  test::static_vector::should_be_popable();
}
