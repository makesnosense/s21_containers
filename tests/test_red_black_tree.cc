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

TEST(RedBlackTreeTest, BlackUncleOuterChild) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({10, 999});
  tree.insert({5, 999});
  tree.insert({2, 999});

  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeTest, BlackUncleOuterChild2) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({10, 999});
  tree.insert({20, 999});
  tree.insert({30, 999});
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeTest, BlackUncleInnerChild) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({10, 999});

  tree.insert({15, 999});
  tree.insert({14, 999});
  tree.insert({20, 999});
  tree.insert({2, 999});

  tree.insert({1, 999});

  // tree.insert({5, 999});

  EXPECT_TRUE(ValidateRedBlackTree(tree));
}
TEST(RedBlackTreeTest, EmptyAndSingleNode) {
  s21::RedBlackTree<int, int> tree;
  EXPECT_TRUE(ValidateRedBlackTree(tree));  // Empty tree

  tree.insert({1, 999});
  EXPECT_TRUE(ValidateRedBlackTree(tree));  // Single black root
}

TEST(RedBlackTreeTest, SimpleLeftRotation) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({10, 999});
  tree.insert({15, 999});
  tree.insert({20, 999});  // Should trigger left rotation
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeTest, SimpleRightRotation) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({20, 999});
  tree.insert({15, 999});
  tree.insert({10, 999});  // Should trigger right rotation
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeTest, LeftRightRotation) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({20, 999});
  tree.insert({10, 999});
  tree.insert({15, 999});  // Should trigger left-right rotation
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeTest, RightLeftRotation) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({10, 999});
  tree.insert({20, 999});
  tree.insert({15, 999});  // Should trigger right-left rotation
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeTest, RecoloringCase) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({10, 999});
  tree.insert({5, 999});
  tree.insert({15, 999});
  tree.insert({3, 999});  // Should trigger recoloring
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeTest, ComplexLeftSubtree) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({50, 999});
  tree.insert({25, 999});
  tree.insert({75, 999});
  tree.insert({10, 999});
  tree.insert({30, 999});
  tree.insert({5, 999});
  tree.insert({15, 999});
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeTest, ComplexRightSubtree) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({50, 999});
  tree.insert({25, 999});
  tree.insert({75, 999});
  tree.insert({60, 999});
  tree.insert({90, 999});
  tree.insert({95, 999});
  tree.insert({85, 999});
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeTest, AlternatingInsertions) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({50, 999});
  tree.insert({25, 999});
  tree.insert({75, 999});
  tree.insert({12, 999});
  tree.insert({37, 999});
  tree.insert({62, 999});
  tree.insert({87, 999});
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeTest, AscendingInsertions) {
  s21::RedBlackTree<int, int> tree;
  for (int i = 1; i <= 10; i++) {
    tree.insert({i, 999});
    print_tree(tree);
    if (ValidateRedBlackTree(tree) == false) {
      EXPECT_TRUE(ValidateRedBlackTree(tree));
      return;
    }
  }
}

TEST(RedBlackTreeTest, DescendingInsertions) {
  s21::RedBlackTree<int, int> tree;
  for (int i = 10; i >= 1; i--) {
    tree.insert({i, 999});

    EXPECT_TRUE(ValidateRedBlackTree(tree));
  }
}
