#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#include <gtest/gtest.h>
#pragma GCC diagnostic pop

#include <array>
#include <iostream>

#include "dummy_object.h"
#include "s21_array.h"

template <typename T>
class ArrayTest : public testing::Test {
 protected:
  ArrayTest() : empty_stl_array_(), empty_s21_array_() {};

  std::array<T, 5> empty_stl_array_;
  s21::array<T, 5> empty_s21_array_;
};
using TestedTypes = ::testing::Types<char, int, double, DummyObject>;
TYPED_TEST_SUITE(ArrayTest, TestedTypes, );

TEST(ArrayTest, array) {
  s21::array<int, 5> myArray;

  myArray.fill(0);

  myArray[0] = 1;
  myArray[1] = 2;
  myArray[2] = 3;

  myArray.print();
  myArray.empty();
  myArray.max_size();
  std::cout << "Size of array: " << myArray.size() << std::endl;

  EXPECT_TRUE(true);
  EXPECT_EQ(1, 1);
}
#if 0
#endif
