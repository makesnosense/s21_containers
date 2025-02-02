#ifndef S21_MAP_H
#define S21_MAP_H

#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <iostream>
#include <type_traits>
#include <utility>

#include "s21_red_black_tree.h"
#include "s21_vector.h"

namespace s21 {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

template <typename Key, typename T>
class map {
 public:
  using value_type = std::pair<const Key, T>;
  using key_type = Key;
  using mapped_type = T;
  using node_type = Node<Key, T>;
  using iterator = RedBlackTreeIterator<Key, false, T>;
  using const_iterator = RedBlackTreeIterator<Key, true, T>;
  using size_type = std::size_t;

  map() : tree_{} {}

  map(std::initializer_list<value_type> const& items) : tree_{} {
    for (const auto& item : items) {
      insert(item);
    }
  }

  map(const map& m) : tree_{} {
    for (auto it = m.begin(); it != m.end(); ++it) {
      insert(*it);
    }
  }
  map(map&& m) noexcept : tree_(std::move(m.tree_)) {}

  ~map() = default;

  map& operator=(map&& other) noexcept {
    if (this != &other) {
      tree_ = std::move(other.tree_);
      other.tree_ = RedBlackTree<key_type, mapped_type>();
    }
    return *this;
  }

  mapped_type& at(const key_type& key) {
    auto node = tree_.FindNode(key);
    if (node == nullptr) {
      throw std::out_of_range("Out of range");
    }
    return node->data_.second;
  }

  mapped_type& operator[](const key_type& key) {
    auto it = tree_.FindNode(key);
    if (it != nullptr) {
      return it->data_.second;
    }

    auto result = tree_.insert({key, mapped_type{}});
    mapped_type& result_value = result.first->data_.second;
    // return tree_.insert({key, mapped_type{}}).first->data_.second;
    return result_value;
  }

  iterator begin() { return tree_.begin(); }
  iterator end() { return tree_.end(); }

  const_iterator begin() const { return tree_.begin(); }
  const_iterator end() const { return tree_.end(); }

  bool empty() { return tree_.empty(); }

  size_type size() const { return tree_.size(); }

  size_type max_size() {
    return std::numeric_limits<size_type>::max() / sizeof(key_type) +
           sizeof(mapped_type);
  }

  void clear() { tree_.clear(); }

  std::pair<iterator, bool> insert(const value_type& value) {
    key_type key = value.first;
    node_type* found{tree_.FindNode(key)};
    if (found) {
      return {iterator(found), false};
    } else {
      auto result = tree_.insert(value);
      return {iterator(result.first), result.second};
    }
  }

  std::pair<iterator, bool> insert(const key_type& key, const T& obj) {
    node_type* found{tree_.FindNode(key)};
    if (found) {
      return {iterator(found), false};
    } else {
      value_type value{key, obj};
      auto result = tree_.insert(value);
      return {iterator(result.first), result.second};
    }
  }

  template <typename... Args>
  s21::vector<std::pair<node_type*, bool>> insert_many(Args&&... args) {
    s21::vector<std::pair<node_type*, bool>> results;
    for (const auto& value : {std::forward<Args>(args)...}) {
      node_type* found{tree_.FindNode(value.first)};
      if (found) {
        continue;
      } else {
        auto result = tree_.insert(value);
        std::pair<node_type*, bool> pair =
            std::make_pair(result.first, result.second);
        results.push_back(pair);
      }
    }
    return results;
  }

  iterator erase(iterator pos) { return tree_.erase(pos); }
  void swap(map& other) noexcept { std::swap(tree_, other.tree_); }
  void merge(map& other) {
    for (auto it = other.begin(); it != other.end();) {
      insert(*it);
      it = other.erase(it);
    }
  }

  iterator find(const key_type& key) { return iterator(tree_.FindNode(key)); }

  bool contains(const key_type& key) {
    return iterator(tree_.FindNode(key)) != tree_.end();
  }

  bool operator==(const map& other) const {
    if (this->size() != other.size()) return false;
    auto it1 = begin();
    auto it2 = other.begin();

    while (it1 != end() && it2 != other.end()) {
      if (*it1 != *it2) return false;
      ++it1;
      ++it2;
    }

    return it1 == end() && it2 == other.end();
  }

 private:
  RedBlackTree<Key, mapped_type> tree_;
};
}  // namespace s21

#endif  // S21_MAP_H
