#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#include <gtest/gtest.h>
#pragma GCC diagnostic pop

#include <algorithm>
#include <cstring>
#include <vector>

#include "dummy_object.h"
#include "random.h"
#include "s21_map.h"

template <typename T>
class MapTest : public testing::Test {
 protected:
  using key_type = typename T::first_type;
  using mapped_type = typename T::second_type;

  MapTest()
      : empty_stl_map_(),
        empty_s21_map_(),
        stl_map_{{key_type(103), mapped_type("a")},
                 {key_type(53), mapped_type("b")},
                 {key_type(73), mapped_type("c")},
                 {key_type(106), mapped_type("d")},
                 {key_type(57), mapped_type("e")},
                 {key_type(102), mapped_type("f")},
                 {key_type(46), mapped_type("g")},
                 {key_type(104), mapped_type("h")}},
        s21_map_{{key_type(103), mapped_type("a")},
                 {key_type(53), mapped_type("b")},
                 {key_type(73), mapped_type("c")},
                 {key_type(106), mapped_type("d")},
                 {key_type(57), mapped_type("e")},
                 {key_type(102), mapped_type("f")},
                 {key_type(46), mapped_type("g")},
                 {key_type(104), mapped_type("h")}} {}

  std::map<key_type, mapped_type> empty_stl_map_;
  s21::map<key_type, mapped_type> empty_s21_map_;
  std::map<key_type, mapped_type> stl_map_;
  s21::map<key_type, mapped_type> s21_map_;
};

using TestedTypes =
    ::testing::Types<std::pair<char, std::string>, std::pair<int, std::string>,
                     std::pair<double, std::string>,
                     std::pair<DummyObject, std::string>>;

TYPED_TEST_SUITE(MapTest, TestedTypes, );

TYPED_TEST(MapTest, InsertMap_1) {
  for (int i{0}; i < 200; i++) {
    typename TypeParam::first_type key{7};
    typename TypeParam::second_type value{"test"};
    this->empty_stl_map_.insert({key, value});
    this->empty_s21_map_.insert({key, value});
  }

  typename s21::map<typename TypeParam::first_type,
                    typename TypeParam::second_type>::iterator s21_map_it{
      this->empty_s21_map_.begin()};
  typename std::map<typename TypeParam::first_type,
                    typename TypeParam::second_type>::iterator std_map_it{
      this->empty_stl_map_.begin()};

  for (size_t i{0}; i < this->empty_s21_map_.size(); i++) {
    EXPECT_EQ(s21_map_it->first, std_map_it->first);
    EXPECT_EQ(s21_map_it->second, std_map_it->second);
    ++std_map_it;
    ++s21_map_it;
  }

  EXPECT_TRUE(this->empty_s21_map_.size() == this->empty_stl_map_.size());
}

TYPED_TEST(MapTest, InsertDuplicate) {
  typename TypeParam::first_type key{42};
  typename TypeParam::second_type value{"test"};

  this->empty_stl_map_.insert({key, value});
  this->empty_s21_map_.insert({key, value});

  auto result_std = this->empty_stl_map_.insert({key, value});
  auto result_s21 = this->empty_s21_map_.insert({key, value});

  EXPECT_FALSE(result_s21.second);
  EXPECT_FALSE(result_std.second);

  EXPECT_EQ(result_s21.first->first, result_std.first->first);
  EXPECT_EQ(result_s21.first->second, result_std.first->second);
  EXPECT_EQ(this->empty_s21_map_.size(), this->empty_stl_map_.size());
}

TYPED_TEST(MapTest, InsertFromEmptyMap) {
  typename TypeParam::first_type key{100};
  typename TypeParam::second_type value{"test"};

  auto result_s21 = this->empty_s21_map_.insert({key, value});
  auto result_std = this->empty_stl_map_.insert({key, value});

  EXPECT_TRUE(result_s21.second);
  EXPECT_TRUE(result_std.second);

  EXPECT_EQ(result_s21.first->first, result_std.first->first);
  EXPECT_EQ(result_s21.first->second, result_std.first->second);
  EXPECT_EQ(this->empty_s21_map_.size(), this->empty_stl_map_.size());
}

TYPED_TEST(MapTest, InsertWithExistingElements) {
  this->empty_s21_map_.insert({10, "a"});
  this->empty_s21_map_.insert({20, "b"});
  this->empty_s21_map_.insert({30, "c"});

  this->empty_stl_map_.insert({10, "a"});
  this->empty_stl_map_.insert({20, "b"});
  this->empty_stl_map_.insert({30, "c"});

  auto result_s21 = this->empty_s21_map_.insert({20, "new"});
  auto result_std = this->empty_stl_map_.insert({20, "new"});

  EXPECT_FALSE(result_s21.second);
  EXPECT_FALSE(result_std.second);

  EXPECT_EQ(result_s21.first->first, result_std.first->first);
  EXPECT_EQ(result_s21.first->second, result_std.first->second);
  EXPECT_EQ(this->empty_s21_map_.size(), this->empty_stl_map_.size());
}

// Element access tests
TYPED_TEST(MapTest, At) {
  EXPECT_EQ(this->s21_map_.at(103), "a");
  EXPECT_EQ(this->s21_map_.at(53), "b");
  EXPECT_EQ(this->s21_map_.at(73), "c");

  EXPECT_THROW(this->s21_map_.at(999), std::out_of_range);
  EXPECT_THROW(this->empty_s21_map_.at(1), std::out_of_range);
}

TYPED_TEST(MapTest, OperatorSubscript) {
  // Access existing element
  EXPECT_EQ(this->s21_map_[103], "a");

  // Insert new element
  typename TypeParam::first_type key{999};
  this->s21_map_[key] = "new";
  EXPECT_EQ(this->s21_map_[key], "new");
  EXPECT_EQ(this->s21_map_.size(), size_t{9});
}

// Copy constructor
TYPED_TEST(MapTest, CopyConstructor) {
  s21::map<typename TypeParam::first_type, typename TypeParam::second_type>
      s21_map_2(this->s21_map_);

  EXPECT_EQ(this->s21_map_, s21_map_2);
}

// Move constructor
TYPED_TEST(MapTest, MoveConstructor) {
  s21::map<typename TypeParam::first_type, typename TypeParam::second_type>
      s21_map_2(std::move(this->s21_map_));

  EXPECT_EQ(s21_map_2.size(), size_t{8});
  EXPECT_EQ(s21_map_2.at(103), "a");
  EXPECT_EQ(s21_map_2.at(53), "b");
  EXPECT_EQ(s21_map_2.at(73), "c");
  EXPECT_EQ(s21_map_2.at(106), "d");
  EXPECT_EQ(s21_map_2.at(57), "e");
  EXPECT_EQ(s21_map_2.at(102), "f");
  EXPECT_EQ(s21_map_2.at(46), "g");
  EXPECT_EQ(s21_map_2.at(104), "h");

  EXPECT_EQ(this->s21_map_.size(), size_t{0});
  EXPECT_THROW(this->s21_map_.at(103), std::out_of_range);
}

// Move Assignment Operator
TYPED_TEST(MapTest, MoveAssignmentOperator) {
  s21::map<typename TypeParam::first_type, typename TypeParam::second_type>
      s21_temp_map{this->s21_map_};

  this->empty_s21_map_ = std::move(this->s21_map_);

  EXPECT_TRUE(this->s21_map_.empty());
  EXPECT_EQ(this->empty_s21_map_.size(), size_t{8});

  EXPECT_EQ(s21_temp_map, this->empty_s21_map_);
}

// Erase
TYPED_TEST(MapTest, EraseIteratorPos) {
  auto it = this->s21_map_.find(103);
  EXPECT_NE(it, this->s21_map_.end());
  this->s21_map_.erase(it);

  EXPECT_EQ(this->s21_map_.size(), size_t{7});
  EXPECT_THROW(this->s21_map_.at(103), std::out_of_range);

  it = this->s21_map_.find(102);
  EXPECT_NE(it, this->s21_map_.end());
  this->s21_map_.erase(it);

  EXPECT_EQ(this->s21_map_.size(), size_t{6});
  EXPECT_THROW(this->s21_map_.at(102), std::out_of_range);
}

// Swap
TYPED_TEST(MapTest, SwapMaps) {
  s21::map<typename TypeParam::first_type, typename TypeParam::second_type>
      map1{{1, "one"}, {2, "two"}};
  s21::map<typename TypeParam::first_type, typename TypeParam::second_type>
      map2{{3, "three"}, {4, "four"}, {5, "five"}};

  map1.swap(map2);

  EXPECT_EQ(map1.size(), size_t{3});
  EXPECT_EQ(map2.size(), size_t{2});

  EXPECT_EQ(map1.at(3), "three");
  EXPECT_EQ(map1.at(4), "four");
  EXPECT_EQ(map1.at(5), "five");

  EXPECT_EQ(map2.at(1), "one");
  EXPECT_EQ(map2.at(2), "two");
}

// Merge
TYPED_TEST(MapTest, MergeMaps) {
  s21::map<typename TypeParam::first_type, typename TypeParam::second_type>
      other{{200, "extra"}, {201, "more"}};
  size_t initial_size = this->s21_map_.size();

  this->s21_map_.merge(other);

  EXPECT_EQ(this->s21_map_.size(), initial_size + 2);
  EXPECT_EQ(this->s21_map_.at(200), "extra");
  EXPECT_EQ(this->s21_map_.at(201), "more");
  EXPECT_TRUE(other.empty());
}

// Find
TYPED_TEST(MapTest, Find) {
  auto it = this->s21_map_.find(103);
  EXPECT_NE(it, this->s21_map_.end());
  EXPECT_EQ(it->second, "a");

  it = this->s21_map_.find(999);
  EXPECT_EQ(it, this->s21_map_.end());
}

// Contains
TYPED_TEST(MapTest, Contains) {
  EXPECT_TRUE(this->s21_map_.contains(103));
  EXPECT_FALSE(this->s21_map_.contains(999));
  EXPECT_FALSE(this->empty_s21_map_.contains(1));
}