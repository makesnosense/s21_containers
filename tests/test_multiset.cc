#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#include <gtest/gtest.h>
#pragma GCC diagnostic pop

#include <set>

#include "s21_multiset.h"

template <typename Key>
class MultisetTest : public testing::Test {
 protected:
  MultisetTest()
      : empty_stl_mset_(),
        empty_s21_mset_(),
        stl_mset_{103, 53, 73, 106, 57, 102, 46, 104,
                  53,  73, 53, 53,  53, 53,  53, 53},  // Note duplicates
        s21_mset_{103, 53, 73, 106, 57, 102, 46, 104,
                  53,  73, 53, 53,  53, 53,  53, 53} {}

  std::multiset<Key> empty_stl_mset_;
  s21::multiset<Key> empty_s21_mset_;
  std::multiset<Key> stl_mset_;
  s21::multiset<Key> s21_mset_;
};

using TestedTypes = ::testing::Types<int, double>;
TYPED_TEST_SUITE(MultisetTest, TestedTypes, );

// Tests specific to multiset functionality
TYPED_TEST(MultisetTest, InsertDuplicates) {
  TypeParam value{42};

  // Insert same value multiple times
  this->empty_s21_mset_.insert(value);
  this->empty_s21_mset_.insert(value);
  this->empty_s21_mset_.insert(value);

  EXPECT_EQ(this->empty_s21_mset_.count(value), size_t{3});
  EXPECT_EQ(this->empty_s21_mset_.size(), size_t{3});
}

TYPED_TEST(MultisetTest, LowerUpperBound) {
  auto lower = this->s21_mset_.lower_bound(53);
  auto upper = this->s21_mset_.upper_bound(53);

  EXPECT_EQ(*lower, TypeParam{53});
  EXPECT_NE(*upper, TypeParam{53});
  EXPECT_GT(*upper, TypeParam{53});
}

TEST(MultisetNonTyped, MultipleIdenticalElements) {
  s21::multiset<int> mset(5, 42);  // Constructor with count and value
  EXPECT_EQ(mset.size(), size_t{5});
  EXPECT_EQ(mset.count(42), size_t{5});
}

TEST(MultisetNonTyped, EraseRange) {
  s21::multiset<int> mset{1, 1, 2, 2, 3, 3};
  auto first = mset.find(1);
  auto last = mset.find(3);

  while (first != last) {
    first = mset.erase(first);
  }

  EXPECT_EQ(mset.size(), size_t{2});  // Only the 3s should remain
  EXPECT_EQ(mset.count(3), size_t{2});
}

TYPED_TEST(MultisetTest, AnotherCount) {
  EXPECT_EQ(this->s21_mset_.count(53), this->stl_mset_.count(53));
}

// Tests for special multiset operations
TYPED_TEST(MultisetTest, EqualRange) {
  // Test equal_range on element with multiple occurrences
  auto range = this->s21_mset_.equal_range(53);
  size_t count = 0;
  for (auto it = range.first; it != range.second; ++it) {
    EXPECT_EQ(*it, TypeParam{53});
    ++count;
  }
  EXPECT_EQ(count, this->s21_mset_.count(53));
}

TYPED_TEST(MultisetTest, EraseAllOccurrences) {
  TypeParam value{53};
  size_t initial_count = this->s21_mset_.count(value);
  size_t erased = this->s21_mset_.erase(value);
  EXPECT_EQ(erased, initial_count);
  EXPECT_EQ(this->s21_mset_.count(value), size_t{0});
}

TYPED_TEST(MultisetTest, MergeWithDuplicates) {
  s21::multiset<TypeParam> other{53, 53, 53};
  size_t initial_count = this->s21_mset_.count(53);
  size_t other_count = other.count(53);

  this->s21_mset_.merge(other);

  EXPECT_EQ(this->s21_mset_.count(53), initial_count + other_count);
  EXPECT_TRUE(other.empty());
}

TYPED_TEST(MultisetTest, InsertIteratorValidity) {
  TypeParam value{42};
  auto it1 = this->empty_s21_mset_.insert(value);
  auto it2 = this->empty_s21_mset_.insert(value);

  EXPECT_NE(it1, it2);    // Different iterators for same value
  EXPECT_EQ(*it1, *it2);  // But pointing to same value
}

TEST(MultisetNonTyped, SwapWithDuplicates) {
  s21::multiset<int> mset1{1, 1, 2, 2};
  s21::multiset<int> mset2{3, 3, 3};

  size_t size1 = mset1.size();
  size_t size2 = mset2.size();

  mset1.swap(mset2);

  EXPECT_EQ(mset1.size(), size2);
  EXPECT_EQ(mset2.size(), size1);
  EXPECT_EQ(mset1.count(3), size_t{3});
  EXPECT_EQ(mset2.count(1), size_t{2});
}

TEST(MultisetNonTyped, ComplexOperations) {
  s21::multiset<int> mset;

  // Insert elements with duplicates
  for (int i = 0; i < 5; ++i) {
    mset.insert(1);  // 5 ones
    mset.insert(2);  // 5 twos
  }

  // Test lower_bound and upper_bound
  auto lower = mset.lower_bound(1);
  auto upper = mset.upper_bound(1);

  size_t count = 0;
  for (auto it = lower; it != upper; ++it) {
    EXPECT_EQ(*it, 1);
    ++count;
  }
  EXPECT_EQ(count, size_t{5});

  // Erase some elements and verify state
  mset.erase(lower);  // Remove one occurrence
  EXPECT_EQ(mset.count(1), size_t{4});

  // Verify iterators still work after modification
  auto new_lower = mset.lower_bound(1);
  EXPECT_NE(new_lower, lower);
  EXPECT_EQ(*new_lower, 1);
}