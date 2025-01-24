#include <cstddef>
#include <cstdint>
#include <iostream>
#include <utility>

#include "s21_base.h"

namespace s21 {

enum class NodeColor : int8_t { RED = 0, BLACK = 1 };

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
template <typename Key, typename T>
class Node {
 public:
  std::pair<const Key, T> data_;
  Node* left_{nullptr};
  Node* right_{nullptr};
  Node* parent_{nullptr};
  NodeColor color_{NodeColor::RED};

  Node(const Key& key, const T& value) : data_(key, value) {}
};
#pragma GCC diagnostic pop

template <typename Key, typename T>
class Tree;

template <typename Key, typename T>
void print_tree(const Tree<Key, T>& tree);

template <typename Key, typename T>
class Tree {
 public:
  using node = Node<Key, T>;
  using value_type = std::pair<const Key, T>;
  // using key_type = Key;
  // using mapped_type = T;
  // using value_type = typename traits::value_type;
  // using pointer = T*;
  // using const_pointer = const T*;
  // using reference = typename traits::reference;
  // using const_reference = typename traits::const_reference;
  using size_type = std::size_t;
  friend void print_tree<Key, T>(const Tree<Key, T>&);

  Tree() : root_(nullptr), size_(0) {}
  Tree(const Tree&) = delete;
  Tree& operator=(const Tree&) = delete;
  Tree(Tree&& other) = delete;
  Tree& operator=(Tree&& other) = delete;

  // void insert(const Key& key, const T& obj) {
  //   if (root_ == nullptr) {
  //     root_ = new node(key, obj);
  //   }
  //   if (root_->left_->data_.first < key) {
  //     insert(key, obj);

  //   }
  // }

  std::pair<node*, bool> insert(const value_type& value) {
    if (root_ == nullptr) {
      root_ = new node(value.first, value.second);
      root_->color_ = NodeColor::BLACK;
      ++size_;
      return {root_, true};
    } else {
      node* current = root_;
      while (true) {
        if (value.first < current->data_.first) {
          if (current->left_ == nullptr) {
            current->left_ = new node(value.first, value.second);
            current->left_->parent_ = current;
            ++size_;
            return {current->left_, true};
          }
          current = current->left_;
        } else if (value.first > current->data_.first) {
          if (current->right_ == nullptr) {
            current->right_ = new node(value.first, value.second);
            current->right_->parent_ = current;
            ++size_;
            return {current->right_, true};
          }
          current = current->right_;
        } else {
          return {current, false};
        }
      }
    }
  }

 private:
  node* root_;
  size_type size_;
};
template <typename KeyType, typename ValueType>
void print_tree_helper(const Node<KeyType, ValueType>* root,
                       std::string prefix = "", std::string child_prefix = "") {
  if (!root) {
    std::cout << prefix << "null\n";
    return;
  }

  std::cout << prefix << root->data_.first << "("
            << (root->color_ == NodeColor::RED ? "R" : "B") << ")\n";

  if (root->right_ || root->left_) {  // if at least one child exists
    std::cout << child_prefix << "│\n";
    std::cout << child_prefix << "├── RIGHT: ";
    print_tree_helper(root->right_, "", child_prefix + "│   ");

    std::cout << child_prefix << "│\n";
    std::cout << child_prefix << "└── LEFT: ";
    print_tree_helper(root->left_, "", child_prefix + "    ");
  }
}

// Main print function
template <typename Key, typename T>
void print_tree(const Tree<Key, T>& tree) {
  std::cout << "\nTree contents:\n";
  print_tree_helper(tree.root_);
  std::cout << "Size: " << tree.size_ << "\n\n";
}

}  // namespace s21
