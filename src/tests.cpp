#include "n/utils.hpp"
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


int main() {
  test::maybe::should_be_empty();
  test::maybe::should_be_valued();
  test::maybe::should_map_new_value();
}
