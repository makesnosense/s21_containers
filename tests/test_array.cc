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
  ArrayTest() {};
};

using TestedTypes = ::testing::Types<char, int, double, DummyObject>;
TYPED_TEST_SUITE(ArrayTest, TestedTypes, );

TYPED_TEST(ArrayTest, DefaultConstructor) {
  s21::array<TypeParam, 5> arr;
  EXPECT_EQ(arr.size(), size_t{5});
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

// iterator
TYPED_TEST(ArrayTest, ComparisonOperators) {
  s21::array<TypeParam, 5> arr = {TypeParam(55), TypeParam(66), TypeParam(77)};
  typename s21::array<TypeParam, 5>::iterator iter1{arr.end()};
  typename s21::array<TypeParam, 5>::iterator iter2{arr.end()};

  --iter2;
  --iter2;

  EXPECT_TRUE(iter1 > iter2);
  EXPECT_FALSE(iter2 > iter1);
  EXPECT_TRUE(iter2 < iter1);
  EXPECT_FALSE(iter1 < iter2);
  EXPECT_TRUE(iter1 >= iter1);
  EXPECT_TRUE(iter1 >= iter2);
  EXPECT_FALSE(iter2 >= iter1);
  EXPECT_TRUE(iter2 <= iter2);
  EXPECT_TRUE(iter2 <= iter1);
  EXPECT_FALSE(iter1 <= iter2);
}

TYPED_TEST(ArrayTest, ComparisonOperators2) {
  s21::array<TypeParam, 5> arr = {TypeParam(55), TypeParam(66), TypeParam(77)};
  typename s21::array<TypeParam, 5>::const_iterator iter1(arr.end());
  typename s21::array<TypeParam, 5>::const_iterator iter2(arr.end());

  --iter2;
  --iter2;

  EXPECT_TRUE(iter1 > iter2);
  EXPECT_FALSE(iter2 > iter1);
  EXPECT_TRUE(iter2 < iter1);
  EXPECT_FALSE(iter1 < iter2);
  EXPECT_TRUE(iter1 >= iter1);
  EXPECT_TRUE(iter1 >= iter2);
  EXPECT_FALSE(iter2 >= iter1);
  EXPECT_TRUE(iter2 <= iter2);
  EXPECT_TRUE(iter2 <= iter1);
  EXPECT_FALSE(iter1 <= iter2);
}

TEST(ArrayConstexpr, BasicConstruction) {
  // Default construction
  constexpr s21::array<int, 3> arr1;
  static_assert(arr1.size() == 3, "Size should be 3");
  static_assert(!arr1.empty(), "Array shouldn't be empty");
  static_assert(arr1[0] == 0, "Default initialized element should be 0");
  static_assert(arr1[1] == 0, "Default initialized element should be 0");
  static_assert(arr1[2] == 0, "Default initialized element should be 0");

  // Copy construction
  constexpr s21::array<int, 3> arr2(arr1);
  static_assert(arr2[0] == 0, "Copied element should be 0");
  static_assert(arr2.size() == arr1.size(), "Sizes should match");

  // Move construction
  constexpr s21::array<int, 3> arr3(std::move(arr1));
  static_assert(arr3[0] == 0, "Moved element should be 0");
  static_assert(arr3.size() == 3, "Size should be preserved after move");
}

TEST(ArrayConstexpr, DefaultConstruction) {
  constexpr s21::array<int, 3> arr;
  static_assert(arr.size() == 3, "Size should be 3");
  static_assert(!arr.empty(), "Array shouldn't be empty");
  static_assert(arr[0] == 0, "Default initialized element should be 0");
}

TEST(ArrayConstexpr, ConstexprElementAccess) {
  constexpr s21::array<int, 3> arr;
  static_assert(arr[0] == 0, "Subscript operator failed");
  static_assert(arr.front() == 0, "Front access failed");
  static_assert(arr.back() == 0, "Back access failed");
}

TEST(ArrayConstexpr, ConstexprSizeChecks) {
  constexpr s21::array<int, 5> non_empty_arr;

  static_assert(!non_empty_arr.empty(), "Non-empty array shouldn't be empty");
  static_assert(non_empty_arr.size() == 5, "Non-empty array size should be 5");
}

TEST(ArrayConstexpr, ConstexprMaxSize) {
  constexpr s21::array<int, 3> arr;
  static_assert(arr.max_size() == 3, "Max size should equal N");
}

TEST(ArrayConstexpr, DifferentTypes) {
  constexpr s21::array<double, 2> double_arr;
  static_assert(double_arr[0] == 0.0, "Double array initialization failed");

  constexpr s21::array<char, 2> char_arr;
  static_assert(char_arr[0] == '\0', "Char array initialization failed");
}

TEST(ArrayConstexpr, ConstexprFill) {
  // Option 1: Using lambda for fully constexpr check
  constexpr auto make_filled_array = []() {
    s21::array<int, 3> arr{};
    arr.fill(42);
    return arr;
  };
  constexpr auto filled = make_filled_array();
  static_assert(filled[0] == 42, "First element should be 42");
  static_assert(filled[1] == 42, "Second element should be 42");
  static_assert(filled[2] == 42, "Third element should be 42");

  // Option 2: Runtime check with EXPECT
  s21::array<int, 3> arr{};
  arr.fill(42);
  EXPECT_EQ(arr[0], 42);
  EXPECT_EQ(arr[1], 42);
  EXPECT_EQ(arr[2], 42);

  // Option 3: Check through iterators
  for (const auto& element : arr) {
    EXPECT_EQ(element, 42);
  }
}

// Helper function for compile-time array operations
constexpr int get_test_value() {
  s21::array<int, 3> arr{};  // Default construct
  arr[1] = 42;               // Modify one element
  return arr[1];             // Return the modified value
}

TEST(ArrayConstexpr, SimpleModification) {
  static_assert(get_test_value() == 42,
                "Should be able to modify and read array in constexpr");
}