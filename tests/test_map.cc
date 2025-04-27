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
                 {key_type(103), mapped_type("dup")},
                 {key_type(53), mapped_type("b")},
                 {key_type(73), mapped_type("c")},
                 {key_type(106), mapped_type("d")},
                 {key_type(57), mapped_type("e")},
                 {key_type(102), mapped_type("f")},
                 {key_type(46), mapped_type("g")},
                 {key_type(104), mapped_type("h")}},
        s21_map_{{key_type(103), mapped_type("a")},
                 {key_type(103), mapped_type("dup")},
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

TYPED_TEST(MapTest, InsertMany) {
  std::vector<TypeParam> values = {
      {1, "a"}, {2, "g"}, {3, "d"}, {4, "j"}, {5, "k"}};

  this->empty_s21_map_.insert_many(values[0], values[1], values[2], values[3],
                                   values[4]);

  EXPECT_EQ(this->empty_s21_map_.size(), values.size());

  auto s21_it = this->empty_s21_map_.begin();

  for (size_t i = 0; i < values.size(); i++) {
    EXPECT_EQ((*s21_it).second, values[i].second);
    *s21_it++;
  }
}

TYPED_TEST(MapTest, InsertManyDuplicates) {
  std::vector<TypeParam> values = {
      {1, "a"}, {2, "g"}, {2, "d"}, {4, "j"}, {5, "k"}};

  auto results = this->empty_s21_map_.insert_many(
      values[0], values[1], values[2], values[3], values[4]);

  EXPECT_EQ(this->empty_s21_map_.size(), size_t{4});

  EXPECT_NE(results.size(), values.size());

  std::vector<TypeParam> result = {{1, "a"}, {2, "d"}, {4, "j"}, {5, "k"}};

  for (const auto& value : result) {
    EXPECT_TRUE(this->empty_s21_map_.contains(value.first));
  }

  EXPECT_TRUE(results[0].second);
  EXPECT_TRUE(results[1].second);
  EXPECT_TRUE(results[2].second);
  EXPECT_TRUE(results[3].second);
}

TYPED_TEST(MapTest, OperatorSqaerScobki) {
  s21::map<int, std::string> empty_s21_map_;
  this->empty_s21_map_[2] = "a";
  this->empty_s21_map_[2] = "b";
  this->empty_s21_map_[4] = "c";
  this->empty_s21_map_[3] = "d";

  std::vector<TypeParam> values = {{2, "b"}, {3, "d"}, {4, "c"}};
  auto s21_it = this->empty_s21_map_.begin();
  for (size_t i = 0; i < values.size(); i++) {
    EXPECT_EQ((*s21_it).second, values[i].second);
    *s21_it++;
  }

  EXPECT_EQ(this->empty_s21_map_.size(), size_t{3});
}

TYPED_TEST(MapTest, At) {
  EXPECT_EQ(this->s21_map_.at(103), "a");
  EXPECT_EQ(this->s21_map_.at(53), "b");
  EXPECT_EQ(this->s21_map_.at(73), "c");

  EXPECT_THROW(this->s21_map_.at(52), std::out_of_range);
  EXPECT_THROW(this->empty_s21_map_.at(1), std::out_of_range);
}

TYPED_TEST(MapTest, OperatorSubscript) {
  EXPECT_EQ(this->s21_map_[103], "a");

  typename TypeParam::first_type key{99};
  this->s21_map_[key] = "new";
  EXPECT_EQ(this->s21_map_[key], "new");
  EXPECT_EQ(this->s21_map_.size(), size_t{9});
}

TYPED_TEST(MapTest, CopyConstructor) {
  s21::map<typename TypeParam::first_type, typename TypeParam::second_type>
      s21_map_2(this->s21_map_);

  EXPECT_EQ(this->s21_map_, s21_map_2);
}

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

TYPED_TEST(MapTest, MoveAssignmentOperator) {
  s21::map<typename TypeParam::first_type, typename TypeParam::second_type>
      s21_temp_map{this->s21_map_};

  this->empty_s21_map_ = std::move(this->s21_map_);

  EXPECT_TRUE(this->s21_map_.empty());
  EXPECT_EQ(this->empty_s21_map_.size(), size_t{8});

  EXPECT_EQ(s21_temp_map, this->empty_s21_map_);
}

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

TYPED_TEST(MapTest, MergeMaps) {
  s21::map<typename TypeParam::first_type, typename TypeParam::second_type>
      other{{48, "extra"}, {49, "more"}};
  size_t initial_size = this->s21_map_.size();

  this->s21_map_.merge(other);

  EXPECT_EQ(this->s21_map_.size(), initial_size + 2);
  EXPECT_EQ(this->s21_map_.at(48), "extra");
  EXPECT_EQ(this->s21_map_.at(49), "more");
  EXPECT_TRUE(other.empty());
}

TYPED_TEST(MapTest, Find) {
  auto it = this->s21_map_.find(103);
  EXPECT_NE(it, this->s21_map_.end());
  EXPECT_EQ(it->second, "a");

  it = this->s21_map_.find(99);
  EXPECT_EQ(it, this->s21_map_.end());
}

TYPED_TEST(MapTest, Contains) {
  EXPECT_TRUE(this->s21_map_.contains(103));
  EXPECT_FALSE(this->s21_map_.contains(52));
  EXPECT_FALSE(this->empty_s21_map_.contains(1));
}

TEST(MapNonTyped, EraseIteratorPos_SingleElement) {
  s21::map<int, std::string> s21_map = {{10, "test"}};

  auto it = s21_map.begin();
  s21_map.erase(it);

  EXPECT_TRUE(s21_map.empty());
}

TEST(MapNonTyped, IteratorPos_NotFound) {
  s21::map<int, std::string> s21_map = {
      {1, "one"}, {2, "two"}, {3, "three"}, {4, "four"}, {5, "five"}};

  auto it = s21_map.find(6);
  EXPECT_EQ(it, s21_map.end());

  EXPECT_EQ(s21_map.size(), size_t{5});
}

TEST(MapNonTyped, Swap_EmptyAndNonEmpty) {
  s21::map<int, std::string> s21_map1;
  s21::map<int, std::string> s21_map2 = {{42, "test"}};

  s21_map1.swap(s21_map2);

  EXPECT_TRUE(s21_map2.empty());
  EXPECT_EQ(s21_map1.size(), size_t{1});
  EXPECT_EQ(s21_map1.at(42), "test");
}

TYPED_TEST(MapTest, Swap_BothEmpty) {
  s21::map<typename TypeParam::first_type, typename TypeParam::second_type>
      s21_map;

  this->empty_s21_map_.swap(s21_map);

  EXPECT_TRUE(this->empty_s21_map_.empty());
  EXPECT_TRUE(s21_map.empty());
}

TYPED_TEST(MapTest, Swap_SameElements) {
  s21::map<typename TypeParam::first_type, typename TypeParam::second_type>
      s21_map{this->s21_map_};

  s21_map.swap(this->s21_map_);

  EXPECT_EQ(s21_map.size(), size_t{8});
  EXPECT_EQ(this->s21_map_.size(), size_t{8});

  EXPECT_EQ(s21_map, this->s21_map_);
}

TEST(MapNonTyped, Merge_EmptyIntoNonEmpty) {
  s21::map<int, std::string> s21_map1 = {
      {10, "ten"}, {20, "twenty"}, {30, "thirty"}};
  s21::map<int, std::string> s21_map2;

  s21_map1.merge(s21_map2);

  EXPECT_EQ(s21_map1.size(), size_t{3});
  EXPECT_EQ(s21_map1.at(10), "ten");
  EXPECT_EQ(s21_map1.at(20), "twenty");
  EXPECT_EQ(s21_map1.at(30), "thirty");

  EXPECT_TRUE(s21_map2.empty());
}

TEST(MapNonTyped, Merge_NonEmptyIntoEmpty) {
  s21::map<int, std::string> s21_map1;
  s21::map<int, std::string> s21_map2 = {
      {100, "hundred"}, {200, "two hundred"}, {300, "three hundred"}};

  s21_map1.merge(s21_map2);

  EXPECT_EQ(s21_map1.size(), size_t{3});
  EXPECT_EQ(s21_map1.at(100), "hundred");
  EXPECT_EQ(s21_map1.at(200), "two hundred");
  EXPECT_EQ(s21_map1.at(300), "three hundred");

  EXPECT_TRUE(s21_map2.empty());
}

TEST(MapNonTyped, Merge_BothEmpty) {
  s21::map<int, std::string> s21_map1;
  s21::map<int, std::string> s21_map2;

  s21_map1.merge(s21_map2);

  EXPECT_TRUE(s21_map1.empty());
  EXPECT_TRUE(s21_map2.empty());
}

TEST(MapNonTyped, Merge_SameMaps) {
  s21::map<int, std::string> s21_map1 = {{1, "one"}, {2, "two"}, {3, "three"}};
  s21::map<int, std::string> s21_map2 = {{1, "one"}, {2, "two"}, {3, "three"}};

  s21_map1.merge(s21_map2);

  EXPECT_EQ(s21_map1.size(), size_t{3});
  EXPECT_EQ(s21_map1.at(1), "one");
  EXPECT_EQ(s21_map1.at(2), "two");
  EXPECT_EQ(s21_map1.at(3), "three");

  EXPECT_TRUE(s21_map2.empty());
}

TYPED_TEST(MapTest, Find_FirstElement) {
  auto s21_it = this->s21_map_.find(46);  // First element in the map
  auto std_it = this->stl_map_.find(46);

  EXPECT_NE(s21_it, this->s21_map_.end());
  EXPECT_NE(std_it, this->stl_map_.end());

  EXPECT_EQ(s21_it->first, std_it->first);
  EXPECT_EQ(s21_it->second, std_it->second);
}

TYPED_TEST(MapTest, Find_LastElement) {
  auto s21_it = this->s21_map_.find(106);  // Last element in the map
  auto std_it = this->stl_map_.find(106);

  EXPECT_NE(s21_it, this->s21_map_.end());
  EXPECT_NE(std_it, this->stl_map_.end());

  EXPECT_EQ(s21_it->first, std_it->first);
  EXPECT_EQ(s21_it->second, std_it->second);
}

TYPED_TEST(MapTest, Find_InEmptyMap) {
  auto s21_it = this->empty_s21_map_.find(3);
  auto std_it = this->empty_stl_map_.find(3);

  EXPECT_EQ(s21_it, this->empty_s21_map_.end());
  EXPECT_EQ(std_it, this->empty_stl_map_.end());
}
