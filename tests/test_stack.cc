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
TYPED_TEST(StackTest, PushElement) {
  this->empty_s21_stack_.push(TypeParam());
  this->empty_stl_stack_.push(TypeParam());
  EXPECT_EQ(this->empty_s21_stack_.size(), this->empty_stl_stack_.size());
  EXPECT_FALSE(this->empty_s21_stack_.empty());
}
TYPED_TEST(StackTest, PopElement) {
  TypeParam value = TypeParam();
  this->empty_s21_stack_.push(value);

  EXPECT_EQ(this->empty_s21_stack_.size(), 1);

  this->empty_s21_stack_.pop();

  EXPECT_EQ(this->empty_s21_stack_.size(), 0);

  EXPECT_TRUE(this->empty_s21_stack_.empty());
}
TYPED_TEST(StackTest, TopElement) {
  TypeParam value = TypeParam();
  this->empty_s21_stack_.push(value);
  EXPECT_EQ(this->empty_s21_stack_.top(), value);
}
TYPED_TEST(StackTest, IsEmpty) {
  EXPECT_TRUE(this->empty_s21_stack_.empty());
  TypeParam value = TypeParam();
  this->empty_s21_stack_.push(value);
  EXPECT_FALSE(this->empty_s21_stack_.empty());
}

TYPED_TEST(StackTest, CopyConstructor) {
  this->empty_s21_stack_.push(TypeParam());
  s21::stack<TypeParam> copied_stack = this->empty_s21_stack_;

  EXPECT_EQ(copied_stack.size(), this->empty_s21_stack_.size());
  EXPECT_EQ(copied_stack.top(), this->empty_s21_stack_.top());
}

TYPED_TEST(StackTest, MoveConstructor) {
  this->empty_s21_stack_.push(TypeParam());
  s21::stack<TypeParam> moved_stack = std::move(this->empty_s21_stack_);

  EXPECT_EQ(moved_stack.size(), 1);
  EXPECT_EQ(moved_stack.top(), TypeParam());
}
TYPED_TEST(StackTest, CopyAssignmentOperator) {
  this->empty_s21_stack_.push(TypeParam());
  s21::stack<TypeParam> another_stack;
  another_stack = this->empty_s21_stack_;

  EXPECT_EQ(another_stack.size(), this->empty_s21_stack_.size());
  EXPECT_EQ(another_stack.top(), this->empty_s21_stack_.top());
}

TYPED_TEST(StackTest, MoveAssignmentOperator) {
  this->empty_s21_stack_.push(TypeParam());
  s21::stack<TypeParam> another_stack;
  another_stack = std::move(this->empty_s21_stack_);

  EXPECT_EQ(another_stack.size(), 1);
  EXPECT_EQ(another_stack.top(), TypeParam());
}
// TYPED_TEST(StackTest, stack1) { EXPECT_EQ(1, 1); }

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
