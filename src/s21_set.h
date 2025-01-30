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
  // using value_type = typename node_type::value_type;
  using key_type = Key;
  using value_type = key_type;
  using iterator = RedBlackTreeIterator<Key, false, void>;
  using const_iterator = RedBlackTreeIterator<Key, true, void>;
  using size_type = std::size_t;

  set() : tree_{} {}
  set(std::initializer_list<value_type> const& items) {
    for (const auto& i : items) {
      tree_.insert(i);
    }
  }
  // set(const set &s){}
  // set(set &&s){}

  explicit set(size_type size, value_type value) : tree_{} {
    for (size_type i{0}; i < size; i++) {
      tree_.insert(value);
    }
  }

  ~set() = default;

  // operator=(set &&s){}

  iterator begin() { return tree_.begin(); }
  iterator end() { return tree_.end(); }

  bool empty() { return tree_.size() == 0; }
  size_type size() { return tree_.size(); }
  size_type max_size() {
    return std::numeric_limits<size_type>::max() / sizeof(Key);
  }

  // void clear(){}
  std::pair<iterator, bool> insert(const value_type& value) {
    auto result = tree_.insert(value);
    return {iterator(result.first), result.second};
  }
  void erase(iterator pos) {
    node_type node_to_remove = pos.current_;
    tree_.erase(node_to_remove.GetKey());
  }
  // void swap(set& other){}
  // void merge(set& other){}

  iterator find(const Key& key) { return iterator(tree_.FindNode(key)); }
  bool contains(const Key& key) {
    return iterator(tree_.FindNode(key)) != nullptr;
  }

 private:
  RedBlackTree<Key> tree_;
};

}  // namespace s21
