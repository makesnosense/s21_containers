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
        stl_set_{103, 53, 73, 106, 57, 102, 46, 104},
        s21_set_{103, 53, 73, 106, 57, 102, 46, 104} {}

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

TYPED_TEST(SetTest, InsertMany) {
  std::vector<TypeParam> values = {TypeParam{1}, TypeParam{2}, TypeParam{3},
                                   TypeParam{4}, TypeParam{5}};

  this->empty_s21_set_.insert_many(values[0], values[1], values[2], values[3],
                                   values[4]);

  EXPECT_EQ(this->empty_s21_set_.size(), values.size());

  auto s21_it = this->empty_s21_set_.begin();

  for (size_t i = 0; i < values.size(); i++) {
    EXPECT_EQ(*s21_it, values[i]);
    *s21_it++;
  }
}

TYPED_TEST(SetTest, InsertManyDuplicates) {
  std::vector<TypeParam> values = {TypeParam{1}, TypeParam{2}, TypeParam{2},
                                   TypeParam{3}, TypeParam{4}};

  auto results = this->empty_s21_set_.insert_many(
      values[0], values[1], values[2], values[3], values[4]);

  EXPECT_EQ(this->empty_s21_set_.size(), size_t{4});

  EXPECT_NE(results.size(), values.size());

  std::vector<TypeParam> result = {TypeParam{1}, TypeParam{2}, TypeParam{3},
                                   TypeParam{4}};

  for (const auto& value : result) {
    EXPECT_TRUE(this->empty_s21_set_.contains(value));
  }

  EXPECT_TRUE(results[0].second);
  EXPECT_TRUE(results[1].second);
  EXPECT_TRUE(results[2].second);
  EXPECT_TRUE(results[3].second);
}

TYPED_TEST(SetTest, CopyConstructor) {
  s21::set<TypeParam> s21_set_2(this->s21_set_);

  EXPECT_EQ(this->s21_set_, s21_set_2);
}

TYPED_TEST(SetTest, MoveConstructor) {
  s21::set<TypeParam> s21_set_2(std::move(this->s21_set_));

  EXPECT_EQ(s21_set_2.size(), size_t{8});
  EXPECT_TRUE(s21_set_2.contains(103));
  EXPECT_TRUE(s21_set_2.contains(53));
  EXPECT_TRUE(s21_set_2.contains(73));
  EXPECT_TRUE(s21_set_2.contains(106));
  EXPECT_TRUE(s21_set_2.contains(57));
  EXPECT_TRUE(s21_set_2.contains(102));
  EXPECT_TRUE(s21_set_2.contains(46));
  EXPECT_TRUE(s21_set_2.contains(104));

  EXPECT_EQ(this->s21_set_.size(), size_t{0});
  EXPECT_FALSE(this->s21_set_.contains(1));
  EXPECT_FALSE(this->s21_set_.contains(2));
  EXPECT_FALSE(this->s21_set_.contains(3));
}

TEST(SetNonTyped, ConstructorWithSizeAndValue) {
  size_t size = 5;
  int value = 42;

  s21::set<int> test_set(size, value);

  EXPECT_EQ(test_set.size(), size_t{1});

  EXPECT_TRUE(test_set.contains(value));

  s21::set<int> empty_set(0, value);
  EXPECT_EQ(empty_set.size(), size_t{0});
  EXPECT_FALSE(empty_set.contains(value));
}

TYPED_TEST(SetTest, MoveAssignmentOperator) {
  s21::set<TypeParam> s21_temp_set{this->s21_set_};

  this->empty_s21_set_ = std::move(this->s21_set_);

  EXPECT_TRUE(this->s21_set_.empty());
  EXPECT_EQ(this->empty_s21_set_.size(), size_t{8});

  EXPECT_EQ(s21_temp_set, this->empty_s21_set_);
}

TYPED_TEST(SetTest, EraseIteratorPos1) {
  auto it = this->s21_set_.find(103);
  EXPECT_NE(it, this->s21_set_.end());
  this->s21_set_.erase(it);

  EXPECT_EQ(this->s21_set_.size(), size_t{7});
  EXPECT_FALSE(this->s21_set_.contains(3));

  it = this->s21_set_.find(102);
  EXPECT_NE(it, this->s21_set_.end());
  this->s21_set_.erase(it);

  EXPECT_EQ(this->s21_set_.size(), size_t{6});
  EXPECT_FALSE(this->s21_set_.contains(2));

  it = this->s21_set_.find(57);
  EXPECT_NE(it, this->s21_set_.end());
  this->s21_set_.erase(it);

  EXPECT_EQ(this->s21_set_.size(), size_t{5});
  EXPECT_FALSE(this->s21_set_.contains(57));

  EXPECT_FALSE(this->s21_set_.empty());
}

TYPED_TEST(SetTest, EraseIteratorPos2) {
  auto s21_it = this->s21_set_.find(103);
  auto std_it = this->stl_set_.find(103);

  EXPECT_NE(s21_it, this->s21_set_.end());
  EXPECT_NE(std_it, this->stl_set_.end());

  this->s21_set_.erase(s21_it);
  this->stl_set_.erase(std_it);

  EXPECT_EQ(this->s21_set_.size(), this->stl_set_.size());
  EXPECT_EQ(this->s21_set_.find(103) == this->s21_set_.end(),
            this->stl_set_.find(103) == this->stl_set_.end());

  s21_it = this->s21_set_.find(102);
  std_it = this->stl_set_.find(102);

  EXPECT_NE(s21_it, this->s21_set_.end());
  EXPECT_NE(std_it, this->stl_set_.end());

  this->s21_set_.erase(s21_it);
  this->stl_set_.erase(std_it);

  EXPECT_EQ(this->s21_set_.size(), this->stl_set_.size());
  EXPECT_EQ(this->s21_set_.find(102) == this->s21_set_.end(),
            this->stl_set_.find(102) == this->stl_set_.end());

  s21_it = this->s21_set_.find(57);
  std_it = this->stl_set_.find(57);

  EXPECT_NE(s21_it, this->s21_set_.end());
  EXPECT_NE(std_it, this->stl_set_.end());

  this->s21_set_.erase(s21_it);
  this->stl_set_.erase(std_it);

  EXPECT_EQ(this->s21_set_.size(), this->stl_set_.size());
  EXPECT_EQ(this->s21_set_.find(57) == this->s21_set_.end(),
            this->stl_set_.find(57) == this->stl_set_.end());

  EXPECT_EQ(this->s21_set_.empty(), this->stl_set_.empty());
}

TEST(SetNonTyped, EraseIteratorPos_SingleElement) {
  s21::set<int> s21_set = {10};

  auto it = s21_set.begin();
  s21_set.erase(it);

  EXPECT_TRUE(s21_set.empty());
}

TEST(SetNonTyped, IteratorPos_NotFound) {
  s21::set<int> s21_set = {1, 2, 3, 4, 5};

  auto it = s21_set.find(6);
  EXPECT_EQ(it, s21_set.end());

  EXPECT_EQ(s21_set.size(), size_t{5});
}

TYPED_TEST(SetTest, Swap_SameSize_1) {
  this->empty_s21_set_.swap(this->s21_set_);

  EXPECT_EQ(this->empty_s21_set_.size(), size_t{8});
  EXPECT_EQ(this->s21_set_.size(), size_t{0});

  EXPECT_TRUE(this->empty_s21_set_.contains(103));
  EXPECT_TRUE(this->empty_s21_set_.contains(53));
  EXPECT_TRUE(this->empty_s21_set_.contains(73));
  EXPECT_TRUE(this->empty_s21_set_.contains(106));
  EXPECT_TRUE(this->empty_s21_set_.contains(57));
  EXPECT_TRUE(this->empty_s21_set_.contains(102));
  EXPECT_TRUE(this->empty_s21_set_.contains(46));
  EXPECT_TRUE(this->empty_s21_set_.contains(104));

  EXPECT_FALSE(this->s21_set_.contains(73));
  EXPECT_FALSE(this->s21_set_.contains(53));
  EXPECT_FALSE(this->s21_set_.contains(3));
}

TYPED_TEST(SetTest, Swap_SameSize_2) {
  this->empty_s21_set_.swap(this->s21_set_);
  this->empty_stl_set_.swap(this->stl_set_);

  EXPECT_EQ(this->empty_s21_set_.size(), this->empty_stl_set_.size());
  EXPECT_EQ(this->s21_set_.size(), this->stl_set_.size());

  EXPECT_EQ(this->empty_s21_set_.contains(3),
            this->empty_stl_set_.count(3) > 0);
  EXPECT_EQ(this->empty_s21_set_.contains(53),
            this->empty_stl_set_.count(53) > 0);
  EXPECT_EQ(this->empty_s21_set_.contains(73),
            this->empty_stl_set_.count(73) > 0);
  EXPECT_EQ(this->empty_s21_set_.contains(6),
            this->empty_stl_set_.count(6) > 0);
  EXPECT_EQ(this->empty_s21_set_.contains(57),
            this->empty_stl_set_.count(57) > 0);
  EXPECT_EQ(this->empty_s21_set_.contains(2),
            this->empty_stl_set_.count(2) > 0);
  EXPECT_EQ(this->empty_s21_set_.contains(46),
            this->empty_stl_set_.count(46) > 0);
  EXPECT_EQ(this->empty_s21_set_.contains(4),
            this->empty_stl_set_.count(4) > 0);

  EXPECT_EQ(this->s21_set_.contains(73), this->stl_set_.count(73) > 0);
  EXPECT_EQ(this->s21_set_.contains(53), this->stl_set_.count(53) > 0);
  EXPECT_EQ(this->s21_set_.contains(3), this->stl_set_.count(3) > 0);
}

TEST(SetNonTyped, Swap_DifferentSize) {
  s21::set<int> s21_set1 = {1, 2, 3, 4, 5};
  s21::set<int> s21_set2 = {10};

  s21_set1.swap(s21_set2);

  EXPECT_EQ(s21_set1.size(), size_t{1});
  EXPECT_EQ(s21_set2.size(), size_t{5});

  EXPECT_TRUE(s21_set1.contains(10));

  EXPECT_TRUE(s21_set2.contains(1));
  EXPECT_TRUE(s21_set2.contains(2));
  EXPECT_TRUE(s21_set2.contains(3));
  EXPECT_TRUE(s21_set2.contains(4));
  EXPECT_TRUE(s21_set2.contains(5));
}

TEST(SetNonTyped, Swap_EmptyAndNonEmpty) {
  s21::set<int> s21_set1;
  s21::set<int> s21_set2 = {42};

  s21_set1.swap(s21_set2);

  EXPECT_TRUE(s21_set2.empty());
  EXPECT_EQ(s21_set1.size(), size_t{1});
  EXPECT_TRUE(s21_set1.contains(42));
}

TYPED_TEST(SetTest, Swap_BothEmpty) {
  s21::set<TypeParam> s21_set;

  this->empty_s21_set_.swap(s21_set);

  EXPECT_TRUE(this->empty_s21_set_.empty());
  EXPECT_TRUE(s21_set.empty());
}

TYPED_TEST(SetTest, Swap_SameElements) {
  s21::set<TypeParam> s21_set{this->s21_set_};

  s21_set.swap(this->s21_set_);

  EXPECT_EQ(s21_set.size(), size_t{8});
  EXPECT_EQ(this->s21_set_.size(), size_t{8});

  EXPECT_EQ(s21_set, this->s21_set_);
}

// merge
TYPED_TEST(SetTest, Merge_NonOverlapping) {
  s21::set<TypeParam> s21_set2 = {19, 21, 29};
  std::set<TypeParam> std_set2 = {19, 21, 29};

  this->s21_set_.merge(s21_set2);
  this->stl_set_.merge(std_set2);

  EXPECT_EQ(this->s21_set_.size(), size_t{11});
  EXPECT_TRUE(this->s21_set_.contains(103));
  EXPECT_TRUE(this->s21_set_.contains(53));
  EXPECT_TRUE(this->s21_set_.contains(73));
  EXPECT_TRUE(this->s21_set_.contains(106));
  EXPECT_TRUE(this->s21_set_.contains(57));
  EXPECT_TRUE(this->s21_set_.contains(102));
  EXPECT_TRUE(this->s21_set_.contains(46));
  EXPECT_TRUE(this->s21_set_.contains(104));
  EXPECT_TRUE(this->s21_set_.contains(19));
  EXPECT_TRUE(this->s21_set_.contains(21));
  EXPECT_TRUE(this->s21_set_.contains(29));

  EXPECT_EQ(this->stl_set_.size(), size_t{11});
  EXPECT_NE(this->stl_set_.find(102), this->stl_set_.end());
  EXPECT_NE(this->stl_set_.find(53), this->stl_set_.end());
  EXPECT_NE(this->stl_set_.find(73), this->stl_set_.end());
  EXPECT_NE(this->stl_set_.find(106), this->stl_set_.end());
  EXPECT_NE(this->stl_set_.find(57), this->stl_set_.end());
  EXPECT_NE(this->stl_set_.find(103), this->stl_set_.end());
  EXPECT_NE(this->stl_set_.find(46), this->stl_set_.end());
  EXPECT_NE(this->stl_set_.find(104), this->stl_set_.end());
  EXPECT_NE(this->stl_set_.find(19), this->stl_set_.end());
  EXPECT_NE(this->stl_set_.find(21), this->stl_set_.end());
  EXPECT_NE(this->stl_set_.find(29), this->stl_set_.end());

  EXPECT_TRUE(s21_set2.empty());
  EXPECT_TRUE(std_set2.empty());
}

TEST(SetNonTyped, Merge_Overlapping) {
  s21::set<int> s21_set1 = {1, 2, 3};
  s21::set<int> s21_set2 = {3, 4, 5};

  s21_set1.merge(s21_set2);

  EXPECT_EQ(s21_set1.size(), size_t{5});
  EXPECT_TRUE(s21_set1.contains(1));
  EXPECT_TRUE(s21_set1.contains(2));
  EXPECT_TRUE(s21_set1.contains(3));
  EXPECT_TRUE(s21_set1.contains(4));
  EXPECT_TRUE(s21_set1.contains(5));

  EXPECT_TRUE(s21_set2.empty());
}

TEST(SetNonTyped, Merge_EmptyIntoNonEmpty) {
  s21::set<int> s21_set1 = {10, 20, 30};
  s21::set<int> s21_set2;

  s21_set1.merge(s21_set2);

  EXPECT_EQ(s21_set1.size(), size_t{3});
  EXPECT_TRUE(s21_set1.contains(10));
  EXPECT_TRUE(s21_set1.contains(20));
  EXPECT_TRUE(s21_set1.contains(30));

  EXPECT_TRUE(s21_set2.empty());
}

TEST(SetNonTyped, Merge_NonEmptyIntoEmpty) {
  s21::set<int> s21_set1;
  s21::set<int> s21_set2 = {100, 200, 300};

  s21_set1.merge(s21_set2);

  EXPECT_EQ(s21_set1.size(), size_t{3});
  EXPECT_TRUE(s21_set1.contains(100));
  EXPECT_TRUE(s21_set1.contains(200));
  EXPECT_TRUE(s21_set1.contains(300));

  EXPECT_TRUE(s21_set2.empty());
}

TEST(SetNonTyped, Merge_BothEmpty) {
  s21::set<int> s21_set1;
  s21::set<int> s21_set2;

  s21_set1.merge(s21_set2);

  EXPECT_TRUE(s21_set1.empty());
  EXPECT_TRUE(s21_set2.empty());
}

TEST(SetNonTyped, Merge_SameSets) {
  s21::set<int> s21_set1 = {1, 2, 3};
  s21::set<int> s21_set2 = {1, 2, 3};

  s21_set1.merge(s21_set2);

  EXPECT_EQ(s21_set1.size(), size_t{3});
  EXPECT_TRUE(s21_set1.contains(1));
  EXPECT_TRUE(s21_set1.contains(2));
  EXPECT_TRUE(s21_set1.contains(3));

  EXPECT_TRUE(s21_set2.empty());
}

TYPED_TEST(SetTest, Find_ExistingElement) {
  auto s21_it = this->s21_set_.find(53);
  auto std_it = this->stl_set_.find(53);

  EXPECT_NE(s21_it, this->s21_set_.end());
  EXPECT_NE(std_it, this->stl_set_.end());

  EXPECT_EQ(*s21_it, *std_it);
}

TYPED_TEST(SetTest, Find_NonExistingElement) {
  auto s21_it = this->s21_set_.find(100);
  auto std_it = this->stl_set_.find(100);

  EXPECT_EQ(s21_it, this->s21_set_.end());
  EXPECT_EQ(std_it, this->stl_set_.end());
}

TYPED_TEST(SetTest, Find_FirstElement) {
  auto s21_it = this->s21_set_.find(103);
  auto std_it = this->stl_set_.find(103);

  EXPECT_NE(s21_it, this->s21_set_.end());
  EXPECT_NE(std_it, this->stl_set_.end());

  EXPECT_EQ(*s21_it, *std_it);
}

TYPED_TEST(SetTest, Find_LastElement) {
  auto s21_it = this->s21_set_.find(103);
  auto std_it = this->stl_set_.find(103);

  EXPECT_NE(s21_it, this->s21_set_.end());
  EXPECT_NE(std_it, this->stl_set_.end());

  EXPECT_EQ(*s21_it, *std_it);
}

TYPED_TEST(SetTest, Find_InEmptySet) {
  auto s21_it = this->empty_s21_set_.find(3);
  auto std_it = this->empty_stl_set_.find(3);

  EXPECT_EQ(s21_it, this->empty_s21_set_.end());
  EXPECT_EQ(std_it, this->empty_stl_set_.end());
}
