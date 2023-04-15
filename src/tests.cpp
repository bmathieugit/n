#include "n/utils.hpp"
#include "n/vector.hpp"
#include <n/maybe.hpp>

#include <stdio.h>
#include <assert.h>




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
    n::maybe<int> init =2;
    auto mapped = init.map([](const auto& i){return i*2;});
    assert(mapped.has());
    assert(mapped.get() == 4);
  }
}

namespace test::static_vector {
  void should_be_empty(){
    n::static_vector<int, 10> emptyv;
    assert(emptyv.empty());
  }

  void should_contain_one_item();
  void should_contain_three_item();
  void should_be_full();
  void should_be_popable();
  void should_contains_one_item_after_push_it();
}


int main() {
  test::maybe::should_be_empty();
  test::maybe::should_be_valued();
  test::maybe::should_map_new_value();

  test::static_vector::should_be_empty();
}
