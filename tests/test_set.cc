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
        stl_set_{3, 53, 73, 6, 57, 2, 46, 4},
        s21_set_{3, 53, 73, 6, 57, 2, 46, 4} {}

  std::set<Key> empty_stl_set_;
  s21::set<Key> empty_s21_set_;
  std::set<Key> stl_set_;
  s21::set<Key> s21_set_;
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
TYPED_TEST(SetTest, CopyConstructor) {
  s21::set<TypeParam> s21_set_2(this->s21_set_);

  EXPECT_EQ(this->s21_set_, s21_set_2);
}

// move constructer
TYPED_TEST(SetTest, MoveConstructor) {
  s21::set<TypeParam> s21_set_2(std::move(this->s21_set_));

  EXPECT_EQ(s21_set_2.size(), size_t{8});
  EXPECT_TRUE(s21_set_2.contains(3));
  EXPECT_TRUE(s21_set_2.contains(53));
  EXPECT_TRUE(s21_set_2.contains(73));
  EXPECT_TRUE(s21_set_2.contains(6));
  EXPECT_TRUE(s21_set_2.contains(57));
  EXPECT_TRUE(s21_set_2.contains(2));
  EXPECT_TRUE(s21_set_2.contains(46));
  EXPECT_TRUE(s21_set_2.contains(4));

  EXPECT_EQ(this->s21_set_.size(), size_t{0});
  EXPECT_FALSE(this->s21_set_.contains(1));
  EXPECT_FALSE(this->s21_set_.contains(2));
  EXPECT_FALSE(this->s21_set_.contains(3));
}

// ConstructorWithSizeAndValue
TEST(SetTest, ConstructorWithSizeAndValue) {
  size_t size = 5;
  int value = 42;

  s21::set<int> test_set(size, value);

  EXPECT_EQ(test_set.size(), 1);

  EXPECT_TRUE(test_set.contains(value));

  s21::set<int> empty_set(0, value);
  EXPECT_EQ(empty_set.size(), size_t{0});
  EXPECT_FALSE(empty_set.contains(value));
}

// Move Assignment Operator
TYPED_TEST(SetTest, MoveAssignmentOperator) {
  s21::set<TypeParam> s21_temp_set{this->s21_set_};

  this->empty_s21_set_ = std::move(this->s21_set_);

  EXPECT_TRUE(this->s21_set_.empty());
  EXPECT_EQ(this->empty_s21_set_.size(), size_t{8});

  EXPECT_EQ(s21_temp_set, this->empty_s21_set_);
}

// erase
TYPED_TEST(SetTest, EraseIteratorPos) {
  auto it = this->s21_set_.find(3);
  ASSERT_NE(it, this->s21_set_.end());
  this->s21_set_.erase(it);

  EXPECT_EQ(this->s21_set_.size(), size_t{7});
  EXPECT_FALSE(this->s21_set_.contains(3));

  it = this->s21_set_.find(2);
  ASSERT_NE(it, this->s21_set_.end());
  this->s21_set_.erase(it);

  EXPECT_EQ(this->s21_set_.size(), size_t{6});
  EXPECT_FALSE(this->s21_set_.contains(2));

  it = this->s21_set_.find(57);
  ASSERT_NE(it, this->s21_set_.end());
  this->s21_set_.erase(it);

  EXPECT_EQ(this->s21_set_.size(), size_t{5});
  EXPECT_FALSE(this->s21_set_.contains(57));

  EXPECT_FALSE(this->s21_set_.empty());
}

TYPED_TEST(SetTest, EraseIteratorPos_EmptySet) {
  auto it = this->empty_s21_set_.begin();
  this->empty_s21_set_.erase(it);

  EXPECT_TRUE(this->empty_s21_set_.empty());
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

  EXPECT_EQ(s21_set.size(), size_t{5});
}
