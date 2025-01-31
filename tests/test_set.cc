#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#include <gtest/gtest.h>
#pragma GCC diagnostic pop

#include <algorithm>
#include <cstring>
#include <vector>

#include "dummy_object.h"
#include "random.h"
#include "s21_set.h"

template <typename Key>
class SetTest : public testing::Test {
 protected:
  SetTest()
      : empty_stl_set_(),
        empty_s21_set_(),
        stl_set_{68, 66, 67},
        s21_set_{68, 66, 67}  // large_stl_set_(1000, 42),
                              // large_s21_set_(1000, 42)
  {}

  std::set<Key> empty_stl_set_;
  s21::set<Key> empty_s21_set_;
  std::set<Key> stl_set_;
  s21::set<Key> s21_set_;
  // std::set<Key> large_stl_set_;
  // s21::set<Key> large_s21_set_;
};

using TestedTypes = ::testing::Types<char, int, double, DummyObject>;
TYPED_TEST_SUITE(SetTest, TestedTypes, );

TYPED_TEST(SetTest, InsertSet_1) {
  for (int i{0}; i < 200; i++) {
    TypeParam value{7};
    this->empty_stl_set_.insert(value);
    this->empty_s21_set_.insert(value);
  }

  typename s21::set<TypeParam>::iterator s21_set_it{
      this->empty_s21_set_.begin()};
  typename std::set<TypeParam>::iterator std_set_it{
      this->empty_stl_set_.begin()};

  for (size_t i{0}; i < this->empty_s21_set_.size(); i++) {
    EXPECT_EQ(*s21_set_it, *std_set_it);
    *std_set_it++;
    *s21_set_it++;
  }

  EXPECT_TRUE(this->empty_s21_set_.size() == this->empty_stl_set_.size());
}

TYPED_TEST(SetTest, InsertDuplicate) {
  this->empty_stl_set_.insert(42);
  this->empty_s21_set_.insert(42);

  auto result_std = this->empty_stl_set_.insert(42);
  auto result_s21 = this->empty_s21_set_.insert(42);

  EXPECT_FALSE(result_s21.second);
  EXPECT_FALSE(result_std.second);

  EXPECT_EQ(*result_s21.first, *result_std.first);
  EXPECT_EQ(this->empty_s21_set_.size(), this->empty_stl_set_.size());
}

TYPED_TEST(SetTest, InsertFromEmptySet) {
  auto result_s21 = this->empty_s21_set_.insert(100);
  auto result_std = this->empty_stl_set_.insert(100);

  EXPECT_TRUE(result_s21.second);
  EXPECT_TRUE(result_std.second);

  EXPECT_EQ(*result_s21.first, *result_std.first);
  EXPECT_EQ(this->empty_s21_set_.size(), this->empty_stl_set_.size());
}

TYPED_TEST(SetTest, InsertWithExistingElements) {
  this->empty_s21_set_.insert(10);
  this->empty_s21_set_.insert(20);
  this->empty_s21_set_.insert(30);

  this->empty_stl_set_.insert(10);
  this->empty_stl_set_.insert(20);
  this->empty_stl_set_.insert(30);

  auto result_s21 = this->empty_s21_set_.insert(20);
  auto result_std = this->empty_stl_set_.insert(20);

  EXPECT_FALSE(result_s21.second);
  EXPECT_FALSE(result_std.second);

  EXPECT_EQ(*result_s21.first, *result_std.first);
  EXPECT_EQ(this->empty_s21_set_.size(), this->empty_stl_set_.size());
}

// copy contructer
TEST(SetTest, CopyConstructor) {
  s21::set<int> s21_set_1{3, 53, 145, 6, 57, 2, 46, 2};
  s21_set_1.insert(1);
  s21_set_1.insert(2);
  s21_set_1.insert(3);

  s21::set<int> s21_set_2(s21_set_1);

  EXPECT_EQ(s21_set_1, s21_set_2);
}

// move constructer
TEST(SetTest, MoveConstructor) {
  s21::set<int> s21_set_1;
  s21_set_1.insert(1);
  s21_set_1.insert(2);
  s21_set_1.insert(3);

  s21::set<int> s21_set_2(std::move(s21_set_1));

  EXPECT_EQ(s21_set_2.size(), size_t{3});
  EXPECT_TRUE(s21_set_2.contains(1));
  EXPECT_TRUE(s21_set_2.contains(2));
  EXPECT_TRUE(s21_set_2.contains(3));

  EXPECT_EQ(s21_set_1.size(), size_t{0});
  EXPECT_FALSE(s21_set_1.contains(1));
  EXPECT_FALSE(s21_set_1.contains(2));
  EXPECT_FALSE(s21_set_1.contains(3));
}

// ConstructorWithSizeAndValue
TEST(SetTest, ConstructorWithSizeAndValue) {
  size_t size = 5;
  int value = 42;

  s21::set<int> test_set(size, value);

  EXPECT_EQ(test_set.size(), 1);

  EXPECT_TRUE(test_set.contains(value));

  s21::set<int> empty_set(0, value);
  EXPECT_EQ(empty_set.size(), 0);
  EXPECT_FALSE(empty_set.contains(value));
}

// Move Assignment Operator
TEST(SetTest, MoveAssignmentOperator) {
  s21::set<int> s21_set_1 = {1, 2, 3, 4, 5};
  s21::set<int> s21_set_2;

  s21_set_2 = std::move(s21_set_1);

  EXPECT_TRUE(s21_set_1.empty());
  EXPECT_EQ(s21_set_2.size(), 5);

  for (int i = 1; i <= 5; ++i) {
    EXPECT_TRUE(s21_set_2.contains(i));
  }
}

// erase
TEST(SetTest, EraseIteratorPos) {
  s21::set<int> s21_set = {1, 2, 3, 4, 5};

  auto it = s21_set.find(3);
  ASSERT_NE(it, s21_set.end());
  s21_set.erase(it);

  EXPECT_EQ(s21_set.size(), 4);
  EXPECT_FALSE(s21_set.contains(3));

  it = s21_set.find(1);
  ASSERT_NE(it, s21_set.end());
  s21_set.erase(it);

  EXPECT_EQ(s21_set.size(), 3);
  EXPECT_FALSE(s21_set.contains(1));

  it = s21_set.find(5);
  ASSERT_NE(it, s21_set.end());
  s21_set.erase(it);

  EXPECT_EQ(s21_set.size(), 2);
  EXPECT_FALSE(s21_set.contains(5));

  EXPECT_FALSE(s21_set.empty());
}

TEST(SetTest, EraseIteratorPos_EmptySet) {
  s21::set<int> s21_set;

  auto it = s21_set.begin();
  s21_set.erase(it);

  EXPECT_TRUE(s21_set.empty());
}

TEST(SetTest, EraseIteratorPos_SingleElement) {
  s21::set<int> s21_set = {10};

  auto it = s21_set.begin();
  s21_set.erase(it);

  EXPECT_TRUE(s21_set.empty());
}

TEST(SetTest, EraseIteratorPos_NotFound) {
  s21::set<int> s21_set = {1, 2, 3, 4, 5};

  auto it = s21_set.find(6);
  ASSERT_EQ(it, s21_set.end());
  s21_set.erase(it);

  EXPECT_EQ(s21_set.size(), 5);
}
