#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#include <gtest/gtest.h>
#pragma GCC diagnostic pop

#include <iostream>
#include <list>
#include <queue>

#include "dummy_object.h"
#include "s21_queue.h"

template <typename T>
class QueueTest : public testing::Test {
 protected:
  QueueTest() : empty_stl_queue_(), empty_s21_queue_() {};

  std::queue<T> empty_stl_queue_;
  s21::queue<T> empty_s21_queue_;
  // std::queue<T> stl_queue_;
  // s21::queue<T> s21_queue_;
};
using TestedTypes = ::testing::Types<char, int, double, DummyObject>;
TYPED_TEST_SUITE(QueueTest, TestedTypes, );

TYPED_TEST(QueueTest, qeue1) { EXPECT_EQ(1, 1); }

TEST(QueueTest, qeue) {
  s21::queue<int> mss;
  s21::queue<int> mss_swap;
  for (int i = 0; i < 10; i++) {
    mss.push(i);
  }
  mss_swap.push(5);
  mss_swap.push(5);

  std::cout << mss.size() << "\n";
  mss.pop();
  std::cout << mss.size() << "\n";
  std::cout << mss.front() << "\n";
  std::cout << mss.back() << "\n";
  mss_swap = mss;
  mss_swap.swap(mss);
  EXPECT_TRUE(true);
  EXPECT_EQ(1, 1);
}
#if 0
#endif
