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

template <typename Key, typename T>
class map {
 public:
  using value_type = typename Node<Key, T>::value_type;
  using key_type = Key;
  using mapped_type = T;
  using iterator = RedBlackTreeIterator<Key, false, T>;
  using const_iterator = RedBlackTreeIterator<Key, true, T>;

  map() : tree_{} {}

  map(std::initializer_list<value_type> const& items) : tree_{} {
    for (const auto& item : items) {
      tree_.insert(item);
    }
  }

  map(const map& m) : tree_{} {
    for (auto it = m.begin(); it != m.end(); ++it) {
      tree_.insert(*it);
    }
  }
  map(map&& m) noexcept : tree_(std::move(m.tree_)) {}

  ~map() = default;

  map& operator=(map&& m) noexcept {
    if (this != &m) {
      tree_ = std::move(m.tree_);
      m.tree_ = RedBlackTree<key_type, mapped_type>();
    }
    return *this;
  }

  T& at(const Key& key) {
    auto it = tree_.find(key);
    if (it == tree_.end()) {
      throw std::out_of_range("Out of range");
    }
    return it->second;
  }

  T& operator[](const Key& key) {
    auto it = tree_.find(key);
    if (it != tree_.end()) {
      return it->second;
    }

    return tree_.insert({key, T{}}).first->second;
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
    auto result = tree_.insert(value);
    return {iterator(result.first), result.second};
  }

  std::pair<iterator, bool> insert(const Key& key, const T& obj) {
    value_type value{key, obj};
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

  bool contains(const key_type& key) {
    return iterator(tree_.FindNode(key)) != tree_.end();
  }

 private:
  RedBlackTree<Key, mapped_type> tree_;
};
}  // namespace s21
