#include "s21_red_black_tree.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#include <gtest/gtest.h>
#pragma GCC diagnostic pop

#include <algorithm>
#include <vector>

#include "random.h"

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
bool ValidateBSTProperty(const s21::Node<Key, T>* node, const Key* min_key,
                         const Key* max_key) {
  if (!node) return true;

  // Check current node's key against bounds
  if (min_key && node->GetKey() < *min_key) {
    std::cerr << "BST property violation: node key less than minimum bound";
    return false;
  }
  if (max_key && node->GetKey() > *max_key) {
    std::cerr << "BST property violation: node key greater than maximum bound";
    return false;
  }

  // Recursively validate left and right subtrees
  // For left: all keys must be <= current key
  // For right: all keys must be >= current key
  return ValidateBSTProperty(node->left_, min_key, &node->GetKey()) &&
         ValidateBSTProperty(node->right_, &node->GetKey(), max_key);
}

template <typename Key, typename T>
bool ValidateProperties(const s21::Node<Key, T>* node, int& black_height) {
  if (!node) {
    black_height = 0;  // Null nodes are black, but we don't count them
    return true;
  }

  // Add BST property validation
  if (!ValidateBSTProperty<Key, T>(node, nullptr, nullptr)) {
    return false;
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

TEST(RedBlackTreeMapTest, BlackUncleOuterChild) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({10, 999});
  tree.insert({5, 999});
  tree.insert({2, 999});

  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, BlackUncleOuterChild2) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({10, 999});
  tree.insert({20, 999});
  tree.insert({30, 999});
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, BlackUncleInnerChild) {
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
TEST(RedBlackTreeMapTest, EmptyAndSingleNode) {
  s21::RedBlackTree<int, int> tree;
  EXPECT_TRUE(ValidateRedBlackTree(tree));  // Empty tree

  tree.insert({1, 999});
  EXPECT_TRUE(ValidateRedBlackTree(tree));  // Single black root
}

TEST(RedBlackTreeMapTest, SimpleLeftRotation) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({10, 999});
  tree.insert({15, 999});
  tree.insert({20, 999});  // Should trigger left rotation
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, SimpleRightRotation) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({20, 999});
  tree.insert({15, 999});
  tree.insert({10, 999});  // Should trigger right rotation
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, LeftRightRotation) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({20, 999});
  tree.insert({10, 999});
  tree.insert({15, 999});  // Should trigger left-right rotation
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, RightLeftRotation) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({10, 999});
  tree.insert({20, 999});
  tree.insert({15, 999});  // Should trigger right-left rotation
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, RecoloringCase) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({10, 999});
  tree.insert({5, 999});
  tree.insert({15, 999});
  tree.insert({3, 999});  // Should trigger recoloring
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, ComplexLeftSubtree) {
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

TEST(RedBlackTreeMapTest, ComplexRightSubtree) {
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

TEST(RedBlackTreeMapTest, AlternatingInsertions) {
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

TEST(RedBlackTreeMapTest, AscendingInsertions) {
  s21::RedBlackTree<int, int> tree;
  for (int i = 1; i <= 10; i++) {
    tree.insert({i, 999});
    if (ValidateRedBlackTree(tree) == false) {
      EXPECT_TRUE(ValidateRedBlackTree(tree));
      return;
    }
  }
}

TEST(RedBlackTreeMapTest, DescendingInsertions) {
  s21::RedBlackTree<int, int> tree;
  for (int i = 10; i >= 1; i--) {
    tree.insert({i, 999});
    EXPECT_TRUE(ValidateRedBlackTree(tree));
  }
}

TEST(RedBlackTreeMapTest, Erase) {
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

TEST(RedBlackTreeMapTest, Erase2) {
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

  size_t erased_elements{tree.erase(30)};
  EXPECT_TRUE(ValidateRedBlackTree(tree));
  EXPECT_EQ(erased_elements, size_t{1});
}

TEST(RedBlackTreeEraseNoFixup, RemoveRedWithLeftChild) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({20, 1});  // Black root
  tree.insert({30, 1});  // Red right
  tree.insert({25, 1});  // Red right's left

  size_t erased_elements{tree.erase(30)};
  erased_elements += tree.erase(33);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
  EXPECT_EQ(erased_elements, size_t{1});
}

TEST(RedBlackTreeEraseNoFixup, RemoveOnlyRoot) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({20, 1});  // Single black root

  size_t erased_elements{tree.erase(20)};
  EXPECT_TRUE(ValidateRedBlackTree(tree));
  EXPECT_EQ(tree.get_root(), nullptr);
  EXPECT_EQ(erased_elements, size_t{1});
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

TEST(RedBlackTreeMapTest, EraseDoubleBlackWithRedSibling) {
  s21::RedBlackTree<int, int> tree;

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

TEST(RedBlackTreeMapTest, EraseDoubleBlackWithBlackSiblingBlackChildren) {
  s21::RedBlackTree<int, int> tree;

  tree.insert({20, 999});
  tree.insert({10, 999});
  tree.insert({30, 999});
  tree.insert({5, 999});
  tree.insert({40, 999});

  // s21::print_tree(tree);

  tree.erase(5);
  tree.erase(40);

  // s21::print_tree(tree);

  tree.erase(10);

  // Print result
  // s21::print_tree(tree);

  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, EraseLeafNode) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({10, 999});
  tree.insert({5, 999});
  tree.insert({15, 999});

  tree.erase(5);  // Remove leaf node
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, EraseNodeWithOneChild) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({10, 999});
  tree.insert({5, 999});
  tree.insert({15, 999});
  tree.insert({3, 999});

  // print_tree(tree);
  tree.erase(5);  // Node with one child
  // print_tree(tree);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, EraseNodeWithTwoChildren) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({10, 999});
  tree.insert({5, 999});
  tree.insert({15, 999});
  tree.insert({3, 999});
  tree.insert({7, 999});

  tree.erase(5);  // Node with two children
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, EraseRoot) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({10, 999});
  tree.insert({5, 999});
  tree.insert({15, 999});

  tree.erase(10);  // Remove root
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, EraseNonExistentKey) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({10, 999});
  tree.insert({5, 999});

  tree.erase(15);  // Try to remove non-existent key
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, EraseFromEmptyTree) {
  s21::RedBlackTree<int, int> tree;
  tree.erase(10);  // Try to remove from empty tree
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, EraseAllNodesSequentially) {
  s21::RedBlackTree<int, int> tree;
  std::vector<int> keys = {10, 5, 15, 3, 7, 13, 17};

  // Insert all keys
  for (int key : keys) {
    tree.insert({key, 999});
  }

  // Remove all keys one by one
  for (int key : keys) {
    tree.erase(key);

    EXPECT_TRUE(ValidateRedBlackTree(tree));
  }
}  // seg fault

TEST(RedBlackTreeMapTest, EraseRedNode) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({10, 999});
  tree.insert({5, 999});
  tree.insert({15, 999});
  tree.insert({3, 999});

  tree.erase(3);  // Remove a red node
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, EraseBlackNodeWithRedChild) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({10, 999});
  tree.insert({5, 999});
  tree.insert({15, 999});
  tree.insert({3, 999});
  tree.insert({7, 999});

  tree.erase(5);  // Remove black node with red child
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, ComplexEraseScenario) {
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

TEST(RedBlackTreeMapTest, RepeatedEraseInsert) {
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

TEST(RedBlackTreeMapTest, DeleteLastElement) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({10, 999});

  tree.erase(10);                           // Remove last/root element
  EXPECT_TRUE(ValidateRedBlackTree(tree));  // Empty tree should be valid
  EXPECT_TRUE(tree.get_root() == nullptr);
}

TEST(RedBlackTreeMapTest, EraseSequenceOfNodes) {
  s21::RedBlackTree<int, int> tree;

  // Build a complex tree
  std::vector<int> values = {50, 25, 75, 10, 30, 60, 80, 5, 15, 27, 35};
  for (int val : values) {
    tree.insert({val, 999});
  }

  // Remove in different orders and check tree validity
  tree.erase(25);  // Internal node with two children
  EXPECT_TRUE(ValidateRedBlackTree(tree));

  tree.erase(5);  // Leaf node
  EXPECT_TRUE(ValidateRedBlackTree(tree));

  tree.erase(75);  // Internal node near root
  EXPECT_TRUE(ValidateRedBlackTree(tree));

  tree.erase(50);  // Root node
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, EraseAllNodesInDifferentOrders) {
  s21::RedBlackTree<int, int> tree;
  std::vector<int> values = {20, 10, 30, 5, 15, 25, 35};

  // Test 1: Insert all, then remove in ascending order
  for (int val : values) tree.insert({val, 999});
  std::sort(values.begin(), values.end());
  for (int val : values) {
    tree.erase(val);
    EXPECT_TRUE(ValidateRedBlackTree(tree));
  }

  // Test 2: Insert all, then remove in descending order
  for (int val : values) tree.insert({val, 999});
  std::sort(values.begin(), values.end(), std::greater<int>());
  for (int val : values) {
    tree.erase(val);
    EXPECT_TRUE(ValidateRedBlackTree(tree));
  }
}

TEST(RedBlackTreeMapTest, EraseBlackNodeCases) {
  s21::RedBlackTree<int, int> tree;

  // Create a tree where removing nodes will require different fixup cases
  tree.insert({20, 999});
  tree.insert({10, 999});
  tree.insert({30, 999});
  tree.insert({5, 999});
  tree.insert({15, 999});
  tree.insert({25, 999});
  tree.insert({35, 999});

  tree.erase(5);  // Should trigger black node removal
  EXPECT_TRUE(ValidateRedBlackTree(tree));

  tree.erase(35);  // Another black node removal
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, EraseWithRepeatedOperations) {
  s21::RedBlackTree<int, int> tree;

  // Insert-remove-insert same key multiple times
  tree.insert({10, 999});
  EXPECT_TRUE(ValidateRedBlackTree(tree));

  tree.erase(10);
  EXPECT_TRUE(ValidateRedBlackTree(tree));

  tree.insert({10, 999});
  EXPECT_TRUE(ValidateRedBlackTree(tree));

  tree.erase(10);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, EraseNodeWithSingleChild) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({20, 999});
  tree.insert({10, 999});
  tree.insert({30, 999});
  tree.insert({5, 999});

  tree.erase(10);  // Node with single child
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, EraseLastRemainingNode) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({10, 999});

  tree.erase(10);  // Remove last node
  EXPECT_TRUE(ValidateRedBlackTree(tree));
  EXPECT_TRUE(tree.get_root() == nullptr);
}

TEST(RedBlackTreeMapTest, EraseRootWithChildren) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({20, 999});
  tree.insert({10, 999});
  tree.insert({30, 999});

  tree.erase(20);  // Remove root with two children
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, RemovalFixupCases) {
  s21::RedBlackTree<int, int> tree;

  // Case 1: Double black's sibling is red
  tree.insert({20, 999});
  tree.insert({10, 999});
  tree.insert({30, 999});
  tree.insert({25, 999});
  tree.insert({35, 999});
  tree.erase(10);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, StressTestRemoval) {
  s21::RedBlackTree<int, int> tree;
  const int SIZE = 1000;

  // Insert elements
  for (int i = 0; i < SIZE; i++) {
    tree.insert({i, 999});
    EXPECT_TRUE(ValidateRedBlackTree(tree));
  }

  // Remove randomly
  std::vector<int> indices(SIZE);
  std::iota(indices.begin(), indices.end(), 0);

  std::shuffle(indices.begin(), indices.end(), Random::mt);

  for (int i : indices) {
    tree.erase(i);
    EXPECT_TRUE(ValidateRedBlackTree(tree));
  }
}

TEST(RedBlackTreeSetTest, BlackUncleOuterChild) {
  s21::RedBlackTree<int> tree;
  tree.insert(10);
  tree.insert(5);
  tree.insert(2);

  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, BlackUncleOuterChild2) {
  s21::RedBlackTree<int> tree;
  tree.insert(10);
  tree.insert(20);
  tree.insert(30);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, BlackUncleInnerChild) {
  s21::RedBlackTree<int> tree;
  tree.insert(10);
  tree.insert(15);
  tree.insert(14);
  tree.insert(20);
  tree.insert(2);
  tree.insert(1);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, EmptyAndSingleNode) {
  s21::RedBlackTree<int> tree;
  EXPECT_TRUE(ValidateRedBlackTree(tree));  // Empty tree

  tree.insert(1);
  EXPECT_TRUE(ValidateRedBlackTree(tree));  // Single black root
}

TEST(RedBlackTreeSetTest, SimpleLeftRotation) {
  s21::RedBlackTree<int> tree;
  tree.insert(10);
  tree.insert(15);
  tree.insert(20);  // Should trigger left rotation
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, SimpleRightRotation) {
  s21::RedBlackTree<int> tree;
  tree.insert(20);
  tree.insert(15);
  tree.insert(10);  // Should trigger right rotation
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, LeftRightRotation) {
  s21::RedBlackTree<int> tree;
  tree.insert(20);
  tree.insert(10);
  tree.insert(15);  // Should trigger left-right rotation
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, RightLeftRotation) {
  s21::RedBlackTree<int> tree;
  tree.insert(10);
  tree.insert(20);
  tree.insert(15);  // Should trigger right-left rotation
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, RecoloringCase) {
  s21::RedBlackTree<int> tree;
  tree.insert(10);
  tree.insert(5);
  tree.insert(15);
  tree.insert(3);  // Should trigger recoloring
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, ComplexLeftSubtree) {
  s21::RedBlackTree<int> tree;
  tree.insert(50);
  tree.insert(25);
  tree.insert(75);
  tree.insert(10);
  tree.insert(30);
  tree.insert(5);
  tree.insert(15);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, ComplexRightSubtree) {
  s21::RedBlackTree<int> tree;
  tree.insert(50);
  tree.insert(25);
  tree.insert(75);
  tree.insert(60);
  tree.insert(90);
  tree.insert(95);
  tree.insert(85);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, AlternatingInsertions) {
  s21::RedBlackTree<int> tree;
  tree.insert(50);
  tree.insert(25);
  tree.insert(75);
  tree.insert(12);
  tree.insert(37);
  tree.insert(62);
  tree.insert(87);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, AscendingInsertions) {
  s21::RedBlackTree<int> tree;
  for (int i = 1; i <= 10; i++) {
    tree.insert(i);
    if (ValidateRedBlackTree(tree) == false) {
      EXPECT_TRUE(ValidateRedBlackTree(tree));
      return;
    }
  }
}

TEST(RedBlackTreeSetTest, DescendingInsertions) {
  s21::RedBlackTree<int> tree;
  for (int i = 10; i >= 1; i--) {
    tree.insert(i);
    EXPECT_TRUE(ValidateRedBlackTree(tree));
  }
}

TEST(RedBlackTreeSetTest, Erase) {
  s21::RedBlackTree<int> tree;
  tree.insert(50);
  tree.insert(25);
  tree.insert(75);
  tree.insert(10);
  tree.insert(30);
  tree.insert(5);
  tree.insert(15);

  tree.erase(10);
}

TEST(RedBlackTreeSetTest, Erase2) {
  s21::RedBlackTree<int> tree;
  tree.insert(50);
  tree.insert(25);
  tree.insert(75);
  tree.insert(80);
  tree.insert(78);
  tree.insert(10);
  tree.insert(30);
  tree.insert(5);
  tree.insert(15);
  tree.insert(76);

  tree.erase(50);
}

TEST(RedBlackTreeSetTest, RemoveRedLeaf) {
  s21::RedBlackTree<int> tree;
  tree.insert(20);  // Black root
  tree.insert(10);  // Red leaf to remove

  tree.erase(10);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, RemoveRedMiddleNode) {
  s21::RedBlackTree<int> tree;
  tree.insert(20);  // Black root
  tree.insert(10);  // Red middle
  tree.insert(5);   // Red leaf

  tree.erase(10);  // Remove red middle node
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, RemoveFromEmpty) {
  s21::RedBlackTree<int> tree;
  tree.erase(10);  // Should handle gracefully
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, RemoveNonexistentKey) {
  s21::RedBlackTree<int> tree;
  tree.insert(20);
  tree.insert(10);

  tree.erase(30);  // Key doesn't exist
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, RemoveRedWithRedChild) {
  s21::RedBlackTree<int> tree;
  tree.insert(20);  // Black root
  tree.insert(30);  // Red right
  tree.insert(40);  // Red right's right

  tree.erase(30);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, RemoveRedWithLeftChild) {
  s21::RedBlackTree<int> tree;
  tree.insert(20);  // Black root
  tree.insert(30);  // Red right
  tree.insert(25);  // Red right's left

  tree.erase(30);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, RemoveOnlyRoot) {
  s21::RedBlackTree<int> tree;
  tree.insert(20);  // Single black root

  tree.erase(20);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
  EXPECT_EQ(tree.get_root(), nullptr);
}

TEST(RedBlackTreeSetTest, RemoveRedNodeBothRedChildren) {
  s21::RedBlackTree<int> tree;
  tree.insert(20);  // Black root
  tree.insert(10);  // Left red
  tree.insert(30);  // Right red
  tree.insert(5);   // Red child to remove

  tree.erase(5);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, ConsecutiveRemoveRed) {
  s21::RedBlackTree<int> tree;
  tree.insert(20);
  tree.insert(10);
  tree.insert(30);
  tree.insert(5);
  tree.insert(15);

  tree.erase(5);  // Remove red leaf
  EXPECT_TRUE(ValidateRedBlackTree(tree));
  tree.erase(15);  // Remove another red leaf
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, EraseDoubleBlackWithRedSibling) {
  s21::RedBlackTree<int> tree;

  tree.insert(20);  // Root will be black
  tree.insert(10);
  tree.insert(30);  // Will be red
  tree.insert(25);
  tree.insert(35);
  tree.insert(23);

  tree.erase(10);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, EraseDoubleBlackWithBlackSiblingBlackChildren) {
  s21::RedBlackTree<int> tree;

  tree.insert(20);
  tree.insert(10);
  tree.insert(30);
  tree.insert(5);
  tree.insert(40);

  tree.erase(5);
  tree.erase(40);
  tree.erase(10);

  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, EraseLeafNode) {
  s21::RedBlackTree<int> tree;
  tree.insert(10);
  tree.insert(5);
  tree.insert(15);

  tree.erase(5);  // Remove leaf node
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, EraseNodeWithOneChild) {
  s21::RedBlackTree<int> tree;
  tree.insert(10);
  tree.insert(5);
  tree.insert(15);
  tree.insert(3);

  tree.erase(5);  // Node with one child
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, EraseNodeWithTwoChildren) {
  s21::RedBlackTree<int> tree;
  tree.insert(10);
  tree.insert(5);
  tree.insert(15);
  tree.insert(3);
  tree.insert(7);

  tree.erase(5);  // Node with two children
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, EraseRoot) {
  s21::RedBlackTree<int> tree;
  tree.insert(10);
  tree.insert(5);
  tree.insert(15);

  tree.erase(10);  // Remove root
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, EraseNonExistentKey) {
  s21::RedBlackTree<int> tree;
  tree.insert(10);
  tree.insert(5);

  tree.erase(15);  // Try to remove non-existent key
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, EraseFromEmptyTree) {
  s21::RedBlackTree<int> tree;
  tree.erase(10);  // Try to remove from empty tree
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, EraseAllNodesSequentially) {
  s21::RedBlackTree<int> tree;
  std::vector<int> keys = {10, 5, 15, 3, 7, 13, 17};

  // Insert all keys
  for (int key : keys) {
    tree.insert(key);
  }

  // Remove all keys one by one
  for (int key : keys) {
    tree.erase(key);
    EXPECT_TRUE(ValidateRedBlackTree(tree));
  }
}

TEST(RedBlackTreeSetTest, EraseRedNode) {
  s21::RedBlackTree<int> tree;
  tree.insert(10);
  tree.insert(5);
  tree.insert(15);
  tree.insert(3);

  tree.erase(3);  // Remove a red node
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, EraseBlackNodeWithRedChild) {
  s21::RedBlackTree<int> tree;
  tree.insert(10);
  tree.insert(5);
  tree.insert(15);
  tree.insert(3);
  tree.insert(7);

  tree.erase(5);  // Remove black node with red child
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, ComplexEraseScenario) {
  s21::RedBlackTree<int> tree;
  std::vector<int> keys = {50, 25, 75, 10, 30, 60, 80, 5,
                           15, 27, 35, 55, 65, 77, 85};

  for (int key : keys) {
    tree.insert(key);
  }

  tree.erase(25);  // Remove internal node
  EXPECT_TRUE(ValidateRedBlackTree(tree));

  tree.erase(75);  // Remove another internal node
  EXPECT_TRUE(ValidateRedBlackTree(tree));

  tree.erase(50);  // Remove root
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, RepeatedEraseInsert) {
  s21::RedBlackTree<int> tree;

  // Insert some initial nodes
  for (int i = 1; i <= 5; i++) {
    tree.insert(i * 10);
  }

  // Repeatedly erase and insert
  for (int i = 0; i < 3; i++) {
    tree.erase(20);
    EXPECT_TRUE(ValidateRedBlackTree(tree));

    tree.insert(20);
    EXPECT_TRUE(ValidateRedBlackTree(tree));
  }
}

TEST(RedBlackTreeSetTest, DeleteLastElement) {
  s21::RedBlackTree<int> tree;
  tree.insert(10);

  tree.erase(10);                           // Remove last/root element
  EXPECT_TRUE(ValidateRedBlackTree(tree));  // Empty tree should be valid
  EXPECT_TRUE(tree.get_root() == nullptr);
}

TEST(RedBlackTreeSetTest, EraseSequenceOfNodes) {
  s21::RedBlackTree<int> tree;

  // Build a complex tree
  std::vector<int> values = {50, 25, 75, 10, 30, 60, 80, 5, 15, 27, 35};
  for (int val : values) {
    tree.insert(val);
  }

  tree.erase(25);  // Internal node with two children
  EXPECT_TRUE(ValidateRedBlackTree(tree));

  tree.erase(5);  // Leaf node
  EXPECT_TRUE(ValidateRedBlackTree(tree));

  tree.erase(75);  // Internal node near root
  EXPECT_TRUE(ValidateRedBlackTree(tree));

  tree.erase(50);  // Root node
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, EraseAllNodesInDifferentOrders) {
  s21::RedBlackTree<int> tree;
  std::vector<int> values = {20, 10, 30, 5, 15, 25, 35};

  // Test 1: Insert all, then remove in ascending order
  for (int val : values) tree.insert(val);
  std::sort(values.begin(), values.end());
  for (int val : values) {
    tree.erase(val);
    EXPECT_TRUE(ValidateRedBlackTree(tree));
  }

  // Test 2: Insert all, then remove in descending order
  for (int val : values) tree.insert(val);
  std::sort(values.begin(), values.end(), std::greater<int>());
  for (int val : values) {
    tree.erase(val);
    EXPECT_TRUE(ValidateRedBlackTree(tree));
  }
}

TEST(RedBlackTreeSetTest, EraseBlackNodeCases) {
  s21::RedBlackTree<int> tree;

  tree.insert(20);
  tree.insert(10);
  tree.insert(30);
  tree.insert(5);
  tree.insert(15);
  tree.insert(25);
  tree.insert(35);

  tree.erase(5);  // Should trigger black node removal
  EXPECT_TRUE(ValidateRedBlackTree(tree));

  tree.erase(35);  // Another black node removal
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, EraseWithRepeatedOperations) {
  s21::RedBlackTree<int> tree;

  tree.insert(10);
  EXPECT_TRUE(ValidateRedBlackTree(tree));

  tree.erase(10);
  EXPECT_TRUE(ValidateRedBlackTree(tree));

  tree.insert(10);
  EXPECT_TRUE(ValidateRedBlackTree(tree));

  tree.erase(10);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, EraseNodeWithSingleChild) {
  s21::RedBlackTree<int> tree;
  tree.insert(20);
  tree.insert(10);
  tree.insert(30);
  tree.insert(5);

  tree.erase(10);  // Node with single child
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, EraseLastRemainingNode) {
  s21::RedBlackTree<int> tree;
  tree.insert(10);

  tree.erase(10);  // Remove last node
  EXPECT_TRUE(ValidateRedBlackTree(tree));
  EXPECT_TRUE(tree.get_root() == nullptr);
}

TEST(RedBlackTreeSetTest, StressTestRemoval) {
  s21::RedBlackTree<int> tree;
  const int SIZE = 1000;

  // Insert elements
  for (int i = 0; i < SIZE; i++) {
    tree.insert(i);
    EXPECT_TRUE(ValidateRedBlackTree(tree));
  }

  // Remove randomly
  std::vector<int> indices(SIZE);
  std::iota(indices.begin(), indices.end(), 0);
  std::shuffle(indices.begin(), indices.end(), Random::mt);

  for (int i : indices) {
    tree.erase(i);
    EXPECT_TRUE(ValidateRedBlackTree(tree));
  }
}

TEST(RedBlackTreeSetTest, ComparisonOperatorsSet) {
  s21::RedBlackTree<int> tree;
  const int SIZE = 1000;

  // Insert elements
  for (int i = 0; i < SIZE; i++) {
    tree.insert(i);
  }

  s21::RedBlackTree<int>::iterator iter1{tree.begin()};
  s21::RedBlackTree<int>::iterator iter2{tree.begin()};

  *iter2++;
  *iter2++;

  EXPECT_FALSE(iter1 == iter2);
  EXPECT_TRUE(iter1 != iter2);
}

TEST(RedBlackTreeSetTest, ComparisonOperatorsSetConst) {
  s21::RedBlackTree<int> tree;
  const int SIZE = 1000;

  // Insert elements
  for (int i = 0; i < SIZE; i++) {
    tree.insert(i);
  }

  s21::RedBlackTree<int>::const_iterator iter1{tree.begin()};
  s21::RedBlackTree<int>::const_iterator iter2{tree.begin()};

  *iter2++;

  EXPECT_FALSE(iter1 == iter2);
  EXPECT_TRUE(iter1 != iter2);
}

TEST(RedBlackTreeSetTest, ComparisonOperatorsMap) {
  s21::RedBlackTree<int, int> tree;
  const int SIZE = 1000;

  // Insert elements
  for (int i = 0; i < SIZE; i++) {
    tree.insert({i, i});
  }

  s21::RedBlackTree<int, int>::iterator iter1{tree.begin()};
  s21::RedBlackTree<int, int>::iterator iter2{tree.begin()};

  *iter2++;
  *iter2++;

  EXPECT_FALSE(iter1 == iter2);
  EXPECT_TRUE(iter1 != iter2);
}

TEST(RedBlackTreeSetTest, ComparisonOperatorsMapConst) {
  s21::RedBlackTree<int, int> tree;
  const int SIZE = 1000;

  // Insert elements
  for (int i = 0; i < SIZE; i++) {
    tree.insert({i, i});
  }

  s21::RedBlackTree<int, int>::const_iterator iter1{tree.begin()};
  s21::RedBlackTree<int, int>::const_iterator iter2{tree.begin()};

  *iter2++;

  EXPECT_FALSE(iter1 == iter2);
  EXPECT_TRUE(iter1 != iter2);
}

TEST(RedBlackTreeSetTest, MapConstIterartor) {
  std::map<int, int> std_map{};
  s21::RedBlackTree<int, int> tree;
  const int SIZE = 1000;

  // Insert elements
  for (int i = 0; i < SIZE; i++) {
    tree.insert({i, i});
    std_map.insert(std::make_pair(i, i));
  }
  std::map<int, int>::iterator std_it_map{std_map.begin()};
  std::map<int, int>::const_iterator std_it_map_const{std_map.begin()};

  s21::RedBlackTree<int, int>::iterator s21_it_map{tree.begin()};
  s21::RedBlackTree<int, int>::const_iterator s21_it_map_const{tree.begin()};

  *s21_it_map++;
  *std_it_map++;
  *s21_it_map_const++;
  *std_it_map_const++;
  //   *std_it_map_const = std::make_pair(54, 347);
  //   *s21_it_map_const = {45, 46};
}

TEST(RedBlackTreeSetTest, SetConstIterartor) {
  std::set<int> std_set{};
  s21::RedBlackTree<int> tree;
  const int SIZE = 1000;

  // Insert elements
  for (int i = 0; i < SIZE; i++) {
    tree.insert(i);
    std_set.insert(i);
  }
  std::set<int>::iterator std_it_set{std_set.begin()};
  std::set<int>::const_iterator std_it_set_const{std_set.begin()};

  s21::RedBlackTree<int>::iterator s21_it_set{tree.begin()};
  s21::RedBlackTree<int>::const_iterator s21_it_set_const{tree.begin()};

  *s21_it_set++;
  *std_it_set++;
  *s21_it_set_const++;
  *std_it_set_const++;
  std_it_set_const = std_it_set;
  s21_it_set_const = s21_it_set;
}

TEST(RedBlackTreeMapTest, BeginEmpty) {
  s21::RedBlackTree<int, int> tree;
  EXPECT_EQ(tree.begin(), tree.end());
}

TEST(RedBlackTreeMapTest, BeginSingleElement) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({5, 100});
  auto it = tree.begin();
  EXPECT_EQ(it->first, 5);
  EXPECT_EQ(it->second, 100);
}

TEST(RedBlackTreeMapTest, BeginMultipleElements) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({10, 100});
  tree.insert({5, 200});
  tree.insert({15, 300});

  auto it = tree.begin();
  EXPECT_EQ(it->first, 5);  // Should point to smallest key
  EXPECT_EQ(it->second, 200);
}

TEST(RedBlackTreeSetTest, BeginEmpty) {
  s21::RedBlackTree<int> tree;
  EXPECT_EQ(tree.begin(), tree.end());
}

TEST(RedBlackTreeSetTest, BeginSingleElement) {
  s21::RedBlackTree<int> tree;
  tree.insert(5);
  auto it = tree.begin();
  EXPECT_EQ(*it, 5);
}

TEST(RedBlackTreeSetTest, BeginMultipleElements) {
  s21::RedBlackTree<int> tree;
  tree.insert(10);
  tree.insert(5);
  tree.insert(15);

  auto it = tree.begin();
  EXPECT_EQ(*it, 5);  // Should point to smallest element
}

// Test iterator traversal for Map
TEST(RedBlackTreeMapTest, IteratorTraversal) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({10, 100});
  tree.insert({5, 200});
  tree.insert({15, 300});

  auto it = tree.begin();
  EXPECT_EQ(it->first, 5);
  ++it;
  EXPECT_EQ(it->first, 10);
  ++it;
  EXPECT_EQ(it->first, 15);
  ++it;
  EXPECT_EQ(it, tree.end());
}

// Test iterator traversal for Set
TEST(RedBlackTreeSetTest, IteratorTraversal) {
  s21::RedBlackTree<int> tree;
  tree.insert(10);
  tree.insert(5);
  tree.insert(15);

  auto it = tree.begin();
  EXPECT_EQ(*it, 5);
  ++it;
  EXPECT_EQ(*it, 10);
  ++it;
  EXPECT_EQ(*it, 15);
  ++it;
  EXPECT_EQ(it, tree.end());
}

// Test iterator traversal for Set
TEST(RedBlackTreeSetTest, EraseUsingIterator) {
  s21::RedBlackTree<int> tree;
  tree.insert(1);
  tree.insert(3);
  tree.insert(2);
  tree.insert(4);
  tree.insert(5);
  tree.insert(6);

  auto it = tree.begin();
  while (it != tree.end()) {
    if (*it % 2 == 1) {
      it = tree.erase(it);  // erase() returns iterator to next element
    } else {
      ++it;
    }
  }

  EXPECT_EQ(tree.size(), size_t{3});
}

// Test iterator traversal for Set
TEST(RedBlackTreeSetTest, DecrementIterator) {
  s21::RedBlackTree<int> tree;
  tree.insert(1);
  tree.insert(3);
  tree.insert(2);
  tree.insert(4);
  tree.insert(5);
  tree.insert(6);

  auto it = tree.begin();

  ++it;
  ++it;
  ++it;
  --it;

  tree.erase(it);

  EXPECT_EQ(tree.size(), size_t{5});
  EXPECT_TRUE(tree.FindNode(3) == nullptr);
}

// Test iterator traversal for Set
TEST(RedBlackTreeSetTest, DecrementIteratorAfterEnd) {
  s21::RedBlackTree<int> tree;
  tree.insert(1);
  tree.insert(3);
  tree.insert(2);
  tree.insert(4);
  tree.insert(5);
  tree.insert(6);

  [[maybe_unused]] auto it = tree.end();

  // --it;

  // tree.erase(it);

  // EXPECT_EQ(tree.size(), size_t{5});
  // EXPECT_TRUE(tree.FindNode(3) == nullptr);
}
