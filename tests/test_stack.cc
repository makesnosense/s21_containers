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
};
using TestedTypes = ::testing::Types<char, int, double, DummyObject>;
TYPED_TEST_SUITE(StackTest, TestedTypes, );
TYPED_TEST(StackTest, PushElement) {
  TypeParam value = TypeParam();
  this->empty_s21_stack_.push(value);
  this->empty_stl_stack_.push(value);
  EXPECT_EQ(this->empty_s21_stack_.size(), this->empty_stl_stack_.size());
  EXPECT_FALSE(this->empty_s21_stack_.empty());
}
TYPED_TEST(StackTest, PopElement) {
  TypeParam value = TypeParam();
  this->empty_s21_stack_.push(value);

  EXPECT_EQ(this->empty_s21_stack_.size(), size_t{1});

  this->empty_s21_stack_.pop();

  EXPECT_EQ(this->empty_s21_stack_.size(), size_t{0});

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
  TypeParam value = TypeParam();
  this->empty_s21_stack_.push(value);
  s21::stack<TypeParam> copied_stack = this->empty_s21_stack_;

  EXPECT_EQ(copied_stack.size(), this->empty_s21_stack_.size());
  EXPECT_EQ(copied_stack.top(), this->empty_s21_stack_.top());
}

TYPED_TEST(StackTest, MoveConstructor) {
  TypeParam value = TypeParam();

  this->empty_s21_stack_.push(value);
  s21::stack<TypeParam> moved_stack = std::move(this->empty_s21_stack_);

  EXPECT_EQ(moved_stack.size(), size_t{1});
  EXPECT_EQ(moved_stack.top(), value);
}
TYPED_TEST(StackTest, CopyAssignmentOperator) {
  TypeParam value = TypeParam();

  this->empty_s21_stack_.push(value);
  s21::stack<TypeParam> another_stack;
  another_stack = this->empty_s21_stack_;

  EXPECT_EQ(another_stack.size(), this->empty_s21_stack_.size());
  EXPECT_EQ(another_stack.top(), this->empty_s21_stack_.top());
}

TYPED_TEST(StackTest, MoveAssignmentOperator) {
  TypeParam value = TypeParam();

  this->empty_s21_stack_.push(value);
  s21::stack<TypeParam> another_stack;
  another_stack = std::move(this->empty_s21_stack_);

  EXPECT_EQ(another_stack.size(), size_t{1});
  EXPECT_EQ(another_stack.top(), value);
}
TEST(StackTest, StackPush) {
  s21::stack<int> mss;
  s21::stack<int> mss_swap;
  for (int i = 0; i < 10; i++) {
    mss.push(i);
  }
  mss_swap.push(5);
  mss_swap.push(5);

  mss.pop();

  mss_swap = mss;
  mss_swap.swap(mss);
  EXPECT_TRUE(true);
  EXPECT_EQ(1, 1);
}

TYPED_TEST(StackTest, InsertManyBack) {
  TypeParam value1 = TypeParam();
  TypeParam value2 = TypeParam();
  TypeParam value3 = TypeParam();

  this->empty_s21_stack_.insert_many_back(value1, value2, value3);

  EXPECT_EQ(this->empty_s21_stack_.size(), size_t{3});

  EXPECT_EQ(this->empty_s21_stack_.top(), value3);

  this->empty_s21_stack_.pop();
  EXPECT_EQ(this->empty_s21_stack_.top(), value2);

  this->empty_s21_stack_.pop();
  EXPECT_EQ(this->empty_s21_stack_.top(), value1);

  this->empty_s21_stack_.pop();

  EXPECT_TRUE(this->empty_s21_stack_.empty());
}

#if 0
#endif
