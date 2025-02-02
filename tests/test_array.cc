#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#include <gtest/gtest.h>
#pragma GCC diagnostic pop

#include <array>
#include <iostream>

#include "dummy_object.h"
#include "s21_array.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
template <typename T>
class ArrayTest : public testing::Test {
 protected:
  ArrayTest() : empty_stl_array_(), empty_s21_array_() {};

  std::array<T, 5> empty_stl_array_;
  s21::array<T, 5> empty_s21_array_;
};

using TestedTypes = ::testing::Types<char, int, double, DummyObject>;
TYPED_TEST_SUITE(ArrayTest, TestedTypes, );

TYPED_TEST(ArrayTest, DefaultConstructor) {
  s21::array<TypeParam, 5> arr;
  EXPECT_EQ(arr.size(), 5);
}

TYPED_TEST(ArrayTest, FillAndAccess) {
  s21::array<TypeParam, 5> arr;
  arr.fill(TypeParam());

  for (size_t i = 0; i < arr.size(); ++i) {
    EXPECT_EQ(arr[i], TypeParam());
  }
  arr[0] = TypeParam(1);
  arr[1] = TypeParam(2);

  EXPECT_EQ(arr[0], TypeParam(1));
  EXPECT_EQ(arr[1], TypeParam(2));
}

TYPED_TEST(ArrayTest, AtMethod) {
  s21::array<TypeParam, 5> arr;
  arr.fill(TypeParam(10));

  EXPECT_EQ(arr.at(0), TypeParam(10));
  EXPECT_EQ(arr.at(4), TypeParam(10));
}

TYPED_TEST(ArrayTest, FrontAndBack) {
  s21::array<TypeParam, 5> arr;
  arr.fill(TypeParam(20));

  EXPECT_EQ(arr.front(), TypeParam(20));
  EXPECT_EQ(arr.back(), TypeParam(20));
}

TYPED_TEST(ArrayTest, Swap) {
  s21::array<TypeParam, 5> arr1;
  s21::array<TypeParam, 5> arr2;

  arr1.fill(TypeParam(1));
  arr2.fill(TypeParam(2));

  arr1.swap(arr2);

  for (size_t i = 0; i < arr1.size(); ++i) {
    EXPECT_EQ(arr1[i], TypeParam(2));
    EXPECT_EQ(arr2[i], TypeParam(1));
  }
}

TYPED_TEST(ArrayTest, InitializerListConstructor) {
  s21::array<TypeParam, 5> arr = {TypeParam(1), TypeParam(2), TypeParam(3)};

  EXPECT_EQ(arr[0], TypeParam(1));
  EXPECT_EQ(arr[1], TypeParam(2));
  EXPECT_EQ(arr[2], TypeParam(3));

  EXPECT_EQ(arr[3], TypeParam());
  EXPECT_EQ(arr[4], TypeParam());
}

TYPED_TEST(ArrayTest, OutOfRangeAccess) {
  s21::array<TypeParam, 5> arr;

  EXPECT_THROW(arr[5], std::logic_error);
}

#pragma GCC diagnostic pop
#if 0
#endif
