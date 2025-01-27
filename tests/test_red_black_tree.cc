#include "s21_red_black_tree.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#include <gtest/gtest.h>
#pragma GCC diagnostic pop

template <typename Key, typename T>
bool ValidateProperties(const s21::Node<Key, T>* node, int& black_height);

template <typename Key, typename T>
bool ValidateRedBlackTree(const s21::RedBlackTree<Key, T>& tree) {
  const s21::Node<Key, T>* root{tree.get_root()};

  if (!root) return true;  // Empty tree is valid

  // Root must be black
  if (root->color_ != s21::NodeColor::BLACK) {
    std::cerr << "Root must be black";
    return false;
  }

  // Check black height and red property for all paths
  int black_height = 0;
  return ValidateProperties(root, black_height);
}

template <typename Key, typename T>
bool ValidateProperties(const s21::Node<Key, T>* node, int& black_height) {
  if (!node) {
    black_height = 0;  // Null nodes are black, but we don't count them
    return true;
  }

  int left_black_height = 0;
  int right_black_height = 0;

  // Recursively validate left and right subtrees
  if (ValidateProperties(node->left_, left_black_height) == false ||
      ValidateProperties(node->right_, right_black_height) == false) {
    return false;
  }

  // Property 4: All paths must have same number of black nodes
  if (left_black_height != right_black_height) {
    std::cerr << "Property 4: all paths must have same number of black nodes";
    return false;
  }

  // Property 3: Red nodes cannot have red children
  if (node->color_ == s21::NodeColor::RED) {
    if ((node->left_ && node->left_->color_ == s21::NodeColor::RED) ||
        (node->right_ && node->right_->color_ == s21::NodeColor::RED)) {
      std::cerr << "Property 3: red nodes cannot have red children";
      return false;
    }
  }

  // Calculate black height of current path
  black_height = left_black_height;
  if (node->color_ == s21::NodeColor::BLACK) {
    black_height++;
  }

  // Validate parent pointers
  if (node->left_ && node->left_->parent_ != node) {
    std::cerr << "Parent pointers error.";
    return false;
  }
  if (node->right_ && node->right_->parent_ != node) {
    std::cerr << "Parent pointers error.";
    return false;
  }

  return true;
}

TEST(RedBlackTreeTest, Meow) {
  s21::RedBlackTree<int, int> meow;
  meow.insert({10, 999});
  meow.insert({5, 999});
  print_tree(meow);
  meow.insert({2, 999});

  print_tree(meow);
  // meow.insert({5, 999});

  // meow.insert({2, 999});
  // meow.insert({10, 999});

  // meow.insert({8, 999});
  // print_tree(meow);
  // meow.insert({12, 999});
  // print_tree(meow);
  // meow.insert({6, 999});
  // meow.insert({9, 999});

  // meow.RotateLeft(meow.FindNode(10));
  // print_tree(meow);
  // meow.RotateLeft(meow.FindNode(5));
  // print_tree(meow);

  // int height = 0;
  // bool is_valid = ValidateBlackHeight(meow.get_root(), height);
  EXPECT_TRUE(ValidateRedBlackTree(meow));
  // meow.RotateRight(meow.get_root());
  // print_tree(meow);
}
