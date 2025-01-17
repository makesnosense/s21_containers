#include <gtest/gtest.h>

#include <iostream>
#include <list>

#include "dummy_object.h"
#include "s21_list.h"

template <typename T>
class ListTest : public testing::Test {
 protected:
  ListTest()
      : empty_stl_list_(),
        empty_s21_list_(),
        stl_list_{68, 66, 67},
        s21_list_{68, 66, 67} {}

  std::list<T> empty_stl_list_;
  s21::list<T> empty_s21_list_;
  std::list<T> stl_list_;
  s21::list<T> s21_list_;
};
using TestedTypes = ::testing::Types<char, int, double, DummyObject>;
TYPED_TEST_SUITE(ListTest, TestedTypes);

TYPED_TEST(ListTest, DefaultConstructor) {
  EXPECT_EQ(this->empty_stl_list_.size(), this->empty_s21_list_.size());
  EXPECT_TRUE(this->empty_s21_list_.empty());
}

#if 0
#endif
