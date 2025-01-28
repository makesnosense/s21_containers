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

TEST(RedBlackTreeTest, Erase) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({50, 999});
  tree.insert({25, 999});
  tree.insert({75, 999});
  tree.insert({10, 999});
  tree.insert({30, 999});
  tree.insert({5, 999});
  tree.insert({15, 999});

  // s21::print_tree(tree);

  tree.erase(10);

  // s21::print_tree(tree);
}

TEST(RedBlackTreeTest, Erase2) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({50, 999});
  tree.insert({25, 999});
  tree.insert({75, 999});

  tree.insert({80, 999});
  tree.insert({78, 999});
  tree.insert({10, 999});
  tree.insert({30, 999});
  tree.insert({5, 999});
  tree.insert({15, 999});
  tree.insert({76, 999});
  // s21::print_tree(tree);

  tree.erase(50);

  // s21::print_tree(tree);
}

TEST(RedBlackTreeEraseNoFixup, RemoveRedLeaf) {
  s21::RedBlackTree<int, int> tree;
  // Create a small tree where we can remove a red leaf
  tree.insert({20, 1});  // Black root
  tree.insert({10, 1});  // Red leaf to remove

  tree.erase(10);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeEraseNoFixup, RemoveRedMiddleNode) {
  s21::RedBlackTree<int, int> tree;
  // Create structure where middle node is red with one red child
  tree.insert({20, 1});  // Black root
  tree.insert({10, 1});  // Red middle
  tree.insert({5, 1});   // Red leaf

  tree.erase(10);  // Remove red middle node
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeEraseNoFixup, RemoveFromEmpty) {
  s21::RedBlackTree<int, int> tree;
  tree.erase(10);  // Should handle gracefully
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeEraseNoFixup, RemoveNonexistentKey) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({20, 1});
  tree.insert({10, 1});

  tree.erase(30);  // Key doesn't exist
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeEraseNoFixup, RemoveRedWithRedChild) {
  s21::RedBlackTree<int, int> tree;
  // Structure where removing a red node would replace it with its red child
  tree.insert({20, 1});  // Black root
  tree.insert({30, 1});  // Red right
  tree.insert({40, 1});  // Red right's right

  tree.erase(30);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeEraseNoFixup, RemoveRedWithLeftChild) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({20, 1});  // Black root
  tree.insert({30, 1});  // Red right
  tree.insert({25, 1});  // Red right's left

  tree.erase(30);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeEraseNoFixup, RemoveOnlyRoot) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({20, 1});  // Single black root

  tree.erase(20);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
  EXPECT_EQ(tree.get_root(), nullptr);
}

TEST(RedBlackTreeEraseNoFixup, RemoveRedNodeBothRedChildren) {
  s21::RedBlackTree<int, int> tree;
  // Create a structure where a red node has two red children
  tree.insert({20, 1});  // Black root
  tree.insert({10, 1});  // Left red
  tree.insert({30, 1});  // Right red
  tree.insert({5, 1});   // Red child to remove

  tree.erase(5);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeEraseNoFixup, ConsecutiveRemoveRed) {
  s21::RedBlackTree<int, int> tree;
  // Insert several nodes and remove red ones consecutively
  tree.insert({20, 1});
  tree.insert({10, 1});
  tree.insert({30, 1});
  tree.insert({5, 1});
  tree.insert({15, 1});

  tree.erase(5);  // Remove red leaf
  EXPECT_TRUE(ValidateRedBlackTree(tree));
  tree.erase(15);  // Remove another red leaf
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeTest, EraseDoubleBlackWithRedSibling) {
  s21::RedBlackTree<int, int> tree;

  // Create this structure:
  //       20B
  //      /  \
  //    10B   30R
  //         /  \
  //       25B   35B
  //      /
  //    23R

  tree.insert({20, 999});  // Root will be black
  tree.insert({10, 999});
  tree.insert({30, 999});  // Will be red
  tree.insert({25, 999});
  tree.insert({35, 999});
  tree.insert({23, 999});
  // EXPECT_TRUE(ValidateRedBlackTree(tree));
  // s21::print_tree(tree);  // Verify initial structure

  // When we delete 10, it creates a double-black at its position
  // and its sibling (30) is red, triggering Case 1 of fixup
  tree.erase(10);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
  // s21::print_tree(tree);  // See the result
}

// TEST(RedBlackTreeTest, EraseDoubleBlackWithBlackSiblingBlackChildren) {
//   s21::RedBlackTree<int, int> tree;

//   tree.insert({20, 999});
//   tree.insert({10, 999});
//   tree.insert({30, 999});
//   tree.insert({5, 999});
//   tree.insert({40, 999});
//   s21::print_tree(tree);
//   tree.erase(5);
//   tree.erase(10);
//   // When we delete 10, it creates a double-black at its position
//   // and its sibling (30) is black with two black children
//   // tree.erase(10);

//   // Print result

//   EXPECT_TRUE(ValidateRedBlackTree(tree));
// }

#if 0


TEST(RedBlackTreeTest, EraseLeafNode) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({10, 999});
  tree.insert({5, 999});
  tree.insert({15, 999});

  tree.erase(5);  // Remove leaf node
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeTest, EraseNodeWithOneChild) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({10, 999});
  tree.insert({5, 999});
  tree.insert({15, 999});
  tree.insert({3, 999});

  print_tree(tree);
  tree.erase(5);  // Node with one child
  print_tree(tree);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeTest, EraseNodeWithTwoChildren) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({10, 999});
  tree.insert({5, 999});
  tree.insert({15, 999});
  tree.insert({3, 999});
  tree.insert({7, 999});

  tree.erase(5);  // Node with two children
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeTest, EraseRoot) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({10, 999});
  tree.insert({5, 999});
  tree.insert({15, 999});

  tree.erase(10);  // Remove root
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeTest, EraseNonExistentKey) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({10, 999});
  tree.insert({5, 999});

  tree.erase(15);  // Try to remove non-existent key
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeTest, EraseFromEmptyTree) {
  s21::RedBlackTree<int, int> tree;
  tree.erase(10);  // Try to remove from empty tree
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

// TEST(RedBlackTreeTest, EraseAllNodesSequentially) {
//   s21::RedBlackTree<int, int> tree;
//   std::vector<int> keys = {10, 5, 15, 3, 7, 13, 17};

//   // Insert all keys
//   for (int key : keys) {
//     tree.insert({key, 999});
//   }

//   // Remove all keys one by one
//   for (int key : keys) {
//     tree.erase(key);
//     EXPECT_TRUE(ValidateRedBlackTree(tree));
//   }
// }

TEST(RedBlackTreeTest, EraseRedNode) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({10, 999});
  tree.insert({5, 999});
  tree.insert({15, 999});
  tree.insert({3, 999});

  tree.erase(3);  // Remove a red node
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeTest, EraseBlackNodeWithRedChild) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({10, 999});
  tree.insert({5, 999});
  tree.insert({15, 999});
  tree.insert({3, 999});
  tree.insert({7, 999});

  tree.erase(5);  // Remove black node with red child
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeTest, ComplexEraseScenario) {
  s21::RedBlackTree<int, int> tree;
  // Create a more complex tree
  std::vector<int> keys = {50, 25, 75, 10, 30, 60, 80, 5,
                           15, 27, 35, 55, 65, 77, 85};

  for (int key : keys) {
    tree.insert({key, 999});
  }

  // Test removing nodes that will require complex rebalancing
  tree.erase(25);  // Remove internal node
  EXPECT_TRUE(ValidateRedBlackTree(tree));

  tree.erase(75);  // Remove another internal node
  EXPECT_TRUE(ValidateRedBlackTree(tree));

  tree.erase(50);  // Remove root
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeTest, RepeatedEraseInsert) {
  s21::RedBlackTree<int, int> tree;

  // Insert some initial nodes
  for (int i = 1; i <= 5; i++) {
    tree.insert({i * 10, 999});
  }

  // Repeatedly erase and insert
  for (int i = 0; i < 3; i++) {
    tree.erase(20);
    EXPECT_TRUE(ValidateRedBlackTree(tree));

    tree.insert({20, 999});
    EXPECT_TRUE(ValidateRedBlackTree(tree));
  }
}

#endif
