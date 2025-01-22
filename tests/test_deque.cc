#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#include <gtest/gtest.h>
#pragma GCC diagnostic pop

#include <algorithm>
#include <deque>

#include "dummy_object.h"
#include "random.h"
#include "s21_deque.h"

template <typename T>
class DequeTest : public testing::Test {
 protected:
  DequeTest()
      : empty_stl_deque_(),
        empty_s21_deque_(),
        stl_deque_{68, 3, 86},
        s21_deque_{68, 3, 86},
        large_stl_deque_(10000, 42),
        large_s21_deque_(10000, 42) {}

  std::deque<T> empty_stl_deque_{};
  s21::deque<T> empty_s21_deque_{};
  std::deque<T> stl_deque_{};
  s21::deque<T> s21_deque_{};
  std::deque<T> large_stl_deque_{};
  s21::deque<T> large_s21_deque_{};
};

using TestedTypes = ::testing::Types<char, int, double, DummyObject>;
TYPED_TEST_SUITE(DequeTest, TestedTypes, );

TYPED_TEST(DequeTest, First) {
  TypeParam value{};
  for (int i{0}; i < 70; i++) {
    (this->empty_s21_deque_).push_front(value);
    (this->empty_stl_deque_).push_front(value);
  }
  EXPECT_EQ(this->empty_stl_deque_.size(), this->empty_s21_deque_.size());
}

TYPED_TEST(DequeTest, Second) {
  TypeParam value{};
  for (int i{0}; i < 70; i++) {
    (this->empty_s21_deque_).push_back(value);
    (this->empty_stl_deque_).push_back(value);
  }
  EXPECT_EQ(this->empty_stl_deque_.size(), this->empty_s21_deque_.size());
}

// push_back
TYPED_TEST(DequeTest, PushBackBasic) {
  std::deque<int> std_q;
  s21::deque<int> s21_q;

  TypeParam value{};

  this->empty_s21_deque_.push_back(value);
  this->empty_stl_deque_.push_back(value);
  EXPECT_EQ(this->empty_s21_deque_.back(), this->empty_stl_deque_.back());

  this->empty_s21_deque_.push_back(value);
  this->empty_stl_deque_.push_back(value);
  EXPECT_EQ(this->empty_s21_deque_.back(), this->empty_stl_deque_.back());

  this->empty_s21_deque_.push_back(value);
  this->empty_stl_deque_.push_back(value);
  EXPECT_EQ(this->empty_s21_deque_.back(), this->empty_stl_deque_.back());

  EXPECT_EQ(this->empty_s21_deque_.size(), this->empty_stl_deque_.size());

  for (size_t i = 0; i < this->empty_stl_deque_.size(); ++i) {
    EXPECT_EQ(this->empty_s21_deque_[i], this->empty_stl_deque_[i]);
  }
}

TYPED_TEST(DequeTest, PushBackChunkOverflow) {
  TypeParam value{};

  for (size_t i = 0; i < 15; ++i) {
    this->empty_s21_deque_.push_back(value);
  }

  for (size_t i = 0; i < 15; ++i) {
    EXPECT_EQ(this->empty_s21_deque_[i], value);
  }

  EXPECT_EQ(this->empty_s21_deque_.size(), size_t{15});
}

TYPED_TEST(DequeTest, PushBackGrowMap) {
  TypeParam value{};

  for (size_t i = 0; i < 100; ++i) {
    this->empty_s21_deque_.push_back(value);
  }

  for (size_t i = 0; i < 100; ++i) {
    EXPECT_EQ(this->empty_s21_deque_[i], value);
  }

  EXPECT_EQ(this->empty_s21_deque_.size(), size_t{100});
}

TYPED_TEST(DequeTest, PushBackEmptyDeque) {
  TypeParam value{};

  this->empty_s21_deque_.push_back(value);
  EXPECT_EQ(this->empty_s21_deque_.size(), size_t{1});
  EXPECT_EQ(this->empty_s21_deque_.back(), value);
  EXPECT_EQ(this->empty_s21_deque_.front(), value);
}

// push_front
TYPED_TEST(DequeTest, PushFrontBasic) {
  std::deque<int> std_q;
  s21::deque<int> s21_q;

  TypeParam value{};

  this->empty_s21_deque_.push_front(value);
  this->empty_stl_deque_.push_front(value);
  EXPECT_EQ(this->empty_s21_deque_.front(), this->empty_stl_deque_.front());

  this->empty_s21_deque_.push_front(value);
  this->empty_stl_deque_.push_front(value);
  EXPECT_EQ(this->empty_s21_deque_.front(), this->empty_stl_deque_.front());

  this->empty_s21_deque_.push_front(value);
  this->empty_stl_deque_.push_front(value);
  EXPECT_EQ(this->empty_s21_deque_.front(), this->empty_stl_deque_.front());

  EXPECT_EQ(this->empty_s21_deque_.size(), this->empty_stl_deque_.size());

  for (size_t i = 0; i < this->empty_stl_deque_.size(); ++i) {
    EXPECT_EQ(this->empty_s21_deque_[i], this->empty_stl_deque_[i]);
  }
}

TYPED_TEST(DequeTest, PushFrontChunkOverflow) {
  TypeParam value{};

  for (size_t i = 0; i < 10000; ++i) {
    this->empty_s21_deque_.push_front(value);
  }

  for (size_t i = 0; i < 1000; ++i) {
    EXPECT_EQ(this->empty_s21_deque_[i], value);
  }

  EXPECT_EQ(this->empty_s21_deque_.size(), size_t{10000});
}

TYPED_TEST(DequeTest, PushFrontGrowMap) {
  TypeParam value{};

  for (size_t i = 0; i < 100; ++i) {
    this->empty_s21_deque_.push_front(value);
  }

  for (size_t i = 0; i < 100; ++i) {
    EXPECT_EQ(this->empty_s21_deque_[i], value);
  }

  EXPECT_EQ(this->empty_s21_deque_.size(), size_t{100});
}

TYPED_TEST(DequeTest, PushFrontEmptyDeque) {
  TypeParam value{};

  this->empty_s21_deque_.push_front(value);
  EXPECT_EQ(this->empty_s21_deque_.size(), size_t{1});
  EXPECT_EQ(this->empty_s21_deque_.back(), value);
  EXPECT_EQ(this->empty_s21_deque_.front(), value);
}

// iterartor
TEST(DequeNonTyped, ComparisonOperators) {
  s21::deque<int> arr{1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7};
  s21::deque<int>::iterator iter1 = arr.begin();
  s21::deque<int>::iterator iter2 = arr.begin();

  iter2 += 2;

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

TEST(DequeNonTyped, ArithmeticOperators) {
  s21::deque<int> arr{1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7};
  s21::deque<int>::iterator iter = arr.begin();

  iter += 2;
  EXPECT_EQ(*(iter - 1), 2);
  iter -= 1;
  EXPECT_EQ(*iter, 2);
}

TEST(DequeNonTyped, PostDecrement) {
  s21::deque<int> arr{1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7};
  s21::deque<int>::iterator iter = arr.begin();

  iter += 2;

  s21::deque<int>::iterator temp = iter--;
  EXPECT_EQ(*temp, 3);
  EXPECT_EQ(*iter, 2);
}

TEST(DequeTest, SubscriptOperator) {
  s21::deque<int> arr{1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7};
  s21::deque<int>::iterator iter = arr.begin();

  EXPECT_EQ(*iter, 1);
  EXPECT_EQ(*(iter + 1), 2);
  EXPECT_EQ(*(iter + 2), 3);

  *iter = 100;
  EXPECT_EQ(arr[0], 100);
}

// InitializerListConstructor
TEST(DequeNonTyped, InitializerListConstructor) {
  int arr[] = {10, 20, 30, 40, 50};
  s21::deque<int> q{10, 20, 30, 40, 50};

  EXPECT_EQ(q[0], arr[0]);
  EXPECT_EQ(q[1], arr[1]);
  EXPECT_EQ(q[2], arr[2]);
}

// at
TYPED_TEST(DequeTest, At_1) { EXPECT_EQ(this->s21_deque_, this->stl_deque_); }

TYPED_TEST(DequeTest, At_2) {
  TypeParam value{};

  for (size_t i = 0; i < 2; ++i) {
    this->large_s21_deque_.push_back(value);
    this->large_stl_deque_.push_back(value);
  }

  EXPECT_EQ(this->large_s21_deque_, this->large_stl_deque_);

  TypeParam value_1{};

  this->large_s21_deque_.at(0) = value_1;
  this->large_stl_deque_.at(0) = value_1;
  EXPECT_EQ(this->large_s21_deque_.at(0), this->large_stl_deque_.at(0));

  TypeParam value_2{};

  this->large_s21_deque_.at(4) = value_2;
  this->large_stl_deque_.at(4) = value_2;
  EXPECT_EQ(this->large_s21_deque_.at(4), this->large_stl_deque_.at(4));
}

TYPED_TEST(DequeTest, AtOutOfRange) {
  TypeParam value{};

  for (size_t i = 0; i < 5; ++i) {
    this->empty_s21_deque_.push_back(value);
  }

  EXPECT_THROW(this->empty_s21_deque_.at(5), std::out_of_range);
  EXPECT_THROW(this->empty_s21_deque_.at(10), std::out_of_range);
  EXPECT_NO_THROW(this->empty_s21_deque_.at(4));
}

TYPED_TEST(DequeTest, AtEmptyDeque) {
  EXPECT_THROW(this->empty_s21_deque_.at(0), std::out_of_range);
}

// pop_back
TYPED_TEST(DequeTest, PopBack_1) {
  this->large_s21_deque_.pop_back();
  this->large_stl_deque_.pop_back();

  EXPECT_EQ(this->large_s21_deque_, this->large_stl_deque_);
}

TYPED_TEST(DequeTest, PopBack_2) {
  TypeParam value{};

  for (size_t i = 0; i < 10000; ++i) {
    this->empty_s21_deque_.push_front(value);
    this->empty_stl_deque_.push_front(value);
  }

  for (size_t i = 0; i < 1000; ++i) {
    this->empty_s21_deque_.pop_back();
    this->empty_stl_deque_.pop_back();
  }

  EXPECT_EQ(this->empty_s21_deque_, this->empty_stl_deque_);
}

TYPED_TEST(DequeTest, PopBack_3) {
  TypeParam value{};

  for (int i{1}; i < 6; i++) {
    if (i % 2 == 0) {
      this->empty_s21_deque_.push_front(value);
      this->empty_stl_deque_.push_front(value);
    } else {
      this->empty_s21_deque_.push_back(value);
      this->empty_stl_deque_.push_back(value);
    }
  }

  this->empty_stl_deque_.pop_back();
  this->empty_s21_deque_.pop_back();

  EXPECT_EQ(this->empty_s21_deque_, this->empty_stl_deque_);
}

// pop_front
TYPED_TEST(DequeTest, PopFront_1) {
  TypeParam value{};
  for (size_t i = 0; i < 5; ++i) {
    this->empty_s21_deque_.push_back(value);
    this->empty_stl_deque_.push_back(value);
  }
  this->empty_stl_deque_.pop_front();
  this->empty_s21_deque_.pop_front();

  EXPECT_EQ(this->empty_s21_deque_, this->empty_stl_deque_);
}

TYPED_TEST(DequeTest, PopFront_2) {
  TypeParam value{};

  for (size_t i = 0; i < 5; ++i) {
    this->empty_s21_deque_.push_front(value);
    this->empty_stl_deque_.push_front(value);
  }

  this->empty_s21_deque_.pop_front();
  this->empty_stl_deque_.pop_front();

  EXPECT_EQ(this->empty_s21_deque_, this->empty_stl_deque_);
  EXPECT_EQ(this->empty_s21_deque_.size(), this->empty_stl_deque_.size());
}

TYPED_TEST(DequeTest, PopFront_3) {
  TypeParam value{};

  for (size_t i = 0; i < 10000; ++i) {
    this->empty_s21_deque_.push_front(value);
    this->empty_stl_deque_.push_front(value);
  }

  for (size_t i = 0; i < 1000; ++i) {
    this->empty_s21_deque_.pop_front();
    this->empty_stl_deque_.pop_front();
  }

  EXPECT_EQ(this->empty_s21_deque_, this->empty_stl_deque_);
  EXPECT_EQ(this->empty_s21_deque_.size(), this->empty_stl_deque_.size());
}

TYPED_TEST(DequeTest, PopFront_4) {
  TypeParam value{};

  for (int i{1}; i < 6; i++) {
    if (i % 2 == 0) {
      this->empty_s21_deque_.push_front(value);
      this->empty_stl_deque_.push_front(value);
    } else {
      this->empty_s21_deque_.push_back(value);
      this->empty_stl_deque_.push_back(value);
    }
  }

  this->empty_stl_deque_.pop_front();
  this->empty_s21_deque_.pop_front();

  EXPECT_EQ(this->empty_s21_deque_, this->empty_stl_deque_);
  EXPECT_EQ(this->empty_s21_deque_.size(), this->empty_stl_deque_.size());
}

// back and front
TYPED_TEST(DequeTest, BackAndFront_1) {
  TypeParam value{};

  for (int i{1}; i < 6; i++) {
    if (i % 2 == 0) {
      this->empty_s21_deque_.push_front(value);
      this->empty_stl_deque_.push_front(value);
    } else {
      this->empty_s21_deque_.push_back(value);
      this->empty_stl_deque_.push_back(value);
    }
  }

  EXPECT_EQ(this->empty_s21_deque_.front(), this->empty_stl_deque_.front());
  EXPECT_EQ(this->empty_s21_deque_.back(), this->empty_stl_deque_.back());
}

TYPED_TEST(DequeTest, BackAndFront_2) {
  TypeParam value{};

  for (int i{1}; i < 6; i++) {
    this->empty_s21_deque_.push_back(value);
    this->empty_stl_deque_.push_back(value);
  }

  EXPECT_EQ(this->empty_s21_deque_.front(), this->empty_stl_deque_.front());
  EXPECT_EQ(this->empty_s21_deque_.back(), this->empty_stl_deque_.back());
}

// begin and end
TEST(DequeNonTyped, BeginAndEnd) {
  std::deque<int> std_q;
  s21::deque<int> s21_q;

  for (int i{1}; i < 1000; i++) {
    std_q.push_back(i);
    s21_q.push_back(i);
  }

  for (auto i{s21_q.begin()}; i != s21_q.end(); ++i) {
    *i = 4;
  }

  for (auto i{std_q.begin()}; i != std_q.end(); ++i) {
    *i = 4;
  }

  EXPECT_EQ(std_q, s21_q);
}

// empty
TYPED_TEST(DequeTest, Empty_1) {
  TypeParam value{};

  for (int i{1}; i < 6; i++) {
    this->empty_s21_deque_.push_back(value);
    this->empty_stl_deque_.push_back(value);
  }

  EXPECT_FALSE(this->empty_stl_deque_.empty());
  EXPECT_FALSE(this->empty_s21_deque_.empty());
}

TYPED_TEST(DequeTest, Empty_2) {
  EXPECT_TRUE(this->empty_stl_deque_.empty());
  EXPECT_TRUE(this->empty_s21_deque_.empty());
}

// swap
TEST(DequeNonTyped, Swap) {
  std::deque<int> std_q_1;
  s21::deque<int> s21_q_1;
  std::deque<int> std_q_2;
  s21::deque<int> s21_q_2;

  for (int i{1}; i < 6; i++) {
    std_q_1.push_back(i);
    s21_q_1.push_back(i);
  }

  for (int i{5}; i < 10; i++) {
    std_q_2.push_back(i);
    s21_q_2.push_back(i);
  }

  std_q_1.swap(std_q_2);
  s21_q_1.swap(s21_q_2);

  EXPECT_EQ(s21_q_1.at(0), std_q_1.at(0));
  EXPECT_EQ(s21_q_1.at(1), std_q_1.at(1));
  EXPECT_EQ(s21_q_1.at(2), std_q_1.at(2));
  EXPECT_EQ(s21_q_1.at(3), std_q_1.at(3));
  EXPECT_EQ(s21_q_1.at(4), std_q_1.at(4));

  EXPECT_EQ(s21_q_2.at(0), std_q_2.at(0));
  EXPECT_EQ(s21_q_2.at(1), std_q_2.at(1));
  EXPECT_EQ(s21_q_2.at(2), std_q_2.at(2));
  EXPECT_EQ(s21_q_2.at(3), std_q_2.at(3));
  EXPECT_EQ(s21_q_2.at(4), std_q_2.at(4));
}

TEST(DequeNonTyped, MinMaxSort) {
  std::deque<int> std_q;
  s21::deque<int> s21_q;

  for (int i{0}; i < 25; i++) {
    if (i % 2 == 0) {
      std_q.push_front(i);
      s21_q.push_front(i);
    } else {
      std_q.push_back(i);
      s21_q.push_back(i);
    }
  }

  EXPECT_EQ(*std::min_element(s21_q.begin(), s21_q.end()),
            *std::min_element(std_q.begin(), std_q.end()));

  std::sort(s21_q.begin(), s21_q.end());
  std::sort(std_q.begin(), std_q.end());
  EXPECT_EQ(s21_q, std_q);

  EXPECT_EQ(*std::max_element(s21_q.begin(), s21_q.end()),
            *std::max_element(std_q.begin(), std_q.end()));

  std::shuffle(s21_q.begin(), s21_q.end(), Random::mt);
  std::shuffle(std_q.begin(), std_q.end(), Random::mt);

  std::stable_sort(s21_q.begin(), s21_q.end());
  std::stable_sort(std_q.begin(), std_q.end());
  EXPECT_EQ(s21_q, std_q);

  EXPECT_EQ(std::binary_search(s21_q.begin(), s21_q.end(), s21_q[2]),
            std::binary_search(std_q.begin(), std_q.end(), s21_q[2]));
}

// copy constructor
TYPED_TEST(DequeTest, CopyConstructor) {
  s21::deque<TypeParam> copy(this->s21_deque_);

  EXPECT_EQ(this->s21_deque_.size(), copy.size());

  for (size_t i = 0; i < this->s21_deque_.size(); ++i) {
    EXPECT_EQ(this->s21_deque_[i], copy[i]);
  }

  copy.push_back(60);
  EXPECT_NE(this->s21_deque_.size(), copy.size());
  EXPECT_EQ(this->s21_deque_.back(), 86);
  EXPECT_EQ(copy.back(), 60);
}

// move constructor
TEST(DequeNonTyped, MoveConstructor) {
  s21::deque<int> original{10, 20, 30, 40, 50};
  size_t original_size = original.size();

  s21::deque<int> moved(std::move(original));

  EXPECT_EQ(moved.size(), original_size);
  for (size_t i = 0; i < moved.size(); ++i) {
    EXPECT_EQ(moved[i], static_cast<int>(10 + i * 10));
  }

  EXPECT_EQ(original.size(), size_t{0});
  EXPECT_THROW(original.at(0), std::out_of_range);
}

TEST(DequeTest, CopyAssignment) {
  s21::deque<int> d1{10, 20, 30, 40, 50};
  s21::deque<int> d2;
  d2.push_back(100);
  d2.push_back(200);

  d2 = d1;

  EXPECT_EQ(d2.size(), d1.size());

  for (size_t i = 0; i < d1.size(); ++i) {
    EXPECT_EQ(d2[i], d1[i]);
  }

  d2.push_back(60);
  EXPECT_NE(d2.size(), d1.size());
}

TEST(DequeTest, MoveAssignmentOperator) {
  s21::deque<int> original{10, 20, 30, 40, 50};
  s21::deque<int> moved;

  moved = std::move(original);

  EXPECT_EQ(moved.size(), size_t{5});
  EXPECT_EQ(moved[0], 10);
  EXPECT_EQ(moved[1], 20);
  EXPECT_EQ(moved[2], 30);
  EXPECT_EQ(moved[3], 40);
  EXPECT_EQ(moved[4], 50);

  EXPECT_EQ(original.size(), size_t{0});
  EXPECT_THROW(original.at(0), std::out_of_range);

  s21::deque<int> empty;
  moved = std::move(empty);
  EXPECT_EQ(moved.size(), size_t{0});
  EXPECT_THROW(moved.at(0), std::out_of_range);
}

#if 0
#endif
