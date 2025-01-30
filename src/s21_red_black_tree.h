#include <cstddef>
#include <cstdint>
#include <iostream>
#include <type_traits>
#include <utility>

#include "s21_base.h"

namespace s21 {

enum class NodeColor : int8_t { RED = 0, BLACK = 1 };

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

template <typename Key, typename T = void>
class Node {
 public:
  using value_type = std::pair<const Key, T>;
  using key_type = Key;
  using mapped_type = T;

  value_type data_;
  Node* left_{nullptr};
  Node* right_{nullptr};
  Node* parent_{nullptr};
  NodeColor color_{NodeColor::RED};

  // Constructor for Map case
  Node(const key_type& key, const mapped_type& value) : data_(key, value) {}

  const key_type& GetKey() const { return data_.first; }
  const mapped_type& GetValue() const { return data_.second; }
};

// Specialization for Set case (T = void)
template <typename Key>
class Node<Key, void> {
 public:
  using value_type = Key;
  using key_type = Key;

  value_type data_;
  Node* left_{nullptr};
  Node* right_{nullptr};
  Node* parent_{nullptr};
  NodeColor color_{NodeColor::RED};

  // Constructor for Set case
  explicit Node(const Key& key) : data_(key) {}

  const key_type& GetKey() const { return data_; }
};

#pragma GCC diagnostic pop

template <typename Key, typename T>
class RedBlackTree;
template <typename Key, typename T = void>
class RedBlackTreeIterator;
template <typename Key, typename T>
void print_tree(const RedBlackTree<Key, T>& tree);

template <typename Key, typename T = void>
class RedBlackTree {
 public:
  using node_type = Node<Key, T>;
  using value_type = typename node_type::value_type;
  using key_type = Key;
  using mapped_type = T;
  using iterator = RedBlackTreeIterator<Key, T>;

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

  iterator begin() {
    if (!root_) {
      return end();
    }
    node_type* current = root_;
    while (current->left_) {
      current = current->left_;
    }
    return iterator(current);
  }

  iterator end() { return iterator(nullptr); }

  std::pair<node_type*, bool> insert(const value_type& value) {
    if (root_ == nullptr) {
      root_ = CreateNode(value);
      // root_ = new node(value.first, value.second);
      root_->parent_ = nullptr;
      root_->color_ = NodeColor::BLACK;
      ++size_;
      return {root_, true};
    }
    node_type* current = root_;
    const key_type& inserted_value_key{ExtractKeyFromAmbiguousValue(value)};
    while (true) {
      if (inserted_value_key < current->GetKey()) {
        if (current->left_ == nullptr) {
          current->left_ = CreateNode(value);
          // current->left_ = new node(value.first, value.second);
          current->left_->parent_ = current;
          ++size_;
          InsertFixup(current->left_);

          return {current->left_, true};
        }
        current = current->left_;
      } else if (inserted_value_key > current->GetKey()) {
        if (current->right_ == nullptr) {
          current->right_ = CreateNode(value);
          // current->right_ = new node(value.first, value.second);
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

  void RotateLeft(node_type* us) {
    bool us_is_root{root_ == us};

    node_type* right_child{us->right_};
    node_type* displaced_left_child{right_child->left_};

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

  void RotateRight(node_type* us) {
    bool us_is_root{root_ == us};

    node_type* left_child{us->left_};
    node_type* displaced_right_child{left_child->right_};

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

  node_type* FindNode(const key_type& key) {
    if (root_ == nullptr) {
      return nullptr;
    }

    node_type* current{root_};
    while (true) {
      if (key < current->GetKey()) {
        if (current->left_ == nullptr) {
          return nullptr;
        }
        current = current->left_;
      } else if (key > current->GetKey()) {
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
    node_type* target = FindNode(key);
    if (target == nullptr) {
      return;  // Key not found
    }

    RemoveNode(target);
  }

  node_type* get_root() const { return root_; }

 private:
  // Helper to get key from value_type
  const key_type& ExtractKeyFromAmbiguousValue(const value_type& value) const {
    if constexpr (std::is_same_v<T, void>) {
      return value;  // For Set: value is the key
    } else {
      return value.first;  // For Map: value is a pair
    }
  }

  // Helper for creating new node - specialized for Map/Set
  node_type* CreateNode(const value_type& value) {
    if constexpr (std::is_same_v<T, void>) {
      return new node_type(value);  // Set case
    } else {
      return new node_type(value.first, value.second);  // Map case
    }
  }

  void RemoveNode(node_type* removal_target) {
    // Case 0: we have to remove the only element
    if (removal_target == root_ && !root_->left_ && !root_->right_) {
      delete root_;
      root_ = nullptr;
      return;
    }

    node_type* replacement{nullptr};    // Node that will take target's position
    node_type* node_to_fixup{nullptr};  // Node that may need RB property fixes
    node_type* parent_of_node_to_fixup{nullptr};  // Parent of node_to_fixup

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

  void RemovalFixup(node_type* x, node_type* parent_of_x) {
    while (x != root_ && IsBlack(x)) {
      node_type* sibling = GetSibling(x, parent_of_x);

      // case 4: red sibling
      if (SiblingIsRed(x, parent_of_x)) {
        SwapColors(parent_of_x, sibling);

        if (IsRightChild(sibling)) {
          RotateLeft(parent_of_x);
        } else {
          RotateRight(parent_of_x);
        }

        sibling = GetSibling(x, parent_of_x);
      } else {  // black sibling

        node_type* siblings_child_far_from_x{
            IsRightChild(sibling) ? sibling->right_ : sibling->left_};

        node_type* siblings_child_near_to_x{
            IsRightChild(sibling) ? sibling->left_ : sibling->right_};

        // case 3: black sibling and both children black
        if (IsBlack(siblings_child_near_to_x) &&
            IsBlack(siblings_child_far_from_x)) {
          sibling->color_ = NodeColor::RED;
          x = parent_of_x;
          parent_of_x = parent_of_x->parent_;
        }
        // red child present
        else {
          // case 5 black sibling and near child red
          if (IsBlack(siblings_child_far_from_x) && siblings_child_near_to_x &&
              siblings_child_near_to_x->color_ == NodeColor::RED) {
            SwapColors(sibling, siblings_child_near_to_x);

            IsRightChild(sibling) ? RotateRight(sibling) : RotateLeft(sibling);

            // updating
            sibling = GetSibling(x, parent_of_x);
            siblings_child_near_to_x =
                IsRightChild(sibling) ? sibling->left_ : sibling->right_;
            siblings_child_far_from_x =
                IsRightChild(sibling) ? sibling->right_ : sibling->left_;
          }
          // case 6 black sibling and far child red
          SwapColors(parent_of_x, sibling);
          IsRightChild(sibling) ? RotateLeft(parent_of_x)
                                : RotateRight(parent_of_x);
          siblings_child_far_from_x->color_ = NodeColor::BLACK;
          x = root_;
        }
      }
    }
    x->color_ = NodeColor::BLACK;
  }

  void Transplant(node_type* old_node, node_type* new_node) {
    if (IsRoot(old_node)) {
      root_ = new_node;
      new_node->parent_ = nullptr;
      return;
    }

    node_type* father{old_node->parent_};
    if (IsLeftChild(old_node)) {
      father->left_ = new_node;
    } else {
      father->right_ = new_node;
    }

    if (new_node != nullptr) {
      new_node->parent_ = father;
    }
  }

  node_type* GetMin(node_type* us) {
    node_type* current = us;
    while (current->left_ != nullptr) {
      current = current->left_;
    }
    return current;
  }

  node_type* GetMax(node_type* us) {
    node_type* current = us;
    while (current->right_ != nullptr) {
      current = current->right_;
    }
    return current;
  }

  void InsertFixup(node_type* us) {
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

    node_type* grandfather = us->parent_->parent_;

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

  void InsertFixupTreatOuterChild(node_type* us) {
    node_type* father{us->parent_};
    node_type* grandfather{us->parent_->parent_};

    SwapColors(father, grandfather);
    if (IsLeftChild(father)) {
      RotateRight(grandfather);
    } else {
      RotateLeft(grandfather);
    }
  }

  void InsertFixupTreatInnerChild(node_type* us) {
    node_type* father{us->parent_};
    if (IsLeftChild(father)) {
      RotateLeft(father);
      InsertFixupTreatOuterChild(us->left_);
    } else {
      RotateRight(father);
      InsertFixupTreatOuterChild(us->right_);
    }
  }

  void SwapColors(node_type* first, node_type* other) {
    NodeColor temp{first->color_};
    first->color_ = other->color_;
    other->color_ = temp;
  }

  bool IsBlack(node_type* x) {
    if (x == nullptr) {
      return true;
    } else {
      return x->color_ == NodeColor::BLACK;
    }
  }

  node_type* GetSiblingFromParent(node_type* parent) {
    if (parent->left_ == nullptr) {
      return parent->right_;
    } else {
      return parent->left_;
    }
  }

  node_type* GetSibling(node_type* us, node_type* parent) {
    if (us == nullptr) {
      return GetSiblingFromParent(parent);
    }
    if (IsLeftChild(us)) {
      return parent->right_;
    } else {
      return parent->left_;
    }
  }

  bool SiblingIsRed(node_type* us, node_type* parent) {
    node_type* sibling{GetSibling(us, parent)};
    return sibling && sibling->color_ == NodeColor::RED;
  }

  bool SiblingIsBlack(node_type* us, node_type* parent) {
    return !SiblingIsRed(us, parent);
  }

  bool UncleIsRed(node_type* us) {
    node_type* uncle{GetUncle(us)};
    if (uncle == nullptr) {
      return false;
    }
    return uncle->color_ == NodeColor::RED;
  }

  bool IsInnerChild(node_type* us) {
    node_type* father{us->parent_};
    return (IsLeftChild(us) && IsRightChild(father)) ||
           (IsRightChild(us) && IsLeftChild(father));
  }

  bool IsLeftChild(node_type* us) {
    return us->parent_ && (us == us->parent_->left_);
  }
  bool IsRightChild(node_type* us) { return !IsLeftChild(us); };

  bool IsRoot(node_type* us) {
    if (us == nullptr) {
      return false;
    }
    return us->parent_ == nullptr;
  }

  bool GrandFatherExists(node_type* us) {
    return us && us->parent_ && us->parent_->parent_;
  }

  node_type* GetUncle(node_type* us) {
    node_type* father{us->parent_};
    if (IsLeftChild(father)) {
      return father->parent_->right_;
    } else {
      return father->parent_->left_;
    }
  }

  void RecolorFatherUncleGrandpa(node_type* us) {
    if (GrandFatherExists(us) == false) {
      if (us->parent_) {
        us->parent_->color_ = NodeColor::BLACK;
      }
      return;
    }
    node_type* uncle{GetUncle(us)};
    node_type* father{us->parent_};
    node_type* grandfather{father->parent_};

    father->color_ = NodeColor::BLACK;
    if (uncle) {
      uncle->color_ = NodeColor::BLACK;
    }
    grandfather->color_ = NodeColor::RED;
  }

  void DeleteSubtree(node_type* n) {
    if (n) {
      DeleteSubtree(n->left_);
      DeleteSubtree(n->right_);
      delete n;
    }
  }

  node_type* root_;
  size_type size_;
};

template <typename Key, typename T = void>
class RedBlackTreeIteratorBase {
 protected:
  using node_type = Node<Key, T>;
  node_type* current_{nullptr};

  RedBlackTreeIteratorBase() = default;
  explicit RedBlackTreeIteratorBase(node_type* node) : current_(node) {}

  // Common traversal logic
  void increment() {
    if (current_->right_) {
      current_ = current_->right_;
      while (current_->left_) {
        current_ = current_->left_;
      }
    } else {
      node_type* parent = current_->parent_;
      while (parent && current_ == parent->right_) {
        current_ = parent;
        parent = parent->parent_;
      }
      current_ = parent;
    }
  }

  void decrement() {
    // Similar logic for decrement
  }

 public:
  // Common operators
  bool operator==(const RedBlackTreeIteratorBase& other) const {
    return current_ == other.current_;
  }
  bool operator!=(const RedBlackTreeIteratorBase& other) const {
    return !(*this == other);
  }
};

// Map version
template <typename Key, typename T>
class RedBlackTreeIterator : public RedBlackTreeIteratorBase<Key, T> {
 public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = std::pair<const Key, T>;
  using difference_type = std::ptrdiff_t;
  using pointer = value_type*;
  using reference = value_type&;

  using Base = RedBlackTreeIteratorBase<Key, T>;
  using Base::current_;

  // Constructors
  RedBlackTreeIterator() : Base() {}
  explicit RedBlackTreeIterator(typename Base::node_type* node) : Base(node) {}

  // Specialized dereferencing
  reference operator*() const { return current_->data_; }
  pointer operator->() const { return &(current_->data_); }

  // Increment/decrement using base class implementation
  RedBlackTreeIterator& operator++() {
    this->increment();
    return *this;
  }
  RedBlackTreeIterator& operator--() {
    this->decrement();
    return *this;
  }
};

// Set version
template <typename Key>
class RedBlackTreeIterator<Key, void>
    : public RedBlackTreeIteratorBase<Key, void> {
 public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = Key;
  using difference_type = std::ptrdiff_t;
  using pointer = const Key*;
  using reference = const Key&;

  using Base = RedBlackTreeIteratorBase<Key, void>;
  using Base::current_;

  RedBlackTreeIterator() : Base() {}
  explicit RedBlackTreeIterator(typename Base::node_type* node) : Base(node) {}

  // Specialized dereferencing
  reference operator*() const { return current_->data_; }
  pointer operator->() const { return &(current_->data_); }

  RedBlackTreeIterator& operator++() {
    this->increment();
    return *this;
  }
  RedBlackTreeIterator& operator--() {
    this->decrement();
    return *this;
  }
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
                                               : print_color::BLACK);

  // Handle both Set and Map cases for printing the key
  if constexpr (std::is_same_v<T, void>) {
    std::cout << root->data_;  // Set case: data_ is the key
  } else {
    std::cout << root->data_.first;  // Map case: data_.first is the key
  }

  std::cout << "(" << (root->color_ == NodeColor::RED ? "R" : "B") << ")"
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
