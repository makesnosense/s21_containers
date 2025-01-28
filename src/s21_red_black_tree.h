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
          InsertFixup(current->left_);

          return {current->left_, true};
        }
        current = current->left_;
      } else if (value.first > current->data_.first) {
        if (current->right_ == nullptr) {
          current->right_ = new node(value.first, value.second);
          current->right_->parent_ = current;
          ++size_;
          InsertFixup(current->right_);
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
    node* target = FindNode(key);
    if (target == nullptr) {
      return;  // Key not found
    }

    RemoveNode(target);
  }

  node* get_root() const { return root_; }

 private:
  void RemoveNode(node* removal_target) {
    node* replacement{nullptr};    // Node that will take target's position
    node* node_to_fixup{nullptr};  // Node that might need RB property fixes
    node* parent_of_node_to_fixup{nullptr};  // Parent of node_to_fixup

    // When removing a node, removed_node_original_color stores the color of the
    // node whose position will be physically empty after all the moves
    NodeColor removed_node_original_color{removal_target->color_};

    // Case 1: removal_target has at most one child
    if (removal_target->left_ == nullptr) {
      replacement = removal_target->right_;  // replacement might be nullptr
      node_to_fixup = replacement;  // here node_to_fixup IS the replacement
      parent_of_node_to_fixup =
          removal_target->parent_;  // cause replacement can be nullptr
      Transplant(removal_target, replacement);
    } else if (removal_target->right_ == nullptr) {
      replacement = removal_target->left_;
      node_to_fixup = replacement;  // same here
      parent_of_node_to_fixup = removal_target->parent_;
      Transplant(removal_target, replacement);
    }
    // Case 2: removal_target has both children
    else {
      replacement =
          GetMin(removal_target->right_);  // successor becomes replacement

      removed_node_original_color =
          replacement->color_;  // We store successor's color because
      // successor's original position becomes empty

      node_to_fixup =
          replacement->right_;  // node_to_fixup is NOT the replacement

      //  The replacement (successor) is a direct child of removal target
      if (replacement->parent_ == removal_target) {
        // parent_of_node_to_fixup becomes the replacement because after
        // restructuring, node_to_fixup will be directly under the replacement
        parent_of_node_to_fixup = replacement;
      }
      // The replacement (successor) is deeper in the right subtree
      else {
        parent_of_node_to_fixup = replacement->parent_;
        Transplant(replacement, node_to_fixup);
        // ensuring the successor takes over removed target's connections
        replacement->right_ = removal_target->right_;
        replacement->right_->parent_ = replacement;
      }

      Transplant(removal_target, replacement);
      replacement->left_ = removal_target->left_;
      replacement->left_->parent_ = replacement;
      replacement->color_ = removal_target->color_;
    }

    delete removal_target;
    --size_;
    if (removed_node_original_color == NodeColor::BLACK) {
      RemovalFixup(node_to_fixup, parent_of_node_to_fixup);
    }
  }

  void RemovalFixup(node* x, node* parent_of_x) {
    if (IsRoot(x)) {
      x->color_ = NodeColor::BLACK;
      std::cout << x << parent_of_x << "neiw";
    }
    // print_tree(*this);

    // case 4
    if (SiblingIsRed(x, parent_of_x)) {
      node* sibling{GetSibling(x, parent_of_x)};
      SwapColors(parent_of_x, sibling);

      print_tree(*this);
      if (IsRightChild(sibling)) {
        RotateLeft(parent_of_x);
      }
    }

    node* sibling{GetSibling(x, parent_of_x)};

    node* siblings_child_near_to_x{IsRightChild(sibling) ? sibling->left_
                                                         : sibling->right_};

    node* siblings_child_far_from_x{IsRightChild(sibling) ? sibling->right_
                                                          : sibling->left_};

    // case 5
    if (SiblingIsBlack(x, parent_of_x) &&
        siblings_child_near_to_x->color_ == NodeColor::RED &&
        IsBlack(siblings_child_far_from_x)) {
      SwapColors(sibling, siblings_child_near_to_x);

      IsRightChild(sibling) ? RotateRight(sibling) : RotateLeft(sibling);
      print_tree(*this);
    }

    sibling = GetSibling(x, parent_of_x);
    siblings_child_near_to_x =
        IsRightChild(sibling) ? sibling->left_ : sibling->right_;
    siblings_child_far_from_x =
        IsRightChild(sibling) ? sibling->right_ : sibling->left_;
    // std::cout << SiblingIsBlack(x, parent_of_x);
    // case 6
    if (SiblingIsBlack(x, parent_of_x) &&
        siblings_child_far_from_x->color_ == NodeColor::RED &&
        IsBlack(siblings_child_near_to_x)) {
      SwapColors(parent_of_x, sibling);
      IsRightChild(sibling) ? RotateLeft(parent_of_x)
                            : RotateRight(parent_of_x);
      siblings_child_far_from_x->color_ = NodeColor::BLACK;
    }
    print_tree(*this);
  }

#if 0
  void RemovalFixup2(node* x, node* parent_of_x) {
    // x might be nullptr, representing a "double black" node
    while (x != root_ && (x == nullptr || x->color_ == NodeColor::BLACK)) {
      // Left case: x is left child
      if (x == parent_of_x->left_) {
        node* w = parent_of_x->right_;  // w is x's sibling
        if (w == nullptr) break;        // Protect against nullptr sibling

        // Case 1: Sibling w is red
        if (w->color_ == NodeColor::RED) {
          w->color_ = NodeColor::BLACK;
          parent_of_x->color_ = NodeColor::RED;
          RotateLeft(parent_of_x);
          w = parent_of_x->right_;
          if (w == nullptr) break;  // Check again after rotation
        }

        // Case 2: Both of w's children are black
        if ((w->left_ == nullptr || w->left_->color_ == NodeColor::BLACK) &&
            (w->right_ == nullptr || w->right_->color_ == NodeColor::BLACK)) {
          w->color_ = NodeColor::RED;
          x = parent_of_x;
          parent_of_x = x->parent_;
        } else {
          // Case 3: w's right child is black (left is red)
          if (w->right_ == nullptr || w->right_->color_ == NodeColor::BLACK) {
            if (w->left_ != nullptr) {
              w->left_->color_ = NodeColor::BLACK;
            }
            w->color_ = NodeColor::RED;
            RotateRight(w);
            w = parent_of_x->right_;
            if (w == nullptr) break;
          }

          // Case 4: w's right child is red
          w->color_ = parent_of_x->color_;
          parent_of_x->color_ = NodeColor::BLACK;
          if (w->right_ != nullptr) {
            w->right_->color_ = NodeColor::BLACK;
          }
          RotateLeft(parent_of_x);
          x = root_;  // Terminates the loop
        }
      }
      // Right case: x is right child (mirror of left case)
      else {
        node* w = parent_of_x->left_;  // w is x's sibling
        if (w == nullptr) break;       // Protect against nullptr sibling

        // Case 1: Sibling w is red
        if (w->color_ == NodeColor::RED) {
          w->color_ = NodeColor::BLACK;
          parent_of_x->color_ = NodeColor::RED;
          RotateRight(parent_of_x);
          w = parent_of_x->left_;
          if (w == nullptr) break;  // Check again after rotation
        }

        // Case 2: Both of w's children are black
        if ((w->left_ == nullptr || w->left_->color_ == NodeColor::BLACK) &&
            (w->right_ == nullptr || w->right_->color_ == NodeColor::BLACK)) {
          w->color_ = NodeColor::RED;
          x = parent_of_x;
          parent_of_x = x->parent_;
        } else {
          // Case 3: w's left child is black (right is red)
          if (w->left_ == nullptr || w->left_->color_ == NodeColor::BLACK) {
            if (w->right_ != nullptr) {
              w->right_->color_ = NodeColor::BLACK;
            }
            w->color_ = NodeColor::RED;
            RotateLeft(w);
            w = parent_of_x->left_;
            if (w == nullptr) break;
          }

          // Case 4: w's left child is red
          w->color_ = parent_of_x->color_;
          parent_of_x->color_ = NodeColor::BLACK;
          if (w->left_ != nullptr) {
            w->left_->color_ = NodeColor::BLACK;
          }
          RotateRight(parent_of_x);
          x = root_;  // Terminates the loop
        }
      }
    }

    // Ensure root and x (if it exists) are black
    if (x != nullptr) {
      x->color_ = NodeColor::BLACK;
    }
    root_->color_ = NodeColor::BLACK;
  }
#endif

  void Transplant(node* old_node, node* new_node) {
    if (IsRoot(old_node)) {
      root_ = new_node;
      new_node->parent_ = nullptr;
      return;
    }

    node* father{old_node->parent_};
    if (IsLeftChild(old_node)) {
      father->left_ = new_node;
    } else {
      father->right_ = new_node;
    }

    if (new_node != nullptr) {
      new_node->parent_ = father;
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

  void InsertFixup(node* us) {
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
      InsertFixup(grandfather);
    } else {
      // uncle color is black
      if (IsInnerChild(us)) {
        InsertFixupTreatInnerChild(us);
      } else {
        InsertFixupTreatOuterChild(us);
      }
    }
    // Ensure root remains black after any modifications
    root_->color_ = NodeColor::BLACK;
  }

  void InsertFixupTreatOuterChild(node* us) {
    node* father{us->parent_};
    node* grandfather{us->parent_->parent_};

    SwapColors(father, grandfather);
    if (IsLeftChild(father)) {
      RotateRight(grandfather);
    } else {
      RotateLeft(grandfather);
    }
  }

  void InsertFixupTreatInnerChild(node* us) {
    node* father{us->parent_};
    if (IsLeftChild(father)) {
      RotateLeft(father);
      InsertFixupTreatOuterChild(us->left_);
    } else {
      RotateRight(father);
      InsertFixupTreatOuterChild(us->right_);
    }
  }

  void SwapColors(node* first, node* other) {
    NodeColor temp{first->color_};
    first->color_ = other->color_;
    other->color_ = temp;
  }

  bool IsBlack(node* x) {
    if (x == nullptr) {
      return true;
    } else {
      return x->color_ == NodeColor::BLACK;
    }
  }

  node* GetSiblingFromParent(node* parent) {
    if (parent->left_ == nullptr) {
      return parent->right_;
    } else {
      return parent->left_;
    }
  }

  node* GetSibling(node* us, node* parent) {
    if (us == nullptr) {
      return GetSiblingFromParent(parent);
    }
    if (IsLeftChild(us)) {
      return parent->right_;
    } else {
      return parent->left_;
    }
  }

  bool SiblingIsRed(node* us, node* parent) {
    node* sibling{GetSibling(us, parent)};
    return sibling && sibling->color_ == NodeColor::RED;
  }

  bool SiblingIsBlack(node* us, node* parent) {
    return !SiblingIsRed(us, parent);
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

  bool IsRoot(node* us) {
    if (us == nullptr) {
      return false;
    }
    return us->parent_ == nullptr;
  }

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
