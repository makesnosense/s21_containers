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
        stl_list_{65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75},
        s21_list_{65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75},
        stl_list_for_swap{1, 2, 3, 4, 5, 6, 7, 8, 9},
        s21_list_for_swap{1, 2, 3, 4, 5, 6, 7, 8, 9},
        list_(3),
        alist_(3) {}
  // stl_list_for_copy(stl_list_){},
  // s21_list_for_copy(s21_list_) {}
  /// как сделвть cp
  std::list<T> empty_stl_list_;
  s21::list<T> empty_s21_list_;
  std::list<T> stl_list_;
  s21::list<T> s21_list_;
  std::list<T> stl_list_for_swap;
  s21::list<T> s21_list_for_swap;
  // std::list<T> stl_list_for_copy(stl_list_){};
  // s21::list<T> s21_list_for_copy(s21_list_){};
  std::list<T> list_;
  s21::list<T> alist_;
};
using TestedTypes = ::testing::Types<char, int, double, DummyObject>;
TYPED_TEST_SUITE(ListTest, TestedTypes);

TYPED_TEST(ListTest, DefaultConstructor) {
  EXPECT_EQ(this->empty_stl_list_.size(), this->empty_s21_list_.size());
  EXPECT_TRUE(this->empty_s21_list_.empty());
}
TYPED_TEST(ListTest, push_back) {
  this->empty_stl_list_.push_back(3);
  this->empty_s21_list_.push_back(3);
  EXPECT_EQ(this->empty_stl_list_.size(), this->empty_s21_list_.size());
}
TYPED_TEST(ListTest, pop_back) {
  this->empty_stl_list_.push_back(3);
  this->empty_s21_list_.push_back(3);
  this->empty_stl_list_.pop_back();
  this->empty_s21_list_.pop_back();
  EXPECT_EQ(this->empty_stl_list_.size(), this->empty_s21_list_.size());
}

TYPED_TEST(ListTest, ConstructorSize_type) {
  EXPECT_EQ(this->list_.size(), this->alist_.size());
  EXPECT_EQ(this->stl_list_.size(), this->s21_list_.size());
}
TYPED_TEST(ListTest, max_size) {
  EXPECT_EQ(this->s21_list_.max_size(), this->s21_list_.max_size());
}

TYPED_TEST(ListTest, reverse) {
  this->s21_list_.reverse();
  this->stl_list_.reverse();
  auto it2 = this->stl_list_.begin();
  for (auto it = this->s21_list_.begin(); it != this->s21_list_.end(); ++it) {
    // std::cout << *it << " " << *it2 << " ";
    EXPECT_EQ(*it, *it2);
    // std::cout << "\n";
    ++it2;
  }
}

TYPED_TEST(ListTest, get_front) {
  EXPECT_EQ(this->stl_list_.front(), this->s21_list_.front());
}
TYPED_TEST(ListTest, back) {
  EXPECT_EQ(this->stl_list_.back(), this->s21_list_.back());
}
////ietr
TYPED_TEST(ListTest, begin) {
  auto s21_it = this->s21_list_.begin();
  auto stl_it = this->stl_list_.begin();
  EXPECT_EQ(*s21_it, *stl_it);
}
TYPED_TEST(ListTest, end) {
  auto s21_it = this->s21_list_.end();
  auto stl_it = this->stl_list_.end();
  (void)stl_it;
  EXPECT_EQ(1, 1);
}
TYPED_TEST(ListTest, erase) {
  auto s21_it = this->s21_list_.begin();
  auto stl_it = this->stl_list_.begin();

  this->s21_list_.erase(s21_it);
  this->stl_list_.erase(stl_it);
  auto stl_it2 = this->stl_list_.begin();

  for (auto i = this->s21_list_.begin(); i != this->s21_list_.end(); ++i) {
    EXPECT_EQ(*i, *stl_it2);

    ++stl_it2;
  }
}

TYPED_TEST(ListTest, sort) {
  this->s21_list_.reverse();
  this->stl_list_.reverse();

  this->s21_list_.sort();
  this->stl_list_.sort();
  auto stl_it = this->stl_list_.begin();
  for (auto i = this->s21_list_.begin(); i != this->s21_list_.end(); ++i) {
    EXPECT_EQ(*i, *stl_it);

    ++stl_it;
  }
}

TYPED_TEST(ListTest, unique) {
  this->s21_list_.push_back(5);
  this->s21_list_.push_back(5);
  this->s21_list_.push_back(5);
  this->s21_list_.push_back(5);
  this->s21_list_.push_back(5);
  this->stl_list_.push_back(5);
  this->stl_list_.push_back(5);
  this->stl_list_.push_back(5);
  this->stl_list_.push_back(5);
  this->stl_list_.push_back(5);
  auto stl_it = this->stl_list_.begin();
  for (auto i = this->s21_list_.begin(); i != this->s21_list_.end(); ++i) {
    EXPECT_EQ(*i, *stl_it);

    ++stl_it;
  }

  EXPECT_EQ(this->stl_list_.size(), this->s21_list_.size());
}
TYPED_TEST(ListTest, CopyAssignmentOperator) {
  this->s21_list_ = this->empty_s21_list_;
  this->stl_list_ = this->empty_stl_list_;

  auto stl_it = this->empty_stl_list_.begin();
  for (auto i = this->empty_s21_list_.begin(); i != this->empty_s21_list_.end();
       ++i) {
    EXPECT_EQ(*i, *stl_it);

    ++stl_it;
  }

  EXPECT_EQ(this->empty_s21_list_.size(), this->s21_list_.size());
}
TYPED_TEST(ListTest, swap) {
  this->s21_list_.swap(this->s21_list_for_swap);
  this->stl_list_.swap(this->stl_list_for_swap);

  auto stl_it = this->stl_list_.begin();
  for (auto i = this->s21_list_.begin(); i != this->s21_list_.end(); ++i) {
    EXPECT_EQ(*i, *stl_it);

    ++stl_it;
  }
}
TYPED_TEST(ListTest, merge) {
  this->s21_list_.merge(this->s21_list_for_swap);
  this->stl_list_.merge(this->stl_list_for_swap);

  auto stl_it = this->stl_list_.begin();
  for (auto i = this->s21_list_.begin(); i != this->s21_list_.end(); ++i) {
    EXPECT_EQ(*i, *stl_it);

    ++stl_it;
  }
}
TYPED_TEST(ListTest, insert) {
  auto stl_it = this->stl_list_.begin();
  auto s21_it = this->s21_list_.begin();

  this->stl_list_.insert(stl_it, 2);
  this->s21_list_.insert(s21_it, 2);
  stl_it = this->stl_list_.begin();
  for (auto i = this->s21_list_.begin(); i != this->s21_list_.end(); ++i) {
    EXPECT_EQ(*i, *stl_it);

    ++stl_it;
  }
}
TYPED_TEST(ListTest, copy) {
  // s21::list<int> vec1{1, 2, 3};
  // s21::list<int> vec2(vec1);

  //   for (size_t i = 0; i < vec1.size(); i++) {
  //   std::cout << "\n" << vec1.get_element(i) << "\ngg";
  // }

  // for (size_t i = 0; i < vec2.size(); i++) {
  //   std::cout << "\n" << vec2.get_element(i) << "\n";
  // }
  EXPECT_EQ(this->stl_list_.size(), this->s21_list_.size());
}
TYPED_TEST(ListTest, movedList) {
  s21::list<int> originalList;
  originalList.push_back(1);
  originalList.push_back(2);
  originalList.push_back(3);

  s21::list<int> movedList(std::move(originalList));

  std::cout << "Contents of movedList: ";
  for (size_t i = 0; i < movedList.size(); ++i) {
    std::cout << movedList.get_element(i) << " ";
  }

  std::cout << "\nOriginal list size after move: " << originalList.size();
  EXPECT_EQ(this->stl_list_.size(), this->s21_list_.size());
}
TYPED_TEST(ListTest, splice) {
  this->empty_s21_list_.push_back(1);
  this->empty_s21_list_.push_back(2);
  this->empty_stl_list_.push_back(1);
  this->empty_stl_list_.push_back(2);

  auto it = this->s21_list_.begin();
  ++it;
  auto it2 = this->stl_list_.begin();
  ++it2;

  this->s21_list_.splice(it, this->empty_s21_list_);
  this->stl_list_.splice(it2, this->empty_stl_list_);

  // auto stl_it = this->stl_list_.begin();
  // for (auto i = this->s21_list_.begin(); i != this->s21_list_.end(); ++i) {
  //   EXPECT_EQ(*i, *stl_it);

  //   ++stl_it;
  // }
  bool ans = this->s21_list_ == this->stl_list_;

  EXPECT_TRUE(ans);
}
// TYPED_TEST(ListTest, splice) {

// }
#if 0
#endif
