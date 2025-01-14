#include <gtest/gtest.h>

#include <random>

// #include <string>
#include <algorithm>
#include <vector>

#include "s21_containers.h"

// Custom class for testing complex types
class TestObject {
 public:
  TestObject(int val = 0) : value_(val) {}
  bool operator==(const TestObject& other) const {
    return value_ == other.value_;
  }
  bool operator!=(const TestObject& other) const {
    return value_ != other.value_;
  }

  bool operator<(const TestObject& other) const {
    return value_ < other.value_;
  }

  friend std::ostream& operator<<(std::ostream& os, const TestObject& obj) {
    os << obj.value_;
    return os;
  }

 private:
  int value_;
};

template <typename T>
class VectorTest : public testing::Test {
 protected:
  // Initialize vectors with different sizes and values for different tests
  VectorTest()
      : empty_stl_vec_(),
        empty_s21_vec_(),
        stl_vec_{68, 66, 67},
        s21_vec_{68, 66, 67},
        large_stl_vec_(1000, 42),
        large_s21_vec_(1000, 42) {}

  std::vector<T> empty_stl_vec_;
  s21::vector<T> empty_s21_vec_;
  std::vector<T> stl_vec_;
  s21::vector<T> s21_vec_;
  std::vector<T> large_stl_vec_;
  s21::vector<T> large_s21_vec_;
};

// Test with multiple types including custom class
using TestedTypes = ::testing::Types<char, int, double, TestObject>;
TYPED_TEST_SUITE(VectorTest, TestedTypes);

// Constructor Tests
TYPED_TEST(VectorTest, DefaultConstructor) {
  EXPECT_EQ(this->empty_stl_vec_.size(), this->empty_s21_vec_.size());
  EXPECT_TRUE(this->empty_s21_vec_.empty());
}

TYPED_TEST(VectorTest, SizeConstructor) {
  const size_t test_size = 5;
  std::vector<TypeParam> stl_vec(test_size);
  s21::vector<TypeParam> s21_vec(test_size);

  EXPECT_EQ(stl_vec.size(), s21_vec.size());
  EXPECT_EQ(stl_vec.capacity(), s21_vec.capacity());
}

TYPED_TEST(VectorTest, InitializerListConstructor) {
  EXPECT_EQ(this->stl_vec_.size(), this->s21_vec_.size());
  for (size_t i = 0; i < this->stl_vec_.size(); ++i) {
    EXPECT_EQ(this->stl_vec_[i], this->s21_vec_[i]);
  }
}

// Copy and Move Tests
TYPED_TEST(VectorTest, CopyConstructor) {
  std::vector<TypeParam> stl_copy(this->stl_vec_);
  s21::vector<TypeParam> s21_copy(this->s21_vec_);

  EXPECT_EQ(stl_copy.size(), s21_copy.size());
  for (size_t i = 0; i < stl_copy.size(); ++i) {
    EXPECT_EQ(stl_copy[i], s21_copy[i]);
  }
}

TYPED_TEST(VectorTest, MoveConstructor) {
  std::vector<TypeParam> stl_move(std::move(this->stl_vec_));
  s21::vector<TypeParam> s21_move(std::move(this->s21_vec_));

  EXPECT_EQ(stl_move.size(), s21_move.size());
  EXPECT_TRUE(this->s21_vec_.empty());  // Moved-from vector should be empty
}

// Element Access Tests
TYPED_TEST(VectorTest, OperatorBrackets) {
  for (size_t i = 0; i < this->stl_vec_.size(); ++i) {
    EXPECT_EQ(this->stl_vec_[i], this->s21_vec_[i]);
  }
}

TYPED_TEST(VectorTest, At) {
  for (size_t i = 0; i < this->stl_vec_.size(); ++i) {
    EXPECT_EQ(this->stl_vec_.at(i), this->s21_vec_.at(i));
  }
  EXPECT_THROW(this->s21_vec_.at(1000), std::out_of_range);
}

TYPED_TEST(VectorTest, FrontBack) {
  EXPECT_EQ(this->stl_vec_.front(), this->s21_vec_.front());
  EXPECT_EQ(this->stl_vec_.back(), this->s21_vec_.back());
}

// Capacity Tests
TYPED_TEST(VectorTest, Capacity) {
  EXPECT_EQ(this->stl_vec_.capacity(), this->s21_vec_.capacity());
  EXPECT_EQ(this->large_stl_vec_.capacity(), this->large_s21_vec_.capacity());
}

TYPED_TEST(VectorTest, Reserve) {
  this->s21_vec_.reserve(100);
  this->stl_vec_.reserve(100);
  EXPECT_EQ(this->stl_vec_.capacity(), this->s21_vec_.capacity());
  EXPECT_GE(this->s21_vec_.capacity(), 100);
}

TYPED_TEST(VectorTest, ShrinkToFit) {
  this->s21_vec_.reserve(100);
  this->s21_vec_.shrink_to_fit();
  EXPECT_EQ(this->s21_vec_.capacity(), this->s21_vec_.size());
}

// Modifier Tests
TYPED_TEST(VectorTest, PushBack) {
  TypeParam value{};
  this->stl_vec_.push_back(value);
  this->s21_vec_.push_back(value);

  EXPECT_EQ(this->stl_vec_.size(), this->s21_vec_.size());
  EXPECT_EQ(this->stl_vec_.back(), this->s21_vec_.back());
}

TEST(VectorTest, For_each) {
  std::vector<int> stl_v{1, 2, 3};
  s21::vector<int> s21_v{1, 2, 3};

  for (auto i : stl_v) {
    stl_v.push_back(i);
  }
  for (auto i : s21_v) {
    s21_v.push_back(i);
  }

  auto stl_it = stl_v.begin();
  auto s21_it = s21_v.begin();

  while (stl_it != stl_v.end() && s21_it != s21_v.end()) {
    EXPECT_EQ(*stl_it, *s21_it);
    ++stl_it;
    ++s21_it;
  }
  EXPECT_EQ(stl_v.size(), s21_v.size());
}

TYPED_TEST(VectorTest, PopBack) {
  this->stl_vec_.pop_back();
  this->s21_vec_.pop_back();

  EXPECT_EQ(this->stl_vec_.size(), this->s21_vec_.size());
  EXPECT_EQ(this->stl_vec_.back(), this->s21_vec_.back());
}

TYPED_TEST(VectorTest, Clear) {
  this->s21_vec_.clear();
  EXPECT_TRUE(this->s21_vec_.empty());
  EXPECT_EQ(this->s21_vec_.size(), 0);
}

// Iterator Tests
TYPED_TEST(VectorTest, Iterators) {
  auto stl_it = this->stl_vec_.begin();
  auto s21_it = this->s21_vec_.begin();

  while (stl_it != this->stl_vec_.end() && s21_it != this->s21_vec_.end()) {
    EXPECT_EQ(*stl_it, *s21_it);
    ++stl_it;
    ++s21_it;
  }

  EXPECT_TRUE(stl_it == this->stl_vec_.end() && s21_it == this->s21_vec_.end());
}

TYPED_TEST(VectorTest, OperatorBracketsModification) {
  // Test that operator[] returns a modifiable reference
  this->stl_vec_[0] = this->stl_vec_[1];
  this->s21_vec_[0] = this->s21_vec_[1];

  EXPECT_EQ(this->stl_vec_[0], this->s21_vec_[0]);
  EXPECT_EQ(this->stl_vec_[1], this->s21_vec_[1]);

  // Test chained modifications
  this->stl_vec_[2] = this->stl_vec_[1] = this->stl_vec_[0];
  this->s21_vec_[2] = this->s21_vec_[1] = this->s21_vec_[0];

  EXPECT_EQ(this->stl_vec_[0], this->s21_vec_[0]);
  EXPECT_EQ(this->stl_vec_[1], this->s21_vec_[1]);
  EXPECT_EQ(this->stl_vec_[2], this->s21_vec_[2]);
}

// #if 0

// Additional edge cases for reserve()
TYPED_TEST(VectorTest, ReserveEdgeCases) {
  // Reserve smaller capacity than current
  size_t original_capacity = this->s21_vec_.capacity();
  this->s21_vec_.reserve(1);
  EXPECT_EQ(this->s21_vec_.capacity(), original_capacity);  // Shouldn't shrink

  // Reserve zero
  this->s21_vec_.reserve(0);
  EXPECT_GE(this->s21_vec_.capacity(), this->s21_vec_.size());
}

// insert
TEST(VectorTest, InsertSingleElement) {
  // Вставка элемента в пустой вектор
  s21::vector<int> s21_vec;
  s21_vec.insert(s21_vec.begin(), 324);
  EXPECT_EQ(s21_vec.size(), 1);
  EXPECT_EQ(s21_vec[0], 324);
}

TEST(VectorTest, InsertMultipleElement) {
  // Вставка элементов в пустой вектор
  s21::vector<int> s21_vec1;
  s21::vector<int> s21_vec2{7, 7, 7, 7, 7, 7, 7};
  s21::vector<int>::iterator it = s21_vec2.begin();
  s21_vec1.insert(s21_vec1.begin(), 7, 7);
  EXPECT_EQ(s21_vec1.size(), s21_vec2.size());
  for (auto i : s21_vec1) {
    EXPECT_EQ(i, *it);
    *it++;
  }
}

TYPED_TEST(VectorTest, MinMaxSort) {
  std::random_device rd;   // Create a random device for seed
  std::mt19937 gen(rd());  // Create a Mersenne Twister generator

  EXPECT_EQ(*std::min_element(this->s21_vec_.begin(), this->s21_vec_.end()),
            *std::min_element(this->stl_vec_.begin(), this->stl_vec_.end()));
  std::sort(this->s21_vec_.begin(), this->s21_vec_.end());
  std::sort(this->stl_vec_.begin(), this->stl_vec_.end());
  EXPECT_EQ(this->s21_vec_, this->stl_vec_);

  EXPECT_EQ(*std::max_element(this->s21_vec_.begin(), this->s21_vec_.end()),
            *std::max_element(this->stl_vec_.begin(), this->stl_vec_.end()));

  std::shuffle(this->s21_vec_.begin(), this->s21_vec_.end(), gen);
  std::shuffle(this->stl_vec_.begin(), this->stl_vec_.end(), gen);

  std::stable_sort(this->s21_vec_.begin(), this->s21_vec_.end());
  std::stable_sort(this->stl_vec_.begin(), this->stl_vec_.end());
  EXPECT_EQ(this->s21_vec_, this->stl_vec_);

  EXPECT_EQ(std::binary_search(this->s21_vec_.begin(), this->s21_vec_.end(),
                               this->s21_vec_[2]),
            std::binary_search(this->stl_vec_.begin(), this->stl_vec_.end(),
                               this->s21_vec_[2]));
}

// std::max();
// std::sort();
// #endif
