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

TEST(VectorTest, PushBackEmptyVector) {
  std::vector<int> stl_v;
  s21::vector<int> s21_v;

  stl_v.push_back(42);
  s21_v.push_back(42);

  EXPECT_EQ(stl_v.size(), s21_v.size());
  EXPECT_EQ(stl_v.capacity(), s21_v.capacity());
  EXPECT_EQ(stl_v[0], s21_v[0]);
}

TEST(VectorTest, PushBackMultipleElements) {
  std::vector<int> stl_v;
  s21::vector<int> s21_v;

  for (int i = 0; i < 100; ++i) {
    stl_v.push_back(i);
    s21_v.push_back(i);

    EXPECT_EQ(stl_v.size(), s21_v.size());
    EXPECT_EQ(stl_v.capacity(), s21_v.capacity());
    for (size_t j = 0; j < stl_v.size(); ++j) {
      EXPECT_EQ(stl_v[j], s21_v[j]);
    }
  }
}

TEST(VectorTest, PushBackStringElements) {
  std::vector<std::string> stl_v;
  s21::vector<std::string> s21_v;

  stl_v.push_back("hello");
  s21_v.push_back("hello");

  stl_v.push_back("world");
  s21_v.push_back("world");

  EXPECT_EQ(stl_v.size(), s21_v.size());
  EXPECT_EQ(stl_v.capacity(), s21_v.capacity());
  EXPECT_EQ(stl_v[0], s21_v[0]);
  EXPECT_EQ(stl_v[1], s21_v[1]);
}

TEST(VectorTest, PushBackCustomObjects) {
  struct CustomObject {
    int a;
    double b;
    bool operator==(const CustomObject& other) const {
      return a == other.a && b == other.b;
    }
  };

  std::vector<CustomObject> stl_v;
  s21::vector<CustomObject> s21_v;

  CustomObject obj1{1, 1.1};
  CustomObject obj2{2, 2.2};

  stl_v.push_back(obj1);
  s21_v.push_back(obj1);
  stl_v.push_back(obj2);
  s21_v.push_back(obj2);

  EXPECT_EQ(stl_v.size(), s21_v.size());
  EXPECT_EQ(stl_v.capacity(), s21_v.capacity());
  EXPECT_EQ(stl_v[0], s21_v[0]);
  EXPECT_EQ(stl_v[1], s21_v[1]);
}

TEST(VectorTest, PushBackAfterReserve) {
  std::vector<int> stl_v;
  s21::vector<int> s21_v;

  stl_v.reserve(50);
  s21_v.reserve(50);

  int num_ = 0;

  for (size_t i = 0; i < 50; ++i) {
    stl_v.push_back(num_);
    s21_v.push_back(num_);

    num_++;

    EXPECT_EQ(stl_v.size(), s21_v.size());
    EXPECT_EQ(stl_v.capacity(), s21_v.capacity());
    EXPECT_EQ(stl_v[i], s21_v[i]);
  }
}

TEST(VectorTest, PushBackAfterShrinkToFit) {
  std::vector<int> stl_v(100, 42);
  s21::vector<int> s21_v(100, 42);

  stl_v.shrink_to_fit();
  s21_v.shrink_to_fit();

  stl_v.push_back(24);
  s21_v.push_back(24);

  EXPECT_EQ(stl_v.size(), s21_v.size());
  EXPECT_EQ(stl_v.capacity(), s21_v.capacity());
  EXPECT_EQ(stl_v.back(), s21_v.back());
}

///

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

// insert
TEST(VectorTest, InsertSingleElement) {
  // Вставка элемента в пустой вектор
  s21::vector<int> s21_vec;
  s21_vec.insert(s21_vec.begin(), 324);
  EXPECT_EQ(s21_vec.size(), 1);
  EXPECT_EQ(s21_vec[0], 324);
}

TEST(VectorTest, InsertMultipleElements) {
  // Вставка элементов в пустой вектор
  s21::vector<int> s21_vec1;
  s21::vector<int> s21_vec2{7, 7, 7, 7, 7, 7, 7};
  s21::vector<int>::iterator it = s21_vec2.begin();
  s21_vec1.insert(s21_vec1.begin(), size_t{7}, 7);
  EXPECT_EQ(s21_vec1.size(), s21_vec2.size());
  for (auto i : s21_vec1) {
    EXPECT_EQ(i, *it);
    *it++;
  }
}

TYPED_TEST(VectorTest, InsertIteratorRange) {
  // Test inserting at beginning
  std::vector<TypeParam> source{1, 2, 3};
  this->s21_vec_.insert(this->s21_vec_.begin(), source.begin(), source.end());
  this->stl_vec_.insert(this->stl_vec_.begin(), source.begin(), source.end());
  EXPECT_EQ(this->s21_vec_.size(), this->stl_vec_.size());
  EXPECT_TRUE(std::equal(this->s21_vec_.begin(), this->s21_vec_.end(),
                         this->stl_vec_.begin()));

  // Test inserting at middle
  std::vector<TypeParam> source2{4, 5};
  auto mid_pos_s21 = this->s21_vec_.begin() + 2;
  auto mid_pos_stl = this->stl_vec_.begin() + 2;
  this->s21_vec_.insert(mid_pos_s21, source2.begin(), source2.end());
  this->stl_vec_.insert(mid_pos_stl, source2.begin(), source2.end());
  EXPECT_EQ(this->s21_vec_.size(), this->stl_vec_.size());
  EXPECT_TRUE(std::equal(this->s21_vec_.begin(), this->s21_vec_.end(),
                         this->stl_vec_.begin()));

  // Test inserting at end
  std::vector<TypeParam> source3{6};
  this->s21_vec_.insert(this->s21_vec_.end(), source3.begin(), source3.end());
  this->stl_vec_.insert(this->stl_vec_.end(), source3.begin(), source3.end());
  EXPECT_EQ(this->s21_vec_.size(), this->stl_vec_.size());
  EXPECT_TRUE(std::equal(this->s21_vec_.begin(), this->s21_vec_.end(),
                         this->stl_vec_.begin()));
}

TYPED_TEST(VectorTest, InsertEmptyRange) {
  std::vector<TypeParam> empty_source;
  auto original_size = this->s21_vec_.size();

  this->s21_vec_.insert(this->s21_vec_.begin(), empty_source.begin(),
                        empty_source.end());
  EXPECT_EQ(this->s21_vec_.size(), original_size);
}

TYPED_TEST(VectorTest, InsertLargeRange) {
  // Create large source vector
  std::vector<TypeParam> large_source(1000, TypeParam{42});

  // Insert at beginning
  this->s21_vec_.insert(this->s21_vec_.begin(), large_source.begin(),
                        large_source.end());
  this->stl_vec_.insert(this->stl_vec_.begin(), large_source.begin(),
                        large_source.end());

  EXPECT_EQ(this->s21_vec_.size(), this->stl_vec_.size());
  EXPECT_TRUE(std::equal(this->s21_vec_.begin(), this->s21_vec_.end(),
                         this->stl_vec_.begin()));
}

TYPED_TEST(VectorTest, InsertIntoEmptyVector) {
  std::vector<TypeParam> source{1, 2, 3, 4, 5};

  this->empty_s21_vec_.insert(this->empty_s21_vec_.begin(), source.begin(),
                              source.end());
  this->empty_stl_vec_.insert(this->empty_stl_vec_.begin(), source.begin(),
                              source.end());

  EXPECT_EQ(this->empty_s21_vec_.size(), this->empty_stl_vec_.size());
  EXPECT_TRUE(std::equal(this->empty_s21_vec_.begin(),
                         this->empty_s21_vec_.end(),
                         this->empty_stl_vec_.begin()));
}

// std::max();
// std::sort();
#if 0
#endif
