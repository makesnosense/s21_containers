#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#include <gtest/gtest.h>
#pragma GCC diagnostic pop

#include <iostream>
#include <list>
#include <stack>

#include "dummy_object.h"
#include "s21_stack.h"

template <typename T>
class StackTest : public testing::Test {
 protected:
  StackTest() : empty_stl_stack_(), empty_s21_stack_() {};

  std::stack<T> empty_stl_stack_;
  s21::stack<T> empty_s21_stack_;
  // std::stack<T> stl_stack_;
  // s21::stack<T> s21_stack_;
};
using TestedTypes = ::testing::Types<char, int, double, DummyObject>;
TYPED_TEST_SUITE(StackTest, TestedTypes, );

TYPED_TEST(StackTest, stack1) { EXPECT_EQ(1, 1); }

TEST(StackTest, stack) {
  s21::stack<int> mss;
  s21::stack<int> mss_swap;
  for (int i = 0; i < 10; i++) {
    mss.push(i);
  }
  mss_swap.push(5);
  mss_swap.push(5);

  std::cout << mss.size() << "\n";
  mss.pop();
  std::cout << mss.size() << "\n";
  std::cout << mss.top() << "\n";

  mss_swap = mss;
  mss_swap.swap(mss);
  EXPECT_TRUE(true);
  EXPECT_EQ(1, 1);
}
#if 0
#endif
