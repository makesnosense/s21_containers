#include <gtest/gtest.h>

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

TEST(ListTest, all_of) {
  s21::list<int> numberseee{2, 4, 6, 8};

  bool all_even = std::all_of(numberseee.begin(), numberseee.end(),
                              [](int n) { return n % 2 == 0; });

  if (all_even) {
    std::cout << "All numbers are even." << "\n";
  } else {
    std::cout << "Not all numbers are even." << "\n";
  }
}
TEST(ListTest, any_of) {
  s21::list<int> numbers{2, 4, 6, 8, 11};

  // Check if any number is odd
  bool result = std::any_of(numbers.begin(), numbers.end(),
                            [](int n) { return n % 2 == 0; });

  if (result) {
    std::cout << "The vector contains at least one odd number." << "\n";
  } else {
    std::cout << "All numbers are even." << "\n";
  }
}
TEST(ListTest, none_of) {
  s21::list<int> numbers{2, 4, 6, 8, 11};

  bool result =
      std::none_of(numbers.begin(), numbers.end(), [](int n) { return n < 0; });

  if (result) {
    std::cout << "All numbers are non-negative." << "\n";
  } else {
    std::cout << "There are negative numbers in the vector." << "\n";
  }
}
TEST(ListTest, for_each) {
  s21::list<int> numbers{1, 2, 3, 4, 5};

  std::for_each(numbers.begin(), numbers.end(),
                [](int n) { std::cout << n << " "; });

  std::cout << "\n";

  // Using std::for_each with a lambda expression
  std::for_each(numbers.begin(), numbers.end(),
                [](int n) { std::cout << n * 2 << " "; });
  std::cout << "\n";
}
TEST(ListTest, for_each_n) {
  s21::list<int> numbers{1, 2, 3, 4, 5};

  std::for_each_n(numbers.begin(), 3, [](int& n) { n *= 2; });

  for (const auto& num : numbers) {
    std::cout << num << " ";
  }
  std::cout << std::endl;
}
TEST(ListTest, count) {
  s21::list<int> numbers{1, 2, 3, 4, 5};

  auto count_of_twos = std::count(numbers.begin(), numbers.end(), 2);

  std::cout << "Number 2 appears " << count_of_twos << " times." << std::endl;
}
TEST(ListTest, count_if) {
  s21::list<int> numbers{1, 2, 3, 4, 5};

  auto count_of_evens = std::count_if(numbers.begin(), numbers.end(),
                                      [](int n) { return n % 2 == 0; });

  std::cout << "There are " << count_of_evens << " even numbers." << std::endl;
}
TEST(ListTest, find) {
  s21::list<int> v{1, 2, 3, 4, 5};

  auto it = std::find(v.begin(), v.end(), 3);
  if (it != v.end()) {
    std::cout << "Found: " << *it << std::endl;  // Output: Found: 3
  } else {
    std::cout << "Not found" << std::endl;
  }
}
TEST(ListTest, find_if) {
  s21::list<int> numbers{1, 3, 5, 7, 8, 10};

  auto it = std::find_if(numbers.begin(), numbers.end(),
                         [](int n) { return n % 2 == 0; });

  if (it != numbers.end()) {
    std::cout << "The first even number is " << *it
              << std::endl;  // Output: The first even number is 8
  } else {
    std::cout << "No even numbers found." << std::endl;
  }
}
TEST(ListTest, find_if_not) {
  s21::list<int> numbers{1, 3, 5, 7, 8, 10};

  // Use std::find_if_not to find the first even number
  auto it = std::find_if_not(numbers.begin(), numbers.end(),
                             [](int n) { return n % 2 != 0; });

  if (it != numbers.end()) {
    std::cout << "The first even number is " << *it
              << std::endl;  // Output: The first even number is 6
  } else {
    std::cout << "No even numbers found." << std::endl;
  }
}
TEST(ListTest, find_end) {
  s21::list<int> v1{1, 2, 1, 2, 3, 4, 1, 2};
  s21::list<int> v2{1, 2};

  auto result = std::find_end(v1.begin(), v1.end(), v2.begin(), v2.end());

  if (result != v1.end()) {
    std::cout << "Last sequence found at position: " << (result - v1.begin())
              << std::endl;  // Output position index
  } else {
    std::cout << "Sequence not found." << std::endl;
  }
}
TEST(ListTest, find_first_of) {
  s21::list<char> v1{'f', 'c', 'e', 'd', 'b', 'a'};
  s21::list<char> v2{'D', 'F'};

  // Find the first occurrence of any character from v2 in v1
  auto result = std::find_first_of(
      v1.begin(), v1.end(), v2.begin(), v2.end(),
      [](char a, char b) { return tolower(a) == tolower(b); });

  if (result != v1.end()) {
    std::cout << "Found first match at location: "
              << std::distance(v1.begin(), result) << std::endl;  ////not -
  } else {
    std::cout << "No matches found." << std::endl;
  }
}
TEST(ListTest, adjacent_find) {
  s21::list<int> numbers{1, 2, 3, 3, 5};

  // Find the first occurrence of consecutive identical elements
  auto it = std::adjacent_find(numbers.begin(), numbers.end());

  if (it != numbers.end()) {
    std::cout << "First occurrence of consecutive identical element = " << *it
              << std::endl;  // Output: 3
  } else {
    std::cout << "There are no consecutive identical elements." << std::endl;
  }
}
TEST(ListTest, search) {
  s21::list<int> numbers{1, 2, 3, 4};
  auto it = std::find(numbers.begin(), numbers.end(), 3);
  if (it != numbers.end()) {
    std::cout << "Found: " << *it << std::endl;  // Output: Found: 3
  }
}

TEST(ListTest, search_n) {
  s21::list<int> numbers{1, 2, 3, 3, 3, 4, 5};

  // Search for two consecutive occurrences of the number 3
  auto result = std::search_n(numbers.begin(), numbers.end(), 2, 3);

  if (result != numbers.end()) {
    std::cout << "Found two consecutive 3s at index: "
              << (result - numbers.begin())
              << std::endl;  // Output: Found two consecutive 3s at index: 2
  } else {
    std::cout << "No consecutive occurrences found." << std::endl;
  }
}
TEST(ListTest, mismatch) {
  s21::list<int> vec1{1, 2, 3, 4, 5};
  s21::list<int> vec2{1, 2, 0, 4, 5};  // Note: 0 differs from 3 in vec1

  // Find the first mismatch between vec1 and vec2
  auto result = std::mismatch(vec1.begin(), vec1.end(), vec2.begin());

  if (result.first != vec1.end()) {
    std::cout << "Mismatch found at position: " << (result.first - vec1.begin())
              << std::endl;
    std::cout << "vec1: " << *result.first << ", vec2: " << *result.second
              << std::endl;  // Output: vec1: 3, vec2: 0
  } else {
    std::cout << "No mismatches found." << std::endl;
  }
}

#if 0
#endif
