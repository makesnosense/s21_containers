#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#include <gtest/gtest.h>
#pragma GCC diagnostic pop

#include <algorithm>
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

// Additional edge case tests
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

  // std::cout << mset

  EXPECT_EQ(mset.size(), size_t{2});  // Only the 3s should remain
  EXPECT_EQ(mset.count(3), size_t{2});
}

TYPED_TEST(MultisetTest, Count2) {
  EXPECT_EQ(this->s21_mset_.count(53), this->stl_mset_.count(53));
}

TEST(MultisetTest, m) {
  // s21::multiset<int> m{1, 4, 5, 5, 5, 5, 5, 6, 7, 8, 9, 9};

  std::set m{1, 4, 5, 5, 5, 5, 5, 6, 7, 8, 9, 9};

  // m.erase(5);
  // m.erase(9);
  // Forward iteration
  // print_tree(meow.tree_);

  for (auto it = m.begin(); it != m.end(); ++it) {
    std::cout << *it << ' ';
  }
}