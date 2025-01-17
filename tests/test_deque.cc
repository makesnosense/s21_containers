#include <gtest/gtest.h>

#include <deque>

#include "dummy_object.h"
#include "s21_deque.h"

template <typename T>
class DequeTest : public testing::Test {
 protected:
  DequeTest() : empty_stl_deque_(), empty_s21_deque_() {}

  std::deque<T> empty_stl_deque_{};
  s21::deque<T> empty_s21_deque_{};
  // std::deque<T> stl_vec_;
  // s21::deque<T> s21_vec_;
  // std::deque<T> large_stl_vec_;
  // s21::deque<T> large_s21_vec_;
};

using TestedTypes = ::testing::Types<char, int, double, DummyObject>;
TYPED_TEST_SUITE(DequeTest, TestedTypes);

TYPED_TEST(DequeTest, DefaultConstructor) {
  // EXPECT_EQ(this->empty_stl_vec_.size(), this->empty_s21_vec_.size());
  // EXPECT_TRUE(this->empty_s21_vec_.empty());
}
