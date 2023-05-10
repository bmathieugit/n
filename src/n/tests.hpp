#ifndef __n_tests_hpp__
#define __n_tests_hpp__

#include <stdbool.h>
#include <stdio.h>

// Variables globales pour le rapport de test
static int tests_passed = 0;
static int tests_failed = 0;
static bool verbose = false;

#define BEGIN_TEST_SUITE(suite_name) \
  printf("\033[1m========== Test Suite: %s ==========\033[0m\n\n", #suite_name);

#define END_TEST_SUITE                              \
  printf("\n\033[1m========== Test Suite Results ==========\033[0m\n\n"); \
  printf("\t - \033[32mTests passed: %d\033[0m\n", tests_passed);       \
  printf("\t - \033[31mTests failed: %d\033[0m\n", tests_failed);

#define TEST(test_name) printf("- \033[1mTest: %s ... \033[0m\n", #test_name);

#define ASSERT(assertion)                 \
  if (verbose) printf("\t" #assertion " :  "); \
  if (assertion) {                        \
    printf("\t\033[32mPASSED\033[0m\n");    \
    tests_passed++;                       \
  } else {                                \
    printf("\t\033[31mFAILED\033[0m\n");    \
    tests_failed++;                       \
  }

#endif
