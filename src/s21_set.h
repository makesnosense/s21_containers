#ifndef S21_SET_H
#define S21_SET_H

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

template <typename Key>
class set {
 public:
  using node_type = Node<Key>;
  using key_type = Key;
  using value_type = Key;
  using iterator = RedBlackTreeIterator<Key, false, void>;
  using const_iterator = RedBlackTreeIterator<Key, true, void>;
  using size_type = std::size_t;

  set() : tree_{} {}
  set(std::initializer_list<value_type> const& items) : tree_{} {
    for (const auto& i : items) {
      insert(i);
    }
  }
  set(const set& other) : tree_{} {
    for (auto it = other.begin(); it != other.end(); ++it) {
      insert(*it);
    }
  }
  set(set&& other) noexcept : tree_(std::move(other.tree_)) {}

  explicit set(size_type size, value_type value) : tree_{} {
    for (size_type i{0}; i < size; i++) {
      insert(value);
    }
  }

  ~set() = default;

  set& operator=(set&& other) noexcept {
    if (this != &other) {
      tree_ = std::move(other.tree_);
      other.tree_ = RedBlackTree<key_type>();
    }
    return *this;
  }

  iterator begin() { return tree_.begin(); }
  iterator end() { return tree_.end(); }

  const_iterator begin() const { return tree_.begin(); }
  const_iterator end() const { return tree_.end(); }

  bool empty() { return tree_.empty(); }
  size_type size() const { return tree_.size(); }
  size_type max_size() {
    return std::numeric_limits<size_type>::max() / sizeof(key_type);
  }

  void clear() { tree_.clear(); }

  std::pair<iterator, bool> insert(const value_type& value) {
    // let's try to find first
    node_type* found{tree_.FindNode(value)};
    if (found) {
      return {iterator(found), false};
    } else {
      auto result = tree_.insert(value);
      return {iterator(result.first), result.second};
    }
  }

  template <typename... Args>
  s21::vector<std::pair<node_type*, bool>> insert_many(Args&&... args) {
    s21::vector<std::pair<node_type*, bool>> results;
    for (const auto& value : {std::forward<Args>(args)...}) {
      node_type* found{tree_.FindNode(value)};
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

  void swap(set& other) noexcept { std::swap(tree_, other.tree_); }

  void merge(set& other) {
    for (auto it = other.begin(); it != other.end();) {
      insert(*it);
      it = other.erase(it);
    }
  }

  iterator find(const key_type& key) { return iterator(tree_.FindNode(key)); }
  bool contains(const key_type& key) {
    return iterator(tree_.FindNode(key)) != tree_.end();
  }

  bool operator==(const set& other) const {
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
  RedBlackTree<key_type> tree_;
};

}  // namespace s21

#endif  // S21_SET_H
