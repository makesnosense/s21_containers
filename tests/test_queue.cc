#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#include <gtest/gtest.h>
#pragma GCC diagnostic pop

#include <iostream>
#include <list>
#include <queue>

#include "dummy_object.h"
#include "s21_queue.h"

template <typename T>
class QueueTest : public testing::Test {
 protected:
  QueueTest() : empty_stl_queue_(), empty_s21_queue_() {};

  std::queue<T> empty_stl_queue_;
  s21::queue<T> empty_s21_queue_;
};
using TestedTypes = ::testing::Types<char, int, double, DummyObject>;
TYPED_TEST_SUITE(QueueTest, TestedTypes, );

TYPED_TEST(QueueTest, EnqueueElement) {
  TypeParam value = TypeParam();
  this->empty_s21_queue_.push(value);
  this->empty_stl_queue_.push(value);

  EXPECT_EQ(this->empty_s21_queue_.size(), this->empty_stl_queue_.size());
  EXPECT_FALSE(this->empty_s21_queue_.empty());
}

TYPED_TEST(QueueTest, DequeueElement) {
  TypeParam value = TypeParam();
  this->empty_s21_queue_.push(value);

  EXPECT_EQ(this->empty_s21_queue_.size(), size_t{1});

  this->empty_s21_queue_.pop();

  EXPECT_EQ(this->empty_s21_queue_.size(), size_t{0});
  EXPECT_TRUE(this->empty_s21_queue_.empty());
}

TYPED_TEST(QueueTest, FrontElement) {
  TypeParam value = TypeParam();
  this->empty_s21_queue_.push(value);

  EXPECT_EQ(this->empty_s21_queue_.front(), value);
}

TYPED_TEST(QueueTest, IsEmpty) {
  EXPECT_TRUE(this->empty_s21_queue_.empty());

  TypeParam value = TypeParam();
  this->empty_s21_queue_.push(value);

  EXPECT_FALSE(this->empty_s21_queue_.empty());
}

TYPED_TEST(QueueTest, CopyConstructor) {
  TypeParam value = TypeParam();
  this->empty_s21_queue_.push(value);

  s21::queue<TypeParam> copied_queue = this->empty_s21_queue_;

  EXPECT_EQ(copied_queue.size(), this->empty_s21_queue_.size());
  EXPECT_EQ(copied_queue.front(), this->empty_s21_queue_.front());
}

TYPED_TEST(QueueTest, MoveConstructor) {
  TypeParam value = TypeParam();

  this->empty_s21_queue_.push(value);

  s21::queue<TypeParam> moved_queue = std::move(this->empty_s21_queue_);

  EXPECT_EQ(moved_queue.size(), size_t{1});
  EXPECT_EQ(moved_queue.front(), TypeParam());
}

TYPED_TEST(QueueTest, CopyAssignmentOperator) {
  TypeParam value = TypeParam();

  this->empty_s21_queue_.push(value);

  s21::queue<TypeParam> another_queue;
  another_queue = this->empty_s21_queue_;

  EXPECT_EQ(another_queue.size(), this->empty_s21_queue_.size());
  EXPECT_EQ(another_queue.front(), this->empty_s21_queue_.front());
}

TYPED_TEST(QueueTest, MoveAssignmentOperator) {
  TypeParam value = TypeParam();

  this->empty_s21_queue_.push(value);

  s21::queue<TypeParam> another_queue;
  another_queue = std::move(this->empty_s21_queue_);

  EXPECT_EQ(another_queue.size(), size_t{1});
  EXPECT_EQ(another_queue.front(), TypeParam());

  EXPECT_EQ(this->empty_s21_queue_.size(), size_t{0});
}

TEST(QueueTest, QueueFunctionality) {
  s21::queue<int> queue;
  for (int i = 0; i < 10; ++i) {
    queue.push(i);
  }

  EXPECT_EQ(queue.size(), size_t{10});

  queue.pop();
  EXPECT_EQ(queue.size(), size_t{9});
  EXPECT_EQ(queue.front(), 1);

  for (int i = 0; i < 9; ++i) {
    queue.pop();
  }

  EXPECT_TRUE(queue.empty());
}

TEST(QueueTest, InsertManyBack) {
  s21::queue<int> queue;

  queue.insert_many_back(0, 1, 2, 3, 4, 5, 6, 7, 8, 9);

  EXPECT_EQ(queue.size(), size_t{10});

  for (int i = 0; i < 10; ++i) {
    EXPECT_EQ(queue.front(), i);
    queue.pop();
  }

  EXPECT_TRUE(queue.empty());
}
#if 0
#endif
