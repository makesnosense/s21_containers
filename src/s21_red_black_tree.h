#include <cstddef>
#include <cstdint>
#include <iostream>
#include <utility>

#include "s21_base.h"

namespace s21 {

enum class NodeColor : int8_t { RED = 0, BLACK = 1 };

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
template <typename KeyType, typename ValueType>
class Node {
 public:
  std::pair<const KeyType, ValueType> data_;
  Node* left_{nullptr};
  Node* right_{nullptr};
  Node* parent_{nullptr};
  NodeColor color_{NodeColor::RED};

  Node(const KeyType& key, const ValueType& value) : data_(key, value) {}
};
#pragma GCC diagnostic pop

template <typename KeyType, typename ValueType>
class Tree;

template <typename KeyType, typename ValueType>
void print_tree(const Tree<KeyType, ValueType>& tree);

template <typename KeyType, typename ValueType>
class Tree {
 public:
  using node = Node<KeyType, ValueType>;
  using node_data_type = std::pair<const KeyType, ValueType>;
  using key_type = KeyType;
  using mapped_type = ValueType;
  // using value_type = typename traits::value_type;
  // using pointer = T*;
  // using const_pointer = const T*;
  // using reference = typename traits::reference;
  // using const_reference = typename traits::const_reference;
  using size_type = std::size_t;
  friend void print_tree<KeyType, ValueType>(const Tree<KeyType, ValueType>&);

  Tree() : root_(nullptr), size_(0) {}
  Tree(const Tree&) = delete;
  Tree& operator=(const Tree&) = delete;
  Tree(Tree&& other) = delete;
  Tree& operator=(Tree&& other) = delete;

 private:
  node* root_;
  size_type size_;
};

template <typename KeyType, typename ValueType>
void print_tree_helper(const Node<KeyType, ValueType>* root,
                       std::string prefix = "", bool is_left = false) {
  if (!root) return;

  std::cout << prefix;
  std::cout << (is_left ? "├──" : "└──");

  std::cout << root->data_.first << "("
            << (root->color_ == NodeColor::RED ? "R" : "B") << ")" << std::endl;

  std::string child_prefix = prefix + (is_left ? "│   " : "    ");

  print_tree_helper(root->left_, child_prefix, true);
  print_tree_helper(root->right_, child_prefix, false);
}

// Main print function
template <typename KeyType, typename ValueType>
void print_tree(const Tree<KeyType, ValueType>& tree) {
  std::cout << "\nTree contents:\n";
  print_tree_helper(tree.root_);
  std::cout << "Size: " << tree.size_ << "\n\n";
}

}  // namespace s21
