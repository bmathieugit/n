#ifndef __n_tests_hpp__
#define __n_tests_hpp__

#include <n/format.hpp>
#include <n/iofile.hpp>
#include <n/string.hpp>
#include <n/vector.hpp>

namespace n {
struct Test {
  std::string name;
  void (*func)();
  Test(const std::string& name, void (*func)()) : name(name), func(func) {}
};

struct AssertionResult {
  std::string message;
  bool passed;
};

template <typename T>
AssertionResult AssertEqual(const T& actual, const T& expected,
                            const std::string& message = "") {
  if (actual == expected) {
    return {"", true};
  } else {
    std::string result_message = "Assertion failed: " + message +
                                 "\n"
                                 "  Expected: " +
                                 std::to_string(expected) +
                                 "\n"
                                 "  Actual: " +
                                 std::to_string(actual) + "\n";
    return {result_message, false};
  }
}

class TestRunner {
 public:
  void AddTest(const std::string& name, void (*func)()) {
    tests_.emplace_back(name, func);
  }

  void Run() {
    int passed = 0, failed = 0;
    for (const auto& test : tests_) {
      const AssertionResult test_result = RunTest(test);
      if (test_result.passed) {
        std::cerr << test.name << " PASSED\n";
        ++passed;
      } else {
        std::cerr << test.name << " FAILED\n"
                  << test_result.message << std::endl;
        ++failed;
      }
    }
    std::cerr << "Tests passed: " << passed << ", tests failed: " << failed
              << '\n';
  }

 private:
  std::vector<Test> tests_;

  AssertionResult RunTest(const Test& test) {
    try {
      test.func();
      return {"", true};
    } catch (const AssertionResult& ar) {
      return ar;
    }
  }
};
}  // namespace n

#define ASSERT_EQUAL(actual, expected)                                    \
  do {                                                                    \
    const auto& ar =                                                      \
        TestLib::AssertEqual(actual, expected, #actual " != " #expected); \
    if (!ar.passed) {                                                     \
      throw ar;                                                           \
    }                                                                     \
  } while (false)

#define RUN_TESTS()           \
  TestLib::TestRunner{}.Run() \
  }

#endif
