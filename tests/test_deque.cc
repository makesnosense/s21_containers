#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#include <gtest/gtest.h>
#pragma GCC diagnostic pop

#include <deque>

#include "dummy_object.h"
#include "s21_deque.h"

template <typename T>
class DequeTest : public testing::Test {
 protected:
  DequeTest() : empty_stl_deque_(), empty_s21_deque_() {}

  std::deque<T> empty_stl_deque_{};
  s21::deque<T> empty_s21_deque_{};
  // std::deque<T> stl_vec_;
  // s21::deque<T> s21_vec_;
  // std::deque<T> large_stl_vec_;
  // s21::deque<T> large_s21_vec_;
};

using TestedTypes = ::testing::Types<int>;
TYPED_TEST_SUITE(DequeTest, TestedTypes, );

TYPED_TEST(DequeTest, First) {
  for (int i{0}; i < 70; i++) {
    (this->empty_s21_deque_).push_front(i);
  }
  std::cout << "hey \n";
  std::cout << this->empty_s21_deque_;
  // s21::deque<int> d;
  // EXPECT_EQ(this->empty_stl_vec_.size(), this->empty_s21_vec_.size());
  // EXPECT_TRUE(this->empty_s21_vec_.empty());
}

TYPED_TEST(DequeTest, Second) {
  for (int i{0}; i < 70; i++) {
    (this->empty_s21_deque_).push_back(i);
  }
  std::cout << "hey \n";
  std::cout << this->empty_s21_deque_;
  // s21::deque<int> d;
  // EXPECT_EQ(this->empty_stl_vec_.size(), this->empty_s21_vec_.size());
  // EXPECT_TRUE(this->empty_s21_vec_.empty());
}

// iterartor
TEST(DequeTest, ComparisonOperators) {
  int arr[] = {1, 2, 3, 4};
  s21::deque<int>::iterator iter1(arr);
  s21::deque<int>::iterator iter2(arr + 2);

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

TEST(DequeTest, ArithmeticOperators) {
  int arr[] = {1, 2, 3, 4};
  s21::deque<int>::iterator iter(arr + 1);

  EXPECT_EQ(*(iter - 1), 1);
  iter -= 1;
  EXPECT_EQ(*iter, 1);

  s21::deque<int>::iterator iter2 = 2 + iter;
  EXPECT_EQ(*iter2, 3);
}

TEST(DequeTest, PostDecrement) {
  int arr[] = {1, 2, 3, 4};
  s21::deque<int>::iterator iter(arr + 2);

  s21::deque<int>::iterator temp = iter--;
  EXPECT_EQ(*temp, 3);
  EXPECT_EQ(*iter, 2);
}

TEST(DequeTest, SubscriptOperator) {
  int arr[] = {10, 20, 30, 40, 50};
  s21::deque<int>::iterator iter(arr);

  EXPECT_EQ(iter[0], 10);
  EXPECT_EQ(iter[1], 20);
  EXPECT_EQ(iter[2], 30);

  iter[0] = 100;
  EXPECT_EQ(arr[0], 100);
}

// InitializerListConstructor
TEST(DequeTest, InitializerListConstructor) {
  int arr[] = {10, 20, 30, 40, 50};
  s21::deque<int> q{10, 20, 30, 40, 50};

  EXPECT_EQ(q[0], arr[0]);
  EXPECT_EQ(q[1], arr[1]);
  EXPECT_EQ(q[2], arr[2]);
}

// at
TEST(DequeTest, At) {
  std::deque<int> std_q{10, 20, 30, 40, 50};
  s21::deque<int> s21_q{10, 20, 30, 40, 50};

  EXPECT_EQ(s21_q.at(0), std_q.at(0));
  EXPECT_EQ(s21_q.at(1), std_q.at(1));
  EXPECT_EQ(s21_q.at(2), std_q.at(2));
}

// pop_back
TEST(DequeTest, PopBack_1) {
  std::deque<int> std_q{10, 20, 30, 40, 50};
  s21::deque<int> s21_q{10, 20, 30, 40, 50};

  std_q.pop_back();
  s21_q.pop_back();

  EXPECT_EQ(s21_q.at(0), std_q.at(0));
  EXPECT_EQ(s21_q.at(1), std_q.at(1));
  EXPECT_EQ(s21_q.at(2), std_q.at(2));
  EXPECT_EQ(s21_q.at(3), std_q.at(3));
  EXPECT_EQ(s21_q.size(), std_q.size());
}

TEST(DequeTest, PopBack_2) {
  std::deque<int> std_q;
  s21::deque<int> s21_q;

  for (int i{1}; i < 6; i++) {
    std_q.push_front(i);
    s21_q.push_front(i);
  }

  std_q.pop_back();
  s21_q.pop_back();

  EXPECT_EQ(s21_q.at(0), std_q.at(0));
  EXPECT_EQ(s21_q.at(1), std_q.at(1));
  EXPECT_EQ(s21_q.at(2), std_q.at(2));
  EXPECT_EQ(s21_q.at(3), std_q.at(3));
  EXPECT_EQ(s21_q.size(), std_q.size());
}

TEST(DequeTest, PopBack_3) {
  std::deque<int> std_q;
  s21::deque<int> s21_q;

  for (int i{1}; i < 6; i++) {
    if (i % 2 == 0) {
      std_q.push_front(i);
      s21_q.push_front(i);
    } else {
      std_q.push_back(i);
      s21_q.push_back(i);
    }
  }

  std_q.pop_back();
  s21_q.pop_back();

  EXPECT_EQ(s21_q.at(0), std_q.at(0));
  EXPECT_EQ(s21_q.at(1), std_q.at(1));
  EXPECT_EQ(s21_q.at(2), std_q.at(2));
  EXPECT_EQ(s21_q.at(3), std_q.at(3));
  EXPECT_EQ(s21_q.size(), std_q.size());
}

// pop_front
TEST(DequeTest, PopFront_1) {
  std::deque<int> std_q{10, 20, 30, 40, 50};
  s21::deque<int> s21_q{10, 20, 30, 40, 50};

  std_q.pop_front();
  s21_q.pop_front();

  EXPECT_EQ(s21_q.at(0), std_q.at(0));
  EXPECT_EQ(s21_q.at(1), std_q.at(1));
  EXPECT_EQ(s21_q.at(2), std_q.at(2));
  EXPECT_EQ(s21_q.at(3), std_q.at(3));
  EXPECT_EQ(s21_q.size(), std_q.size());
}

TEST(DequeTest, PopFront_2) {
  std::deque<int> std_q;
  s21::deque<int> s21_q;

  for (int i{1}; i < 6; i++) {
    std_q.push_front(i);
    s21_q.push_front(i);
  }

  std_q.pop_front();
  s21_q.pop_front();

  EXPECT_EQ(s21_q.at(0), std_q.at(0));
  EXPECT_EQ(s21_q.at(1), std_q.at(1));
  EXPECT_EQ(s21_q.at(2), std_q.at(2));
  EXPECT_EQ(s21_q.at(3), std_q.at(3));
  EXPECT_EQ(s21_q.size(), std_q.size());
}

TEST(DequeTest, PopFront_3) {
  std::deque<int> std_q;
  s21::deque<int> s21_q;

  for (int i{1}; i < 6; i++) {
    if (i % 2 == 0) {
      std_q.push_front(i);
      s21_q.push_front(i);
    } else {
      std_q.push_back(i);
      s21_q.push_back(i);
    }
  }

  std_q.pop_front();
  s21_q.pop_front();

  EXPECT_EQ(s21_q.at(0), std_q.at(0));
  EXPECT_EQ(s21_q.at(1), std_q.at(1));
  EXPECT_EQ(s21_q.at(2), std_q.at(2));
  EXPECT_EQ(s21_q.at(3), std_q.at(3));
  EXPECT_EQ(s21_q.size(), std_q.size());
}

// back an front
TEST(DequeTest, BackAndFront_1) {
  std::deque<int> std_q;
  s21::deque<int> s21_q;

  for (int i{1}; i < 6; i++) {
    if (i % 2 == 0) {
      std_q.push_front(i);
      s21_q.push_front(i);
    } else {
      std_q.push_back(i);
      s21_q.push_back(i);
    }
  }

  EXPECT_EQ(s21_q.front(), std_q.front());
  EXPECT_EQ(s21_q.back(), std_q.back());
}

TEST(DequeTest, BackAndFront_2) {
  std::deque<int> std_q;
  s21::deque<int> s21_q;

  for (int i{1}; i < 6; i++) {
    std_q.push_back(i);
    s21_q.push_back(i);
  }

  EXPECT_EQ(s21_q.front(), std_q.front());
  EXPECT_EQ(s21_q.back(), std_q.back());
}

// begin and end
TEST(DequeTest, BeginAndEnd) {
  std::deque<int> std_q;
  s21::deque<int> s21_q;

  for (int i{1}; i < 6; i++) {
    std_q.push_back(i);
    s21_q.push_back(i);
  }

  EXPECT_EQ(*(s21_q.begin()), *(std_q.begin()));
  EXPECT_EQ(*(s21_q.end()), *(std_q.end()));
}

// empty
TEST(DequeTest, Empty_1) {
  std::deque<int> std_q;
  s21::deque<int> s21_q;

  for (int i{1}; i < 6; i++) {
    std_q.push_back(i);
    s21_q.push_back(i);
  }

  EXPECT_FALSE(std_q.empty());
  EXPECT_FALSE(s21_q.empty());
}

TEST(DequeTest, Empty_2) {
  std::deque<int> std_q;
  s21::deque<int> s21_q;

  EXPECT_TRUE(std_q.empty());
  EXPECT_TRUE(s21_q.empty());
}
