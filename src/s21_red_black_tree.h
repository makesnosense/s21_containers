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
class RedBlackTree;

template <typename Key, typename T>
void print_tree(const RedBlackTree<Key, T>& tree);

template <typename Key, typename T>
class RedBlackTree {
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
  friend void print_tree<Key, T>(const RedBlackTree<Key, T>&);

  RedBlackTree() : root_(nullptr), size_(0) {}
  RedBlackTree(const RedBlackTree&) = delete;
  RedBlackTree(RedBlackTree&& other) = delete;
  ~RedBlackTree() {
    DeleteSubtree(root_);
    root_ = nullptr;
    size_ = 0;
  }
  RedBlackTree& operator=(const RedBlackTree&) = delete;
  RedBlackTree& operator=(RedBlackTree&& other) = delete;

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
          current->left_->parent_ = current;
          ++size_;
          InsertFixUp(current->left_);

          return {current->left_, true};
        }
        current = current->left_;
      } else if (value.first > current->data_.first) {
        if (current->right_ == nullptr) {
          current->right_ = new node(value.first, value.second);
          current->right_->parent_ = current;
          ++size_;
          InsertFixUp(current->right_);
          return {current->right_, true};
        }
        current = current->right_;
      } else {
        return {current, false};
      }
    }
  }

  void RotateLeft(node* us) {
    bool us_is_root{root_ == us};

    node* right_child{us->right_};
    node* displaced_left_child{right_child->left_};

    // right_child moves up, becoming parent of us
    right_child->parent_ = us->parent_;

    // if we had parent, update its child pointer
    if (us_is_root == false) {
      if (IsLeftChild(us)) {
        us->parent_->left_ = right_child;
      } else {
        us->parent_->right_ = right_child;
      }
    }

    // changing core rotation connections
    right_child->left_ = us;
    us->parent_ = right_child;

    // Reattach the displaced subtree
    us->right_ = displaced_left_child;
    if (displaced_left_child) {
      displaced_left_child->parent_ = us;
    }

    if (us_is_root) {
      root_ = right_child;
    }
  }

  void RotateRight(node* us) {
    bool us_is_root{root_ == us};

    node* left_child{us->left_};
    node* displaced_right_child{left_child->right_};

    // left_child moves up, becoming parent of us
    left_child->parent_ = us->parent_;

    // // if we had parent, update its child pointer
    if (us_is_root == false) {
      if (IsLeftChild(us)) {
        us->parent_->left_ = left_child;
      } else {
        us->parent_->right_ = left_child;
      }
    }

    // changing core rotation connections
    left_child->right_ = us;
    us->parent_ = left_child;

    // Reattach the displaced subtree
    us->left_ = displaced_right_child;
    if (displaced_right_child) {
      displaced_right_child->parent_ = us;
    }

    if (us_is_root) {
      root_ = left_child;
    }
  }

  node* FindNode(const Key& key) {
    if (root_ == nullptr) {
      return nullptr;
    }

    node* current{root_};
    while (true) {
      if (key < current->data_.first) {
        if (current->left_ == nullptr) {
          return nullptr;
        }
        current = current->left_;
      } else if (key > current->data_.first) {
        if (current->right_ == nullptr) {
          return nullptr;
        }
        current = current->right_;
      } else {
        return current;
      }
    }
  }

  void erase(const Key& key) {
    node* current = FindNode(key);
    if (current == nullptr) {
      return;
    }

    node* replacement_node{nullptr};

    // if node have one child or have no children
    if (current->left_ == nullptr) {
      replacement_node = current->right_;
      Transplant(current, current->right_);
    } else if (current->right_ == nullptr) {
      replacement_node = current->left_;
      Transplant(current, current->left_);
    } else {
      // if node have two children
      node* min_in_right_subtree = GetMin(current->right_);
      replacement_node = min_in_right_subtree->right_;

      if (min_in_right_subtree->parent_ != current) {
        Transplant(min_in_right_subtree, replacement_node);
        min_in_right_subtree->right_ = current->right_;
        min_in_right_subtree->right_->parent_ = min_in_right_subtree;
      }
      Transplant(current, min_in_right_subtree);
      min_in_right_subtree->left_ = current->left_;
      min_in_right_subtree->left_->parent_ = min_in_right_subtree;
      min_in_right_subtree->color_ = current->color_;
    }

    delete current;
    --size_;

    // if (original_color == NodeColor::BLACK) {
    //     EnsureValidityAfterDelete(replacement_node);
    // }
  }

  node* get_root() const { return root_; }

 private:
  void Transplant(node* old_node, node* new_node) {
    if (IsRoot(old_node)) {
      root_ = new_node;
    } else {
      if (IsLeftChild(old_node)) {
        old_node->parent_->left_ = new_node;
      } else {
        old_node->parent_->right_ = new_node;
      }
    }

    if (new_node != nullptr) {
      new_node->parent_ = old_node->parent_;
    }
  }

  node* GetMin(node* us) {
    node* current = us;
    while (current->left_ != nullptr) {
      current = current->left_;
    }
    return current;
  }

  node* GetMax(node* us) {
    node* current = us;
    while (current->right_ != nullptr) {
      current = current->right_;
    }
    return current;
  }

  void InsertFixUp(node* us) {
    if (IsRoot(us)) {
      us->color_ = NodeColor::BLACK;
      return;
    }
    if (us->parent_->color_ == NodeColor::BLACK) {
      return;
    }
    if (GrandFatherExists(us) == false) {
      us->parent_->color_ = NodeColor::BLACK;
      return;
    }
    // At this point we know:
    // 1. Node is not root
    // 2. Parent is red
    // 3. Grandfather exists

    node* grandfather = us->parent_->parent_;

    if (UncleIsRed(us)) {
      // father and uncle are red
      RecolorFatherUncleGrandpa(us);
      InsertFixUp(grandfather);
    } else {
      // uncle color is black
      if (IsInnerChild(us)) {
        InsertFixUpTreatInnerChild(us);
      } else {
        InsertFixUpTreatOuterChild(us);
      }
    }
    // Ensure root remains black after any modifications
    root_->color_ = NodeColor::BLACK;
  }

  void InsertFixUpTreatOuterChild(node* us) {
    node* father{us->parent_};
    node* grandfather{us->parent_->parent_};

    SwapColors(father, grandfather);
    if (IsLeftChild(father)) {
      RotateRight(grandfather);
    } else {
      RotateLeft(grandfather);
    }
  }

  void InsertFixUpTreatInnerChild(node* us) {
    node* father{us->parent_};
    if (IsLeftChild(father)) {
      RotateLeft(father);
      InsertFixUpTreatOuterChild(us->left_);
    } else {
      RotateRight(father);
      InsertFixUpTreatOuterChild(us->right_);
    }
  }

  void SwapColors(node* first, node* other) {
    NodeColor temp{first->color_};
    first->color_ = other->color_;
    other->color_ = temp;
  }

  bool UncleIsRed(node* us) {
    node* uncle{GetUncle(us)};
    if (uncle == nullptr) {
      return false;
    }
    return uncle->color_ == NodeColor::RED;
  }

  bool IsInnerChild(node* us) {
    node* father{us->parent_};
    return (IsLeftChild(us) && IsRightChild(father)) ||
           (IsRightChild(us) && IsLeftChild(father));
  }

  bool IsLeftChild(node* us) {
    return us->parent_ && (us == us->parent_->left_);
  }
  bool IsRightChild(node* us) { return !IsLeftChild(us); };

  bool IsRoot(node* us) { return us->parent_ == nullptr; };
  bool GrandFatherExists(node* us) {
    return us && us->parent_ && us->parent_->parent_;
  }

  node* GetUncle(node* us) {
    node* father{us->parent_};
    if (IsLeftChild(father)) {
      return father->parent_->right_;
    } else {
      return father->parent_->left_;
    }
  }

  void RecolorFatherUncleGrandpa(node* us) {
    if (GrandFatherExists(us) == false) {
      if (us->parent_) {
        us->parent_->color_ = NodeColor::BLACK;
      }
      return;
    }
    node* uncle{GetUncle(us)};
    node* father{us->parent_};
    node* grandfather{father->parent_};

    father->color_ = NodeColor::BLACK;
    if (uncle) {
      uncle->color_ = NodeColor::BLACK;
    }
    grandfather->color_ = NodeColor::RED;
  }

  void DeleteSubtree(node* n) {
    if (n) {
      DeleteSubtree(n->left_);
      DeleteSubtree(n->right_);
      delete n;
    }
  }

  node* root_;
  size_type size_;
};

namespace print_color {
const char* RED = "\033[1;31m";
const char* BLACK = "\033[2;37m";
const char* RESET = "\033[0m";
}  // namespace print_color

template <typename Key, typename T>
void print_tree_helper(const Node<Key, T>* root, std::string prefix = "",
                       std::string child_prefix = "") {
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
void print_tree(const RedBlackTree<Key, T>& tree) {
  std::cout << "\nTree contents:\n";
  print_tree_helper(tree.root_);
  std::cout << "Size: " << tree.size_ << "\n\n";
}

}  // namespace s21
