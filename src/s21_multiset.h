#ifndef S21_MULTISET_H
#define S21_MULTISET_H

#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <iostream>
#include <type_traits>
#include <utility>

#include "s21_red_black_tree.h"

namespace s21 {

template <typename Key>
class multiset {
 public:
  using key_type = Key;
  using value_type = Key;
  using iterator = RedBlackTreeIterator<Key, false, void>;
  using const_iterator = RedBlackTreeIterator<Key, true, void>;
  using size_type = std::size_t;

  multiset() : tree_{} {}

  multiset(std::initializer_list<value_type> const& items) : tree_{} {
    for (const auto& i : items) {
      tree_.insert(i);
    }
  }

  multiset(const multiset& other) : tree_{} {
    for (auto it = other.begin(); it != other.end(); ++it) {
      tree_.insert(*it);
    }
  }

  multiset(multiset&& other) noexcept : tree_(std::move(other.tree_)) {}

  // Additional constructor for multiple identical elements
  explicit multiset(size_type size, value_type value) : tree_{} {
    for (size_type i{0}; i < size; i++) {
      tree_.insert(value);
    }
  }

  ~multiset() = default;

  multiset& operator=(multiset&& other) noexcept {
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

  // modified insert - always returns iterator, no bool needed
  iterator insert(const value_type& value) {
    auto result = tree_.insert(value);
    return iterator(result.first);
  }

  iterator erase(iterator pos) { return tree_.erase(pos); }

  // erase by key removes all occurrences
  size_type erase(const key_type& key) {
    size_type count = 0;
    auto it = find(key);
    while (it != end() && *it == key) {
      it = erase(it);
      ++count;
    }
    return count;
  }

  void swap(multiset& other) noexcept { std::swap(tree_, other.tree_); }

  void merge(multiset& other) {
    for (auto it = other.begin(); it != other.end();) {
      tree_.insert(*it);
      it = other.erase(it);
    }
  }

  // multiset-specific methods
  size_type count(const key_type& key) const {
    size_type count = 0;
    for (auto it = begin(); it != end(); ++it) {
      if (*it == key) ++count;
    }
    return count;
  }

  // Returns first occurrence
  iterator find(const key_type& key) { return iterator(tree_.FindNode(key)); }

  // Returns range of elements matching key
  std::pair<iterator, iterator> equal_range(const key_type& key) {
    iterator first = lower_bound(key);
    iterator last = upper_bound(key);
    return {first, last};
  }

  // First element not less than key
  iterator lower_bound(const key_type& key) {
    iterator it = begin();
    while (it != end() && *it < key) ++it;
    return it;
  }

  // First element greater than key
  iterator upper_bound(const key_type& key) {
    iterator it = begin();
    while (it != end() && *it <= key) ++it;
    return it;
  }

  bool contains(const key_type& key) { return find(key) != end(); }

  // Equality comparison needs to account for duplicates
  bool operator==(const multiset& other) const {
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

#endif  // S21_MULTISET_H
