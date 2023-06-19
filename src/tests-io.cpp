#include <cstdio>
#include <n/format.hpp>
#include <n/io.hpp>
#include <n/result.hpp>
#include <n/string.hpp>
#include <n/tests.hpp>
#include <n/vector.hpp>

#include "n/iterator.hpp"

bool operator==(const n::string<char>& s, const char* s1) {
  return n::equal(s.iter(), n::pointer_iterator(s1, strlen(s1)));
}

bool operator==(const n::string<char>& s, const n::string<char>& s1) {
  return n::equal(s.iter(), s1.iter());
}

// Test file_iterator (readable mode)
void test_file_iterator_readable_mode() {
  const char* filename = "test_file.txt";
  FILE* file = fopen(filename, "w+");
  fputs("Hello\nWorld\n", file);
  rewind(file);

  n::file<char, n::mode::rp> f(file);
  n::file_iterator<char, n::mode::rp> it(&f);

  N_TEST_ASSERT_TRUE(it.has_next());
  N_TEST_ASSERT_EQUALS(it.next(), 'H');
  N_TEST_ASSERT_TRUE(it.has_next());
  N_TEST_ASSERT_EQUALS(it.next(), 'e');
  N_TEST_ASSERT_TRUE(it.has_next());
  N_TEST_ASSERT_EQUALS(it.next(), 'l');
  N_TEST_ASSERT_TRUE(it.has_next());
  N_TEST_ASSERT_EQUALS(it.next(), 'l');
  N_TEST_ASSERT_TRUE(it.has_next());
  N_TEST_ASSERT_EQUALS(it.next(), 'o');
  N_TEST_ASSERT_TRUE(it.has_next());
  N_TEST_ASSERT_EQUALS(it.next(), '\n');
  N_TEST_ASSERT_TRUE(it.has_next());
  N_TEST_ASSERT_EQUALS(it.next(), 'W');
  N_TEST_ASSERT_TRUE(it.has_next());
  N_TEST_ASSERT_EQUALS(it.next(), 'o');
  N_TEST_ASSERT_TRUE(it.has_next());
  N_TEST_ASSERT_EQUALS(it.next(), 'r');
  N_TEST_ASSERT_TRUE(it.has_next());
  N_TEST_ASSERT_EQUALS(it.next(), 'l');
  N_TEST_ASSERT_TRUE(it.has_next());
  N_TEST_ASSERT_EQUALS(it.next(), 'd');
  N_TEST_ASSERT_TRUE(it.has_next());
  N_TEST_ASSERT_EQUALS(it.next(), '\n');
  N_TEST_ASSERT_FALSE(it.has_next());

  remove(filename);
}

// Test file_oterator (writable mode)
void test_file_oterator_writable_mode() {
  const char* filename = "test_file.txt";

  {
    n::file<char, n::mode::wp> f(filename);
    auto ot = f.oter();

    ot.sext('H');
    ot.sext('e');
    ot.sext('l');
    ot.sext('l');
    ot.sext('o');
    ot.sext('\n');
    ot.sext('W');
    ot.sext('o');
    ot.sext('r');
    ot.sext('l');
    ot.sext('d');
    ot.sext('\n');
  }

  {
    n::file<char, n::mode::r> f(filename);
    n::string<char> s(100);
    
    copy<char>(f.iter(), s.oter());

    N_TEST_ASSERT_EQUALS(s, n::str("Hello\nWorld\n"));
  }

  remove(filename);
}

// Test file (pathable mode)
void test_file_pathable_mode() {
  const char* filename = "test_file.txt";

  // Write to file
  {
    n::file<char, n::mode::w> f(filename);
    f.push('H');
    f.push('e');
    f.push('l');
    f.push('l');
    f.push('o');
    f.push('\n');
    f.push('W');
    f.push('o');
    f.push('r');
    f.push('l');
    f.push('d');
    f.push('\n');
  }

  // Read from file
  {
    n::file<char, n::mode::r> f(filename);
    N_TEST_ASSERT_TRUE(f.opened());

    n::maybe<char> c;

    c = f.pop();
    N_TEST_ASSERT_TRUE(c.has());
    N_TEST_ASSERT_EQUALS(c.get(), 'H');

    c = f.pop();
    N_TEST_ASSERT_TRUE(c.has());
    N_TEST_ASSERT_EQUALS(c.get(), 'e');

    c = f.pop();
    N_TEST_ASSERT_TRUE(c.has());
    N_TEST_ASSERT_EQUALS(c.get(), 'l');

    c = f.pop();
    N_TEST_ASSERT_TRUE(c.has());
    N_TEST_ASSERT_EQUALS(c.get(), 'l');

    c = f.pop();
    N_TEST_ASSERT_TRUE(c.has());
    N_TEST_ASSERT_EQUALS(c.get(), 'o');

    c = f.pop();
    N_TEST_ASSERT_TRUE(c.has());
    N_TEST_ASSERT_EQUALS(c.get(), '\n');

    c = f.pop();
    N_TEST_ASSERT_TRUE(c.has());
    N_TEST_ASSERT_EQUALS(c.get(), 'W');

    c = f.pop();
    N_TEST_ASSERT_TRUE(c.has());
    N_TEST_ASSERT_EQUALS(c.get(), 'o');

    c = f.pop();
    N_TEST_ASSERT_TRUE(c.has());
    N_TEST_ASSERT_EQUALS(c.get(), 'r');

    c = f.pop();
    N_TEST_ASSERT_TRUE(c.has());
    N_TEST_ASSERT_EQUALS(c.get(), 'l');

    c = f.pop();
    N_TEST_ASSERT_TRUE(c.has());
    N_TEST_ASSERT_EQUALS(c.get(), 'd');

    c = f.pop();
    N_TEST_ASSERT_TRUE(c.has());
    N_TEST_ASSERT_EQUALS(c.get(), '\n');

    c = f.pop();
    N_TEST_ASSERT_FALSE(c.has());
  }

  remove(filename);
}

// Test file (stdin mode)
void test_file_stdin_mode() {
  n::file<char, n::mode::std_in> f;
  N_TEST_ASSERT_FALSE(f.opened());
  N_TEST_ASSERT_TRUE(n::stdr.opened());
}

// Test file (stdout mode)
void test_file_stdout_mode() {
  n::file<char, n::mode::std_out> f;
  N_TEST_ASSERT_FALSE(f.opened());
  N_TEST_ASSERT_TRUE(n::stdw.opened());
}


// Main function to run the tests
int main() {
  N_TEST_SUITE("IO file test suite")

  N_TEST_REGISTER(test_file_iterator_readable_mode);
  N_TEST_REGISTER(test_file_oterator_writable_mode);
  N_TEST_REGISTER(test_file_pathable_mode);
  N_TEST_REGISTER(test_file_stdin_mode);
  N_TEST_REGISTER(test_file_stdout_mode);

  N_TEST_RUN_SUITE;

  return 0;
}
