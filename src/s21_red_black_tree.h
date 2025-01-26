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
      root_->parent_ = nullptr;
      root_->color_ = NodeColor::BLACK;
      ++size_;
      return {root_, true};
    }
    node* current = root_;
    while (true) {
      if (value.first < current->data_.first) {
        if (current->left_ == nullptr) {
          current->left_ = new node(value.first, value.second);
          current->left_->color_ = NodeColor::BLACK;
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

 private:
  void CheckValidity(node* us) {
    if (IsRoot(us)) {
      return;
    }
    if (us->parent_->color_ == NodeColor::RED) {
      return;
    }

    if (GrandFatherExists(us)) {
      node* uncle{GetUncle(us)};
      if (uncle->color_ == NodeColor::RED) {
        // Recolor(us);
      } else {
        // uncle color is black
        if (IsInnerChild(us)) {
          TreatInnerChild(us);
        }

        TreatOuterChild(us);
      }
    }
  }

  bool IsInnerChild(node* us) {
    return (IsLeftChild(us) && IsLeftChild(GetUncle(us))) ||
           (IsRightChild(us) && IsRightChild(GetUncle(us)));
  }
  void TreatInnerChild(node* us) {
    if (IsLeftChild(us)) {
      // MinorRotation(us);
    } else {
      // OtherMinorRotation(us);
    }
  }

  void TreatOuterChild(node* us) {
    if (IsLeftChild(us)) {
    }
    {
    }
    // MinorRotation(us);
  }

  bool IsLeftChild(node* us) {
    return us->parent_ && (us == us->parent_->left_);
  }
  bool IsRightChild(node* us) { return !IsLeftChild(us); };

  bool IsRoot(node* us) { return us->parent_ == nullptr; };
  bool GrandFatherExists(node* us) { return us->parent_->parent_ != nullptr; }

  node* GetUncle(node* us) {
    node* father{us->parent_};
    if (IsLeftChild(father)) {
      return father->parent_->right_;
    } else {
      return father->parent_->left_;
    }
  }

  node* root_;
  size_type size_;
};

namespace print_color {  // Anonymous namespace for color codes
const char* RED = "\033[1;31m";
const char* BLACK = "\033[2;37m";
const char* RESET = "\033[0m";
}  // namespace print_color

template <typename KeyType, typename ValueType>
void print_tree_helper(const Node<KeyType, ValueType>* root,
                       std::string prefix = "", std::string child_prefix = "") {
  if (!root) {
    std::cout << print_color::BLACK << prefix << "null" << print_color::RESET
              << '\n';
    return;
  }

  std::cout << prefix;
  std::cout << (root->color_ == NodeColor::RED ? print_color::RED
                                               : print_color::BLACK)
            << root->data_.first << "("
            << (root->color_ == NodeColor::RED ? "R" : "B") << ")"
            << print_color::RESET << "\n";

  if (root->right_ || root->left_) {
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
