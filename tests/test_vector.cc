#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#include <gtest/gtest.h>
#pragma GCC diagnostic pop

#include <algorithm>
#include <cstring>
#include <vector>

#include "dummy_object.h"
#include "random.h"
#include "s21_vector.h"

template <typename T>
class VectorTest : public testing::Test {
 protected:
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

using TestedTypes = ::testing::Types<char, int, double, DummyObject>;
TYPED_TEST_SUITE(VectorTest, TestedTypes, );

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
  EXPECT_TRUE(this->s21_vec_.empty());
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
  EXPECT_GE(this->s21_vec_.capacity(), size_t{100});
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

TEST(VectorTestNonTyped, PushBackEmptyVector) {
  std::vector<int> stl_v;
  s21::vector<int> s21_v;

  stl_v.push_back(42);
  s21_v.push_back(42);

  EXPECT_EQ(stl_v.size(), s21_v.size());
  EXPECT_EQ(stl_v.capacity(), s21_v.capacity());
  EXPECT_EQ(stl_v[0], s21_v[0]);
}

TEST(VectorTestNonTyped, PushBackMultipleElements) {
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

TEST(VectorTestNonTyped, PushBackStringElements) {
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

TEST(VectorTestNonTyped, PushBackAfterReserve) {
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

TEST(VectorTestNonTyped, PushBackAfterShrinkToFit) {
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

TYPED_TEST(VectorTest, PopBack) {
  this->stl_vec_.pop_back();
  this->s21_vec_.pop_back();

  EXPECT_EQ(this->stl_vec_.size(), this->s21_vec_.size());
  EXPECT_EQ(this->stl_vec_.back(), this->s21_vec_.back());
}

TYPED_TEST(VectorTest, Clear) {
  this->s21_vec_.clear();
  EXPECT_TRUE(this->s21_vec_.empty());
  EXPECT_EQ(this->s21_vec_.size(), size_t{0});
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

  this->stl_vec_[2] = this->stl_vec_[1] = this->stl_vec_[0];
  this->s21_vec_[2] = this->s21_vec_[1] = this->s21_vec_[0];

  EXPECT_EQ(this->stl_vec_[0], this->s21_vec_[0]);
  EXPECT_EQ(this->stl_vec_[1], this->s21_vec_[1]);
  EXPECT_EQ(this->stl_vec_[2], this->s21_vec_[2]);
}

// Additional edge cases for reserve()
TYPED_TEST(VectorTest, ReserveEdgeCases) {
  size_t original_capacity = this->s21_vec_.capacity();
  this->s21_vec_.reserve(1);
  EXPECT_EQ(this->s21_vec_.capacity(), original_capacity);

  this->s21_vec_.reserve(0);
  EXPECT_GE(this->s21_vec_.capacity(), this->s21_vec_.size());
}

// insert
TEST(VectorTestNonTyped, InsertSingleElement) {
  s21::vector<int> s21_vec;
  s21_vec.insert(s21_vec.begin(), 324);
  EXPECT_EQ(s21_vec.size(), size_t{1});
  EXPECT_EQ(s21_vec[0], 324);
}

TEST(VectorTestNonTyped, InsertMultipleElements) {
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
  std::vector<TypeParam> source{1, 2, 3};
  this->s21_vec_.insert(this->s21_vec_.begin(), source.begin(), source.end());
  this->stl_vec_.insert(this->stl_vec_.begin(), source.begin(), source.end());
  EXPECT_EQ(this->s21_vec_.size(), this->stl_vec_.size());
  EXPECT_TRUE(std::equal(this->s21_vec_.begin(), this->s21_vec_.end(),
                         this->stl_vec_.begin()));

  std::vector<TypeParam> source2{4, 5};
  auto mid_pos_s21 = this->s21_vec_.begin() + 2;
  auto mid_pos_stl = this->stl_vec_.begin() + 2;
  this->s21_vec_.insert(mid_pos_s21, source2.begin(), source2.end());
  this->stl_vec_.insert(mid_pos_stl, source2.begin(), source2.end());
  EXPECT_EQ(this->s21_vec_.size(), this->stl_vec_.size());
  EXPECT_TRUE(std::equal(this->s21_vec_.begin(), this->s21_vec_.end(),
                         this->stl_vec_.begin()));

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
  std::vector<TypeParam> large_source(1000, TypeParam{42});

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

TYPED_TEST(VectorTest, InsertManyFront) {
  this->s21_vec_.insert_many(this->s21_vec_.begin(), TypeParam{45},
                             TypeParam{56}, TypeParam{67});
  this->stl_vec_.insert(this->stl_vec_.begin(),
                        {TypeParam{45}, TypeParam{56}, TypeParam{67}});

  EXPECT_EQ(this->s21_vec_.size(), this->stl_vec_.size());
  EXPECT_TRUE(std::equal(this->s21_vec_.begin(), this->s21_vec_.end(),
                         this->stl_vec_.begin()));
}

TYPED_TEST(VectorTest, InsertManyMiddle) {
  auto mid_pos_s21 = this->s21_vec_.begin() + 1;
  auto mid_pos_stl = this->stl_vec_.begin() + 1;
  this->s21_vec_.insert_many(mid_pos_s21, TypeParam{45}, TypeParam{56},
                             TypeParam{67});
  this->stl_vec_.insert(mid_pos_stl,
                        {TypeParam{45}, TypeParam{56}, TypeParam{67}});

  EXPECT_EQ(this->s21_vec_.size(), this->stl_vec_.size());
  EXPECT_TRUE(std::equal(this->s21_vec_.begin(), this->s21_vec_.end(),
                         this->stl_vec_.begin()));
}

TYPED_TEST(VectorTest, InsertManyBack) {
  this->s21_vec_.insert_many(this->s21_vec_.end(), TypeParam{45}, TypeParam{56},
                             TypeParam{67});
  this->stl_vec_.insert(this->stl_vec_.end(),
                        {TypeParam{45}, TypeParam{56}, TypeParam{67}});

  EXPECT_EQ(this->s21_vec_.size(), this->stl_vec_.size());
  EXPECT_TRUE(std::equal(this->s21_vec_.begin(), this->s21_vec_.end(),
                         this->stl_vec_.begin()));
}

TYPED_TEST(VectorTest, InsertManyEmpty) {
  auto original_size = this->s21_vec_.size();
  this->s21_vec_.insert_many(this->s21_vec_.begin());
  EXPECT_EQ(this->s21_vec_.size(), original_size);
}

TYPED_TEST(VectorTest, InsertManyBack_2) {
  this->s21_vec_.insert_many_back(TypeParam{45}, TypeParam{56}, TypeParam{67});
  this->stl_vec_.insert(this->stl_vec_.end(),
                        {TypeParam{45}, TypeParam{56}, TypeParam{67}});

  EXPECT_EQ(this->s21_vec_.size(), this->stl_vec_.size());
  EXPECT_TRUE(std::equal(this->s21_vec_.begin(), this->s21_vec_.end(),
                         this->stl_vec_.begin()));
}

TYPED_TEST(VectorTest, InsertManyBackEmpty_2) {
  auto original_size = this->s21_vec_.size();
  this->s21_vec_.insert_many_back();
  EXPECT_EQ(this->s21_vec_.size(), original_size);
}

TYPED_TEST(VectorTest, InsertManyBackWithExistingElements_2) {
  this->s21_vec_.insert_many_back(TypeParam{1}, TypeParam{2}, TypeParam{3});
  this->s21_vec_.insert_many_back(TypeParam{4}, TypeParam{5});

  this->stl_vec_.insert(this->stl_vec_.end(),
                        {TypeParam{1}, TypeParam{2}, TypeParam{3}});
  this->stl_vec_.insert(this->stl_vec_.end(), {TypeParam{4}, TypeParam{5}});

  EXPECT_EQ(this->s21_vec_.size(), this->stl_vec_.size());
  EXPECT_TRUE(std::equal(this->s21_vec_.begin(), this->s21_vec_.end(),
                         this->stl_vec_.begin()));
}

// erase

TEST(VectorTestNonTyped, EraseSingleElement) {
  s21::vector<int> custom_vec;
  custom_vec.push_back(1);
  custom_vec.push_back(2);
  custom_vec.push_back(3);

  std::vector<int> std_vec = {1, 2, 3};

  auto custom_iterator = custom_vec.erase(custom_vec.begin() + 1);
  auto std_iterator = std_vec.erase(std_vec.begin() + 1);

  EXPECT_EQ(custom_vec.size(), std_vec.size());

  EXPECT_EQ(custom_vec[0], std_vec[0]);
  EXPECT_EQ(custom_vec[1], std_vec[1]);

  EXPECT_EQ(*custom_iterator, *std_iterator);
}

TEST(VectorTestNonTyped, EraseRange) {
  s21::vector<int> custom_vec;
  custom_vec.push_back(1);
  custom_vec.push_back(2);
  custom_vec.push_back(3);
  custom_vec.push_back(4);

  std::vector<int> std_vec = {1, 2, 3, 4};

  auto custom_iterator =
      custom_vec.erase(custom_vec.begin() + 1, custom_vec.begin() + 3);
  auto std_iterator = std_vec.erase(std_vec.begin() + 1, std_vec.begin() + 3);

  EXPECT_EQ(custom_vec.size(), std_vec.size());

  EXPECT_EQ(custom_vec[0], std_vec[0]);
  EXPECT_EQ(custom_vec[1], std_vec[1]);

  EXPECT_EQ(*custom_iterator, *std_iterator);
}

TEST(VectorTestNonTyped, EraseAtEnd) {
  s21::vector<int> custom_vec;
  custom_vec.push_back(1);
  custom_vec.push_back(2);

  std::vector<int> std_vec = {1, 2};

  auto custom_iterator = custom_vec.erase(custom_vec.begin() + 1);
  auto std_iterator = std_vec.erase(std_vec.begin() + 1);

  EXPECT_EQ(custom_vec.size(), std_vec.size());

  EXPECT_EQ(custom_vec[0], std_vec[0]);

  EXPECT_EQ(*custom_iterator, *std_iterator);
}

TEST(VectorTestNonTyped, EraseMultipleElements) {
  s21::vector<int> custom_vec;
  custom_vec.push_back(1);
  custom_vec.push_back(2);
  custom_vec.push_back(3);
  custom_vec.push_back(4);

  std::vector<int> std_vec = {1, 2, 3, 4};

  auto custom_iterator =
      custom_vec.erase(custom_vec.begin() + 1, custom_vec.begin() + 3);
  auto std_iterator = std_vec.erase(std_vec.begin() + 1, std_vec.begin() + 3);

  EXPECT_EQ(custom_vec.size(), std_vec.size());

  EXPECT_EQ(custom_vec[0], std_vec[0]);
  EXPECT_EQ(custom_vec[1], std_vec[1]);

  EXPECT_EQ(*custom_iterator, *std_iterator);
}

// swap
TEST(VectorTestNonTyped, SwapContent) {
  s21::vector<int> my_vector1{1, 2, 3};
  s21::vector<int> my_vector2{4, 5};

  std::vector<int> std_vector1{1, 2, 3};
  std::vector<int> std_vector2{4, 5};

  my_vector1.swap(my_vector2);
  std_vector1.swap(std_vector2);

  EXPECT_EQ(my_vector1, std_vector1);
  EXPECT_EQ(my_vector2, std_vector2);
}

TEST(VectorTestNonTyped, SwapEmptyAndNonEmpty) {
  s21::vector<int> my_vector1{};
  s21::vector<int> my_vector2{1, 2, 3};

  std::vector<int> std_vector1{};
  std::vector<int> std_vector2{1, 2, 3};

  my_vector1.swap(my_vector2);
  std_vector1.swap(std_vector2);

  EXPECT_EQ(my_vector1, std_vector1);
  EXPECT_EQ(my_vector2, std_vector2);
}

TEST(VectorTestNonTyped, SwapIdenticalVectors) {
  s21::vector<int> my_vector1{1, 2, 3};
  s21::vector<int> my_vector2{1, 2, 3};

  std::vector<int> std_vector1{1, 2, 3};
  std::vector<int> std_vector2{1, 2, 3};

  my_vector1.swap(my_vector2);
  std_vector1.swap(std_vector2);

  EXPECT_EQ(my_vector1, std_vector1);
  EXPECT_EQ(my_vector2, std_vector2);
}

TEST(VectorTestNonTyped, SwapWithSelf) {
  s21::vector<int> my_vector{1, 2, 3};
  std::vector<int> std_vector{1, 2, 3};

  my_vector.swap(my_vector);
  std_vector.swap(std_vector);

  EXPECT_EQ(my_vector, std_vector);
}

TEST(VectorTestNonTyped, SwapLargeVectors) {
  s21::vector<int> my_vector1(100, 1);
  s21::vector<int> my_vector2(200, 2);

  std::vector<int> std_vector1(100, 1);
  std::vector<int> std_vector2(200, 2);

  my_vector1.swap(my_vector2);
  std_vector1.swap(std_vector2);

  EXPECT_EQ(my_vector1, std_vector1);
  EXPECT_EQ(my_vector2, std_vector2);
}

// iterator
TYPED_TEST(VectorTest, ComparisonOperators) {
  typename s21::vector<TypeParam>::iterator iter1(this->large_s21_vec_.begin());
  typename s21::vector<TypeParam>::iterator iter2(this->large_s21_vec_.begin() +
                                                  2);

  EXPECT_TRUE(iter1 < iter2);
  EXPECT_FALSE(iter2 < iter1);
  EXPECT_TRUE(iter2 > iter1);
  EXPECT_FALSE(iter1 > iter2);
  EXPECT_TRUE(iter1 <= iter1);
  EXPECT_TRUE(iter1 <= iter2);
  EXPECT_FALSE(iter2 <= iter1);
  EXPECT_TRUE(iter2 >= iter2);
  EXPECT_TRUE(iter2 >= iter1);
  EXPECT_FALSE(iter1 >= iter2);
}

TYPED_TEST(VectorTest, ComparisonOperators2) {
  typename s21::vector<TypeParam>::iterator iter1(this->large_s21_vec_.end());
  typename s21::vector<TypeParam>::iterator iter2(this->large_s21_vec_.end() -
                                                  2);

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

TEST(VectorTestNonTyped, ArithmeticOperators) {
  int arr[] = {1, 2, 3, 4};
  s21::vector<int>::iterator iter(arr + 1);

  EXPECT_EQ(*(iter - 1), 1);
  iter -= 1;
  EXPECT_EQ(*iter, 1);

  s21::vector<int>::iterator iter2 = 2 + iter;
  EXPECT_EQ(*iter2, 3);
}

TEST(VectorTestNonTyped, PostDecrement) {
  int arr[] = {1, 2, 3, 4};
  s21::vector<int>::iterator iter(arr + 2);

  s21::vector<int>::iterator temp = iter--;
  EXPECT_EQ(*temp, 3);
  EXPECT_EQ(*iter, 2);
}

TEST(VectorTestNonTyped, SubscriptOperator) {
  int arr[] = {10, 20, 30, 40, 50};
  s21::vector<int>::iterator iter(arr);

  EXPECT_EQ(iter[0], 10);
  EXPECT_EQ(iter[1], 20);
  EXPECT_EQ(iter[2], 30);

  iter[0] = 100;
  EXPECT_EQ(arr[0], 100);
}

TEST(VectorTestNonTyped, EqualVectors) {
  std::vector<int> std_vec = {1, 2, 3, 4, 5};
  s21::vector<int> s21_vec;
  for (int i : std_vec) {
    s21_vec.push_back(i);
  }

  EXPECT_TRUE(std_vec == s21_vec);
  EXPECT_TRUE(s21_vec == std_vec);
}

TEST(VectorTestNonTyped, ReverseIterators) {
  s21::vector<int> my_vec{1, 2, 3, 4};
  auto rbegin = my_vec.rbegin();
  auto rend = my_vec.rend();

  std::vector<int> std_vec = {1, 2, 3, 4};
  auto std_rbegin = std_vec.rbegin();
  // auto std_rend = std_vec.rend();

  for (; rbegin != rend; ++rbegin, ++std_rbegin) {
    EXPECT_EQ(*rbegin, *std_rbegin);
  }
}

TEST(VectorTestNonTyped, CopyAssignmentOperator) {
  s21::vector<int> vec1{1, 2, 3};
  s21::vector<int> vec2;

  vec2 = vec1;

  EXPECT_EQ(vec1, vec2);
}

TEST(VectorTestNonTyped, MoveAssignmentOperator) {
  s21::vector<int> vec1{1, 2, 3};
  s21::vector<int> vec2;

  vec2 = std::move(vec1);

  EXPECT_EQ(vec2.size(), size_t{3});
  EXPECT_EQ(vec2[0], 1);
  EXPECT_EQ(vec2[1], 2);
  EXPECT_EQ(vec2[2], 3);
  EXPECT_EQ(vec1.size(), size_t{0});
}

TEST(VectorTestNonTyped, EqualityOperator) {
  s21::vector<int> vec1{1, 2, 3};
  s21::vector<int> vec2{1, 2, 3};
  s21::vector<int> vec3{1, 2, 4};

  EXPECT_TRUE(vec1 == vec2);
  EXPECT_FALSE(vec1 == vec3);
}

TEST(VectorTestNonTyped, ComparisonWithstd_vector) {
  std::vector<int> std_vec{1, 2, 3};
  s21::vector<int> my_vec{1, 2, 3};

  EXPECT_TRUE(my_vec == std_vec);
  my_vec[2] = 4;
  EXPECT_FALSE(my_vec == std_vec);
}

TEST(VectorTestNonTyped, DataVector) {
  std::vector<int> std_vec{1, 2, 3, 4, 5};
  s21::vector<int> s21_vec{1, 2, 3, 4, 5};
  int* data_s21 = s21_vec.data();
  int* data_std = std_vec.data();

  EXPECT_TRUE(*data_s21 == *data_std);
}

TEST(VectorTestNonTyped, DefaultConstructor) {
  s21::vector<int> v;
  EXPECT_EQ(v.size(), size_t{0});
  EXPECT_EQ(v.capacity(), size_t{0});
  EXPECT_NE(v.data(), nullptr);
}

TEST(VectorTestNonTyped, PositiveSizeConstructor) {
  s21::vector<int> v(10);
  EXPECT_EQ(v.size(), size_t{10});
  EXPECT_EQ(v.capacity(), size_t{10});
  EXPECT_NE(v.data(), nullptr);

  // Проверяем инициализацию значениями по умолчанию
  for (std::size_t i = 0; i < v.size(); ++i) {
    EXPECT_EQ(v[i], 0);
  }
}

TEST(VectorTestNonTyped, ZeroSizeConstructor) {
  s21::vector<int> v(0);
  EXPECT_EQ(v.size(), size_t{0});
  EXPECT_EQ(v.capacity(), size_t{0});
  EXPECT_NE(v.data(), nullptr);
}

TEST(VectorTestNonTyped, MaxSizeExceeded) {
  EXPECT_THROW(s21::vector<int>(
                   std::numeric_limits<std::size_t>::max() / sizeof(int) + 1),
               std::length_error);
}

TEST(VectorTestNonTyped, MemoryAllocationCheck) {
  s21::vector<int> v(5);
  auto* data = v.data();
  EXPECT_NE(data, nullptr);
}

TYPED_TEST(VectorTest, MinMaxSort) {
  EXPECT_EQ(*std::min_element(this->s21_vec_.begin(), this->s21_vec_.end()),
            *std::min_element(this->stl_vec_.begin(), this->stl_vec_.end()));
  std::sort(this->s21_vec_.begin(), this->s21_vec_.end());
  std::sort(this->stl_vec_.begin(), this->stl_vec_.end());
  EXPECT_EQ(this->s21_vec_, this->stl_vec_);

  EXPECT_EQ(*std::max_element(this->s21_vec_.begin(), this->s21_vec_.end()),
            *std::max_element(this->stl_vec_.begin(), this->stl_vec_.end()));

  std::shuffle(this->s21_vec_.begin(), this->s21_vec_.end(), Random::mt);
  std::shuffle(this->stl_vec_.begin(), this->stl_vec_.end(), Random::mt);

  std::stable_sort(this->s21_vec_.begin(), this->s21_vec_.end());
  std::stable_sort(this->stl_vec_.begin(), this->stl_vec_.end());
  EXPECT_EQ(this->s21_vec_, this->stl_vec_);

  EXPECT_EQ(std::binary_search(this->s21_vec_.begin(), this->s21_vec_.end(),
                               this->s21_vec_[2]),
            std::binary_search(this->stl_vec_.begin(), this->stl_vec_.end(),
                               this->s21_vec_[2]));
}

TEST(VectorTestNonTyped, EraseEmptyVector) {
  s21::vector<int> custom_vec;

  EXPECT_NO_THROW(custom_vec.erase(custom_vec.begin()));
}

TEST(VectorConstCorrectnessTest, ConstMemberAccess) {
  const s21::vector<int> const_vec{1, 2, 3};

  // Test const element access methods
  EXPECT_EQ(const_vec[0], 1);              // Test const operator[]
  EXPECT_EQ(const_vec.front(), 1);         // Test const front()
  EXPECT_EQ(const_vec.back(), 3);          // Test const back()
  EXPECT_EQ(const_vec.size(), size_t{3});  // Test const size()
  EXPECT_FALSE(const_vec.empty());         // Test const empty()
}

TEST(VectorConstCorrectnessTest, ConstIterators) {
  const s21::vector<int> const_vec{1, 2, 3};

  // Test const_iterator functionality
  s21::vector<int>::const_iterator it = const_vec.begin();
  EXPECT_EQ(*it, 1);

  // Verify iteration works with const_iterator
  int expected = 1;
  for (auto it = const_vec.begin(); it != const_vec.end(); ++it) {
    EXPECT_EQ(*it, expected++);
  }

  // Verify const_iterator comparison operators
  auto it1 = const_vec.begin();
  auto it2 = const_vec.begin() + 1;
  EXPECT_TRUE(it1 < it2);
  EXPECT_FALSE(it1 > it2);
  EXPECT_TRUE(it1 <= it2);
  EXPECT_FALSE(it1 >= it2);
}

TEST(VectorConstCorrectnessTest, ConstVectorAssignment) {
  const s21::vector<int> const_source{1, 2, 3};
  s21::vector<int> target;

  // Test assignment from const vector
  target = const_source;
  EXPECT_EQ(target.size(), const_source.size());
  for (size_t i = 0; i < target.size(); ++i) {
    EXPECT_EQ(target[i], const_source[i]);
  }
}

TEST(VectorConstCorrectnessTest, ConstAt) {
  const s21::vector<int> const_vec{1, 2, 3};

  // Test const at() method
  EXPECT_EQ(const_vec.at(0), 1);
  EXPECT_EQ(const_vec.at(1), 2);
  EXPECT_EQ(const_vec.at(2), 3);
  EXPECT_THROW(const_vec.at(3), std::out_of_range);
}

TEST(VectorConstCorrectnessTest, ConstIteratorArithmetic) {
  const s21::vector<int> const_vec{1, 2, 3, 4, 5};

  // Test const_iterator arithmetic operations
  auto it = const_vec.begin();
  EXPECT_EQ(*(it + 2), 3);
  EXPECT_EQ(*(it + 4), 5);

  it += 2;
  EXPECT_EQ(*it, 3);

  EXPECT_EQ(it[-1], 2);
  EXPECT_EQ(it[1], 4);
}

TEST(VectorConstCorrectnessTest, ConstData) {
  const s21::vector<int> const_vec{1, 2, 3};

  // Test const data() method
  const int* data = const_vec.data();
  EXPECT_EQ(data[0], 1);
  EXPECT_EQ(data[1], 2);
  EXPECT_EQ(data[2], 3);
}

TEST(VectorConstCorrectnessTest, ComparisonWithConstVector) {
  const s21::vector<int> const_vec1{1, 2, 3};
  const s21::vector<int> const_vec2{1, 2, 3};
  const s21::vector<int> const_vec3{1, 2, 4};

  // Test comparison operators with const vectors
  EXPECT_TRUE(const_vec1 == const_vec2);
  EXPECT_FALSE(const_vec1 == const_vec3);
}

// This is a working example showing how to verify compile failures
TEST(VectorConstCorrectnessTest, NotCompiling) {
  // 1. This would not compile:

  // const s21::vector<int> const_vec{1, 2, 3};
  // s21::vector<int>::iterator it = const_vec.begin();

  // 2. This compiles fine (correct const usage):
  const s21::vector<int> const_vec{1, 2, 3};
  s21::vector<int>::const_iterator const_it = const_vec.begin();
  EXPECT_EQ(*const_it, 1);
}
