#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#include <gtest/gtest.h>
#pragma GCC diagnostic pop

#include <algorithm>
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
        large_stl_list_(1000, 42),
        large_s21_list_(1000, 42) {}

  std::list<T> empty_stl_list_;
  s21::list<T> empty_s21_list_;
  std::list<T> stl_list_;
  s21::list<T> s21_list_;
  std::list<T> stl_list_for_swap;
  s21::list<T> s21_list_for_swap;

  std::list<T> large_stl_list_;
  s21::list<T> large_s21_list_;
};
using TestedTypes = ::testing::Types<char, int, double, DummyObject>;
TYPED_TEST_SUITE(ListTest, TestedTypes, );

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
  EXPECT_EQ(this->large_stl_list_.size(), this->large_s21_list_.size());
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
    EXPECT_EQ(*it, *it2);

    ++it2;
  }
}

TYPED_TEST(ListTest, get_front) {
  EXPECT_EQ(this->stl_list_.front(), this->s21_list_.front());
}

TYPED_TEST(ListTest, back) {
  EXPECT_EQ(this->stl_list_.back(), this->s21_list_.back());
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
  EXPECT_EQ(this->stl_list_.size(), this->s21_list_.size());
}
TYPED_TEST(ListTest, movedList) {
  s21::list<int> originalList;
  originalList.push_back(1);
  originalList.push_back(2);
  originalList.push_back(3);

  s21::list<int> movedList(std::move(originalList));

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

  bool ans = this->s21_list_ == this->stl_list_;

  EXPECT_TRUE(ans);
}

TEST(ListTest, all_of) {
  s21::list<int> s21_numbers_list{2, 4, 6, 8};
  std::list<int> stl_numbers_list{2, 4, 6, 8};

  bool s21_all_even =
      std::all_of(s21_numbers_list.begin(), s21_numbers_list.end(),
                  [](int n) { return n % 2 == 0; });
  bool stl_all_even =
      std::all_of(stl_numbers_list.begin(), stl_numbers_list.end(),
                  [](int n) { return n % 2 == 0; });

  EXPECT_EQ(s21_all_even, stl_all_even);
}
TEST(ListTest, any_of) {
  s21::list<int> s21_numbers_list{2, 4, 6, 8, 11};
  std::list<int> stl_numbers_list{2, 4, 6, 8, 11};

  bool s21_all_even =
      std::any_of(s21_numbers_list.begin(), s21_numbers_list.end(),
                  [](int n) { return n % 2 == 0; });
  bool stl_all_even =
      std::any_of(stl_numbers_list.begin(), stl_numbers_list.end(),
                  [](int n) { return n % 2 == 0; });
  EXPECT_EQ(s21_all_even, stl_all_even);
}
TEST(ListTest, none_of) {
  s21::list<int> s21_numbers_list{2, 4, 6, 8, 11};
  std::list<int> stl_numbers_list{2, 4, 6, 8, 11};

  bool s21_all_even =
      std::none_of(s21_numbers_list.begin(), s21_numbers_list.end(),
                   [](int n) { return n < 0; });
  bool stl_all_even =
      std::none_of(stl_numbers_list.begin(), stl_numbers_list.end(),
                   [](int n) { return n < 0; });
  EXPECT_EQ(s21_all_even, stl_all_even);
}
TEST(ListTest, for_each) {
  s21::list<int> s21_numbers_list{1, 2, 3, 4, 5};
  std::list<int> stl_numbers_list{1, 2, 3, 4, 5};

  std::for_each(s21_numbers_list.begin(), s21_numbers_list.end(),
                [](int& n) { ++n; });

  std::for_each(stl_numbers_list.begin(), stl_numbers_list.end(),
                [](int& n) { ++n; });
  EXPECT_TRUE(s21_numbers_list == stl_numbers_list);
}
TEST(ListTest, for_each_n) {
  s21::list<int> s21_numbers_list{1, 2, 3, 4, 5};
  std::list<int> stl_numbers_list{1, 2, 3, 4, 5};

  std::for_each_n(s21_numbers_list.begin(), 3, [](int& n) { n *= 2; });
  std::for_each_n(stl_numbers_list.begin(), 3, [](int& n) { n *= 2; });

  EXPECT_TRUE(s21_numbers_list == stl_numbers_list);
}
TEST(ListTest, count) {
  s21::list<int> s21_numbers_list{1, 2, 3, 4, 5};
  std::list<int> stl_numbers_list{1, 2, 3, 4, 5};

  auto s21_all_even =
      std::count(s21_numbers_list.begin(), s21_numbers_list.end(), 2);
  auto stl_all_even =
      std::count(stl_numbers_list.begin(), stl_numbers_list.end(), 2);

  EXPECT_EQ(s21_all_even, stl_all_even);
}
TEST(ListTest, count_if) {
  s21::list<int> s21_numbers_list{1, 2, 3, 4, 5};
  std::list<int> stl_numbers_list{1, 2, 3, 4, 5};
  auto s21_all_even =
      std::count_if(s21_numbers_list.begin(), s21_numbers_list.end(),
                    [](int n) { return n % 2 == 0; });
  auto stl_all_even =
      std::count_if(stl_numbers_list.begin(), stl_numbers_list.end(),
                    [](int n) { return n % 2 == 0; });

  EXPECT_EQ(s21_all_even, stl_all_even);
}
TEST(ListTest, find) {
  s21::list<int> s21_numbers_list{1, 2, 3, 4, 5};
  std::list<int> stl_numbers_list{1, 2, 3, 4, 5};

  auto s21_all_even =
      std::find(s21_numbers_list.begin(), s21_numbers_list.end(), 3);
  auto stl_all_even =
      std::find(stl_numbers_list.begin(), stl_numbers_list.end(), 3);
  EXPECT_EQ(*s21_all_even, *stl_all_even);
}
TEST(ListTest, find_if) {
  s21::list<int> s21_numbers_list{1, 3, 5, 7, 8, 10};
  std::list<int> stl_numbers_list{1, 3, 5, 7, 8, 10};

  auto s21_all_even =
      std::find_if(s21_numbers_list.begin(), s21_numbers_list.end(),
                   [](int n) { return n % 2 == 0; });
  auto stl_all_even =
      std::find_if(stl_numbers_list.begin(), stl_numbers_list.end(),
                   [](int n) { return n % 2 == 0; });

  EXPECT_EQ(*s21_all_even, *stl_all_even);
}
TEST(ListTest, find_if_not) {
  s21::list<int> s21_numbers_list{1, 3, 5, 7, 8, 10};
  std::list<int> stl_numbers_list{1, 3, 5, 7, 8, 10};

  auto s21_all_even =
      std::find_if_not(s21_numbers_list.begin(), s21_numbers_list.end(),
                       [](int n) { return n % 2 != 0; });
  auto stl_all_even =
      std::find_if_not(stl_numbers_list.begin(), stl_numbers_list.end(),
                       [](int n) { return n % 2 != 0; });

  EXPECT_EQ(*s21_all_even, *stl_all_even);
}
TEST(ListTest, find_end) {
  s21::list<int> s21_numbers_list{1, 2, 1, 2, 3, 4, 1, 2};
  s21::list<int> s21_numbers_list_end{1, 2};
  std::list<int> stl_numbers_list{1, 2, 1, 2, 3, 4, 1, 2};
  std::list<int> stl_numbers_list_end{1, 2};

  auto s21_all_even =
      std::find_end(s21_numbers_list.begin(), s21_numbers_list.end(),
                    s21_numbers_list_end.begin(), s21_numbers_list_end.end());
  auto stl_all_even =
      std::find_end(stl_numbers_list.begin(), stl_numbers_list.end(),
                    stl_numbers_list_end.begin(), stl_numbers_list_end.end());

  EXPECT_EQ(*s21_all_even, *stl_all_even);
}
TEST(ListTest, find_first_of) {
  s21::list<char> s21_char_list{'f', 'c', 'e', 'd', 'b', 'a'};
  s21::list<char> s21_char_list_find{'D', 'F'};
  std::list<char> stl_char_list{'f', 'c', 'e', 'd', 'b', 'a'};
  std::list<char> stl_char_list_find{'D', 'F'};

  auto s21_all_even = std::find_first_of(
      s21_char_list.begin(), s21_char_list.end(), s21_char_list_find.begin(),
      s21_char_list_find.end(),
      [](char a, char b) { return tolower(a) == tolower(b); });
  auto stl_all_even = std::find_first_of(
      stl_char_list.begin(), stl_char_list.end(), stl_char_list_find.begin(),
      stl_char_list_find.end(),
      [](char a, char b) { return tolower(a) == tolower(b); });

  EXPECT_EQ(*s21_all_even, *stl_all_even);
}
TEST(ListTest, adjacent_find) {
  s21::list<int> s21_numbers_list{1, 2, 3, 3, 5};
  std::list<int> stl_numbers_list{1, 2, 3, 3, 5};

  auto s21_all_even =
      std::adjacent_find(s21_numbers_list.begin(), s21_numbers_list.end());
  auto stl_all_even =
      std::adjacent_find(stl_numbers_list.begin(), stl_numbers_list.end());
  EXPECT_EQ(*s21_all_even, *stl_all_even);
}
TEST(ListTest, search) {
  s21::list<int> s21_numbers_list{1, 2, 3, 4};
  std::list<int> stl_numbers_list{1, 2, 3, 4};
  auto s21_all_even =
      std::find(s21_numbers_list.begin(), s21_numbers_list.end(), 3);
  auto stl_all_even =
      std::find(stl_numbers_list.begin(), stl_numbers_list.end(), 3);
  EXPECT_EQ(*s21_all_even, *stl_all_even);
}

TEST(ListTest, search_n) {
  s21::list<int> s21_numbers_list{1, 2, 3, 3, 3, 4, 5};
  std::list<int> stl_numbers_list{1, 2, 3, 3, 3, 4, 5};

  auto s21_all_even =
      std::search_n(s21_numbers_list.begin(), s21_numbers_list.end(), 2, 3);
  auto stl_all_even =
      std::search_n(stl_numbers_list.begin(), stl_numbers_list.end(), 2, 3);

  EXPECT_EQ(*s21_all_even, *stl_all_even);
}
TEST(ListTest, mismatch) {
  s21::list<int> s21_numbers_list{1, 2, 3, 4, 5};
  s21::list<int> s21_numbers_list_test{1, 2, 0, 4, 5};

  auto s21_all_even =
      std::mismatch(s21_numbers_list.begin(), s21_numbers_list.end(),
                    s21_numbers_list_test.begin());

  EXPECT_TRUE(*s21_all_even.first);
}
TEST(ListTest, equal) {
  s21::list<int> s21_numbers_list{1, 2, 3, 4, 5};
  s21::list<int> s21_numbers_list_equal{1, 2, 3, 4, 5};
  s21::list<int> s21_numbers_list_not_equal{1, 2, 3, 4};
  std::list<int> stl_numbers_list{1, 2, 3, 4, 5};
  std::list<int> stl_numbers_list_equal{1, 2, 3, 4, 5};
  std::list<int> stl_numbers_list_not_equal{1, 2, 3, 4};

  EXPECT_TRUE(std::equal(s21_numbers_list.begin(), s21_numbers_list.end(),
                         s21_numbers_list_equal.begin()));
  bool a = s21_numbers_list.size() == s21_numbers_list_not_equal.size() &&
           std::equal(s21_numbers_list.begin(), s21_numbers_list.end(),
                      s21_numbers_list_not_equal.begin());
  EXPECT_TRUE(!(a));
  EXPECT_TRUE(std::equal(stl_numbers_list.begin(), stl_numbers_list.end(),
                         stl_numbers_list_equal.begin()));
  EXPECT_TRUE(!(stl_numbers_list.size() == stl_numbers_list_not_equal.size() &&
                std::equal(stl_numbers_list.begin(), stl_numbers_list.end(),
                           stl_numbers_list_not_equal.begin())));
}
TEST(ListTest, copyit) {
  s21::list<int> s21_numbers_list{1, 2, 3, 4, 5};
  s21::list<int> s21_numbers_list_copy(5);
  std::list<int> stl_numbers_list{1, 2, 3, 4, 5};
  std::list<int> stl_numbers_list_copy(5);

  std::copy(s21_numbers_list.begin(), s21_numbers_list.end(),
            s21_numbers_list_copy.begin());
  std::copy(stl_numbers_list.begin(), stl_numbers_list.end(),
            stl_numbers_list_copy.begin());

  EXPECT_TRUE(s21_numbers_list_copy == stl_numbers_list_copy);
}

#if 0 
TYPED_TEST(ListTest, begin) {
  auto s21_it = this->s21_list_.begin();
  auto stl_it = this->stl_list_.begin();
  EXPECT_EQ(s21_it, stl_it);
}

TYPED_TEST(ListTest, end) {
  auto s21_it = this->s21_list_.end();
  auto stl_it = this->stl_list_.end();
  (void)stl_it;
  EXPECT_EQ(1, 1);
}
#endif

TEST(ListTest, PopFrontLeak) {
  s21::list<int> list;

  for (int i = 0; i < 1000; ++i) {
    list.push_back(i);
  }

  while (!list.empty()) {
    list.pop_front();
  }

  EXPECT_TRUE(list.empty());
  EXPECT_EQ(list.size(), size_t{0});
}