#include <gtest/gtest.h>

#include <vector>

#include "s21_containers.h"

template <typename T>
class VectorTest : public testing::Test {
 protected:
  VectorTest() : stl_vec_{65, 66, 67}, s21_vec_{65, 66, 67} {}

  std::vector<T> stl_vec_;
  s21::vector<T> s21_vec_;
};

using TestedTypes = ::testing::Types<int, double, char>;
TYPED_TEST_SUITE(VectorTest, TestedTypes);

// Use TYPED_TEST instead of TEST_F
TYPED_TEST(VectorTest, SizeIsCorrect) {
  EXPECT_EQ(this->stl_vec_.size(), this->s21_vec_.size());
}

TYPED_TEST(VectorTest, ElementsMatch) {
  EXPECT_EQ(this->stl_vec_[0], this->s21_vec_[0]);
  EXPECT_EQ(this->stl_vec_[1], this->s21_vec_[1]);
  EXPECT_EQ(this->stl_vec_[2], this->s21_vec_[2]);
}