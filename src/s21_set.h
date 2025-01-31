#include <cstddef>
#include <cstdint>
#include <iostream>
#include <type_traits>
#include <utility>

#include "s21_base.h"
#include "s21_red_black_tree.h"

namespace s21 {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

template <typename Key>
class set {
 public:
  using node_type = Node<Key, void>;
  using key_type = Key;
  using value_type = Key;
  using iterator = RedBlackTreeIterator<Key, false, void>;
  using const_iterator = RedBlackTreeIterator<Key, true, void>;
  using size_type = std::size_t;

  set() : tree_{} {}
  set(std::initializer_list<value_type> const& items) : tree_{} {
    for (const auto& i : items) {
      tree_.insert(i);
    }
  }
  set(const set& other) : tree_{} {
    for (auto it = other.begin(); it != other.end(); ++it) {
      tree_.insert(*it);
    }
  }
  set(set&& other) noexcept : tree_(std::move(other.tree_)) {}

  explicit set(size_type size, value_type value) : tree_{} {
    for (size_type i{0}; i < size; i++) {
      tree_.insert(value);
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
    auto result = tree_.insert(value);
    return {iterator(result.first), result.second};
  }

  iterator erase(iterator pos) { return tree_.erase(pos); }

  void swap(set& other) noexcept { std::swap(tree_, other.tree_); }

  void merge(set& other) {
    for (auto it = other.begin(); it != other.end();) {
      tree_.insert(*it);
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
