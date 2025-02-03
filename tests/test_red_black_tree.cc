#include "s21_red_black_tree.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#include <gtest/gtest.h>
#pragma GCC diagnostic pop

#include <algorithm>
#include <string>
#include <vector>

#include "random.h"

template <typename Key, typename T>
bool ValidateProperties(const s21::Node<Key, T>* node, int& black_height);

template <typename Key, typename T>
bool ValidateRedBlackTree(const s21::RedBlackTree<Key, T>& tree) {
  const s21::Node<Key, T>* root{tree.get_root()};

  if (!root) return true;

  if (root->color_ != s21::NodeColor::BLACK) {
    std::cerr << "Root must be black";
    return false;
  }

  int black_height = 0;
  return ValidateProperties(root, black_height);
}

template <typename Key, typename T>
bool ValidateBSTProperty(const s21::Node<Key, T>* node, const Key* min_key,
                         const Key* max_key) {
  if (!node) return true;

  if (min_key && node->GetKey() < *min_key) {
    std::cerr << "BST property violation: node key less than minimum bound";
    return false;
  }
  if (max_key && node->GetKey() > *max_key) {
    std::cerr << "BST property violation: node key greater than maximum bound";
    return false;
  }

  return ValidateBSTProperty(node->left_, min_key, &node->GetKey()) &&
         ValidateBSTProperty(node->right_, &node->GetKey(), max_key);
}

template <typename Key, typename T>
bool ValidateProperties(const s21::Node<Key, T>* node, int& black_height) {
  if (!node) {
    black_height = 0;
    return true;
  }

  if (!ValidateBSTProperty<Key, T>(node, nullptr, nullptr)) {
    return false;
  }

  int left_black_height = 0;
  int right_black_height = 0;

  if (ValidateProperties(node->left_, left_black_height) == false ||
      ValidateProperties(node->right_, right_black_height) == false) {
    return false;
  }

  if (left_black_height != right_black_height) {
    std::cerr << "Property 4: all paths must have same number of black nodes";
    return false;
  }

  if (node->color_ == s21::NodeColor::RED) {
    if ((node->left_ && node->left_->color_ == s21::NodeColor::RED) ||
        (node->right_ && node->right_->color_ == s21::NodeColor::RED)) {
      std::cerr << "Property 3: red nodes cannot have red children";
      return false;
    }
  }

  black_height = left_black_height;
  if (node->color_ == s21::NodeColor::BLACK) {
    black_height++;
  }

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

  EXPECT_TRUE(ValidateRedBlackTree(tree));
}
TEST(RedBlackTreeMapTest, EmptyAndSingleNode) {
  s21::RedBlackTree<int, int> tree;
  EXPECT_TRUE(ValidateRedBlackTree(tree));

  tree.insert({1, 999});
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, SimpleLeftRotation) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({10, 999});
  tree.insert({15, 999});
  tree.insert({20, 999});
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, SimpleRightRotation) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({20, 999});
  tree.insert({15, 999});
  tree.insert({10, 999});
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, LeftRightRotation) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({20, 999});
  tree.insert({10, 999});
  tree.insert({15, 999});
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, RightLeftRotation) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({10, 999});
  tree.insert({20, 999});
  tree.insert({15, 999});
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, RecoloringCase) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({10, 999});
  tree.insert({5, 999});
  tree.insert({15, 999});
  tree.insert({3, 999});
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

TEST(RedBlackTreeEraseNoFixup, RemoveRedLeaf) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({20, 1});
  tree.insert({10, 1});

  tree.erase(10);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeEraseNoFixup, RemoveRedMiddleNode) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({20, 1});
  tree.insert({10, 1});
  tree.insert({5, 1});

  tree.erase(10);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeEraseNoFixup, RemoveFromEmpty) {
  s21::RedBlackTree<int, int> tree;
  tree.erase(10);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeEraseNoFixup, RemoveNonexistentKey) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({20, 1});
  tree.insert({10, 1});

  tree.erase(30);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeEraseNoFixup, RemoveRedWithRedChild) {
  s21::RedBlackTree<int, int> tree;

  tree.insert({20, 1});
  tree.insert({30, 1});
  tree.insert({40, 1});

  size_t erased_elements{tree.erase(30)};
  EXPECT_TRUE(ValidateRedBlackTree(tree));
  EXPECT_EQ(erased_elements, size_t{1});
}

TEST(RedBlackTreeEraseNoFixup, RemoveRedWithLeftChild) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({20, 1});
  tree.insert({30, 1});
  tree.insert({25, 1});

  size_t erased_elements{tree.erase(30)};
  erased_elements += tree.erase(33);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
  EXPECT_EQ(erased_elements, size_t{1});
}

TEST(RedBlackTreeEraseNoFixup, RemoveOnlyRoot) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({20, 1});

  size_t erased_elements{tree.erase(20)};
  EXPECT_TRUE(ValidateRedBlackTree(tree));
  EXPECT_EQ(tree.get_root(), nullptr);
  EXPECT_EQ(erased_elements, size_t{1});
}

TEST(RedBlackTreeEraseNoFixup, RemoveRedNodeBothRedChildren) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({20, 1});
  tree.insert({10, 1});
  tree.insert({30, 1});
  tree.insert({5, 1});

  tree.erase(5);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeEraseNoFixup, ConsecutiveRemoveRed) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({20, 1});
  tree.insert({10, 1});
  tree.insert({30, 1});
  tree.insert({5, 1});
  tree.insert({15, 1});

  tree.erase(5);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
  tree.erase(15);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, EraseDoubleBlackWithRedSibling) {
  s21::RedBlackTree<int, int> tree;

  tree.insert({20, 999});
  tree.insert({10, 999});
  tree.insert({30, 999});
  tree.insert({25, 999});
  tree.insert({35, 999});
  tree.insert({23, 999});

  tree.erase(10);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, EraseDoubleBlackWithBlackSiblingBlackChildren) {
  s21::RedBlackTree<int, int> tree;

  tree.insert({20, 999});
  tree.insert({10, 999});
  tree.insert({30, 999});
  tree.insert({5, 999});
  tree.insert({40, 999});

  tree.erase(5);
  tree.erase(40);

  tree.erase(10);

  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, EraseLeafNode) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({10, 999});
  tree.insert({5, 999});
  tree.insert({15, 999});

  tree.erase(5);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, EraseNodeWithOneChild) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({10, 999});
  tree.insert({5, 999});
  tree.insert({15, 999});
  tree.insert({3, 999});

  tree.erase(5);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, EraseNodeWithTwoChildren) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({10, 999});
  tree.insert({5, 999});
  tree.insert({15, 999});
  tree.insert({3, 999});
  tree.insert({7, 999});

  tree.erase(5);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, EraseRoot) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({10, 999});
  tree.insert({5, 999});
  tree.insert({15, 999});

  tree.erase(10);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, EraseNonExistentKey) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({10, 999});
  tree.insert({5, 999});

  tree.erase(15);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, EraseFromEmptyTree) {
  s21::RedBlackTree<int, int> tree;
  tree.erase(10);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, EraseAllNodesSequentially) {
  s21::RedBlackTree<int, int> tree;
  std::vector<int> keys = {10, 5, 15, 3, 7, 13, 17};

  for (int key : keys) {
    tree.insert({key, 999});
  }

  for (int key : keys) {
    tree.erase(key);

    EXPECT_TRUE(ValidateRedBlackTree(tree));
  }
}

TEST(RedBlackTreeMapTest, EraseRedNode) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({10, 999});
  tree.insert({5, 999});
  tree.insert({15, 999});
  tree.insert({3, 999});

  tree.erase(3);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, EraseBlackNodeWithRedChild) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({10, 999});
  tree.insert({5, 999});
  tree.insert({15, 999});
  tree.insert({3, 999});
  tree.insert({7, 999});

  tree.erase(5);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, ComplexEraseScenario) {
  s21::RedBlackTree<int, int> tree;
  std::vector<int> keys = {50, 25, 75, 10, 30, 60, 80, 5,
                           15, 27, 35, 55, 65, 77, 85};

  for (int key : keys) {
    tree.insert({key, 999});
  }

  tree.erase(25);
  EXPECT_TRUE(ValidateRedBlackTree(tree));

  tree.erase(75);
  EXPECT_TRUE(ValidateRedBlackTree(tree));

  tree.erase(50);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, RepeatedEraseInsert) {
  s21::RedBlackTree<int, int> tree;

  for (int i = 1; i <= 5; i++) {
    tree.insert({i * 10, 999});
  }

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

  tree.erase(10);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
  EXPECT_TRUE(tree.get_root() == nullptr);
}

TEST(RedBlackTreeMapTest, EraseSequenceOfNodes) {
  s21::RedBlackTree<int, int> tree;

  std::vector<int> values = {50, 25, 75, 10, 30, 60, 80, 5, 15, 27, 35};
  for (int val : values) {
    tree.insert({val, 999});
  }

  tree.erase(25);
  EXPECT_TRUE(ValidateRedBlackTree(tree));

  tree.erase(5);
  EXPECT_TRUE(ValidateRedBlackTree(tree));

  tree.erase(75);
  EXPECT_TRUE(ValidateRedBlackTree(tree));

  tree.erase(50);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, EraseAllNodesInDifferentOrders) {
  s21::RedBlackTree<int, int> tree;
  std::vector<int> values = {20, 10, 30, 5, 15, 25, 35};

  for (int val : values) tree.insert({val, 999});
  std::sort(values.begin(), values.end());
  for (int val : values) {
    tree.erase(val);
    EXPECT_TRUE(ValidateRedBlackTree(tree));
  }

  for (int val : values) tree.insert({val, 999});
  std::sort(values.begin(), values.end(), std::greater<int>());
  for (int val : values) {
    tree.erase(val);
    EXPECT_TRUE(ValidateRedBlackTree(tree));
  }
}

TEST(RedBlackTreeMapTest, EraseBlackNodeCases) {
  s21::RedBlackTree<int, int> tree;

  tree.insert({20, 999});
  tree.insert({10, 999});
  tree.insert({30, 999});
  tree.insert({5, 999});
  tree.insert({15, 999});
  tree.insert({25, 999});
  tree.insert({35, 999});

  tree.erase(5);
  EXPECT_TRUE(ValidateRedBlackTree(tree));

  tree.erase(35);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, EraseWithRepeatedOperations) {
  s21::RedBlackTree<int, int> tree;

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

  tree.erase(10);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, EraseLastRemainingNode) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({10, 999});

  tree.erase(10);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
  EXPECT_TRUE(tree.get_root() == nullptr);
}

TEST(RedBlackTreeMapTest, EraseRootWithChildren) {
  s21::RedBlackTree<int, int> tree;
  tree.insert({20, 999});
  tree.insert({10, 999});
  tree.insert({30, 999});

  tree.erase(20);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeMapTest, RemovalFixupCases) {
  s21::RedBlackTree<int, int> tree;

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
  EXPECT_TRUE(ValidateRedBlackTree(tree));

  tree.insert(1);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, SimpleLeftRotation) {
  s21::RedBlackTree<int> tree;
  tree.insert(10);
  tree.insert(15);
  tree.insert(20);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, SimpleRightRotation) {
  s21::RedBlackTree<int> tree;
  tree.insert(20);
  tree.insert(15);
  tree.insert(10);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, LeftRightRotation) {
  s21::RedBlackTree<int> tree;
  tree.insert(20);
  tree.insert(10);
  tree.insert(15);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, RightLeftRotation) {
  s21::RedBlackTree<int> tree;
  tree.insert(10);
  tree.insert(20);
  tree.insert(15);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, RecoloringCase) {
  s21::RedBlackTree<int> tree;
  tree.insert(10);
  tree.insert(5);
  tree.insert(15);
  tree.insert(3);
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
  tree.insert(20);
  tree.insert(10);

  tree.erase(10);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, RemoveRedMiddleNode) {
  s21::RedBlackTree<int> tree;
  tree.insert(20);
  tree.insert(10);
  tree.insert(5);

  tree.erase(10);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, RemoveFromEmpty) {
  s21::RedBlackTree<int> tree;
  tree.erase(10);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, RemoveNonexistentKey) {
  s21::RedBlackTree<int> tree;
  tree.insert(20);
  tree.insert(10);

  tree.erase(30);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, RemoveRedWithRedChild) {
  s21::RedBlackTree<int> tree;
  tree.insert(20);
  tree.insert(30);
  tree.insert(40);

  tree.erase(30);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, RemoveRedWithLeftChild) {
  s21::RedBlackTree<int> tree;
  tree.insert(20);
  tree.insert(30);
  tree.insert(25);

  tree.erase(30);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, RemoveOnlyRoot) {
  s21::RedBlackTree<int> tree;
  tree.insert(20);

  tree.erase(20);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
  EXPECT_EQ(tree.get_root(), nullptr);
}

TEST(RedBlackTreeSetTest, RemoveRedNodeBothRedChildren) {
  s21::RedBlackTree<int> tree;
  tree.insert(20);
  tree.insert(10);
  tree.insert(30);
  tree.insert(5);

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

  tree.erase(5);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
  tree.erase(15);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, EraseDoubleBlackWithRedSibling) {
  s21::RedBlackTree<int> tree;

  tree.insert(20);
  tree.insert(10);
  tree.insert(30);
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

  tree.erase(5);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, EraseNodeWithOneChild) {
  s21::RedBlackTree<int> tree;
  tree.insert(10);
  tree.insert(5);
  tree.insert(15);
  tree.insert(3);

  tree.erase(5);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, EraseNodeWithTwoChildren) {
  s21::RedBlackTree<int> tree;
  tree.insert(10);
  tree.insert(5);
  tree.insert(15);
  tree.insert(3);
  tree.insert(7);

  tree.erase(5);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, EraseRoot) {
  s21::RedBlackTree<int> tree;
  tree.insert(10);
  tree.insert(5);
  tree.insert(15);

  tree.erase(10);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, EraseNonExistentKey) {
  s21::RedBlackTree<int> tree;
  tree.insert(10);
  tree.insert(5);

  tree.erase(15);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, EraseFromEmptyTree) {
  s21::RedBlackTree<int> tree;
  tree.erase(10);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, EraseAllNodesSequentially) {
  s21::RedBlackTree<int> tree;
  std::vector<int> keys = {10, 5, 15, 3, 7, 13, 17};

  for (int key : keys) {
    tree.insert(key);
  }

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

  tree.erase(3);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, EraseBlackNodeWithRedChild) {
  s21::RedBlackTree<int> tree;
  tree.insert(10);
  tree.insert(5);
  tree.insert(15);
  tree.insert(3);
  tree.insert(7);

  tree.erase(5);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, ComplexEraseScenario) {
  s21::RedBlackTree<int> tree;
  std::vector<int> keys = {50, 25, 75, 10, 30, 60, 80, 5,
                           15, 27, 35, 55, 65, 77, 85};

  for (int key : keys) {
    tree.insert(key);
  }

  tree.erase(25);
  EXPECT_TRUE(ValidateRedBlackTree(tree));

  tree.erase(75);
  EXPECT_TRUE(ValidateRedBlackTree(tree));

  tree.erase(50);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, RepeatedEraseInsert) {
  s21::RedBlackTree<int> tree;

  for (int i = 1; i <= 5; i++) {
    tree.insert(i * 10);
  }

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

  tree.erase(10);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
  EXPECT_TRUE(tree.get_root() == nullptr);
}

TEST(RedBlackTreeSetTest, EraseSequenceOfNodes) {
  s21::RedBlackTree<int> tree;

  std::vector<int> values = {50, 25, 75, 10, 30, 60, 80, 5, 15, 27, 35};
  for (int val : values) {
    tree.insert(val);
  }

  tree.erase(25);
  EXPECT_TRUE(ValidateRedBlackTree(tree));

  tree.erase(5);
  EXPECT_TRUE(ValidateRedBlackTree(tree));

  tree.erase(75);
  EXPECT_TRUE(ValidateRedBlackTree(tree));

  tree.erase(50);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, EraseAllNodesInDifferentOrders) {
  s21::RedBlackTree<int> tree;
  std::vector<int> values = {20, 10, 30, 5, 15, 25, 35};

  for (int val : values) tree.insert(val);
  std::sort(values.begin(), values.end());
  for (int val : values) {
    tree.erase(val);
    EXPECT_TRUE(ValidateRedBlackTree(tree));
  }

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

  tree.erase(5);
  EXPECT_TRUE(ValidateRedBlackTree(tree));

  tree.erase(35);
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

  tree.erase(10);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
}

TEST(RedBlackTreeSetTest, EraseLastRemainingNode) {
  s21::RedBlackTree<int> tree;
  tree.insert(10);

  tree.erase(10);
  EXPECT_TRUE(ValidateRedBlackTree(tree));
  EXPECT_TRUE(tree.get_root() == nullptr);
}

TEST(RedBlackTreeSetTest, StressTestRemoval) {
  s21::RedBlackTree<int> tree;
  const int SIZE = 1000;

  for (int i = 0; i < SIZE; i++) {
    tree.insert(i);
    EXPECT_TRUE(ValidateRedBlackTree(tree));
  }

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
}

TEST(RedBlackTreeSetTest, SetConstIterartor) {
  std::set<int> std_set{};
  s21::RedBlackTree<int> tree;
  const int SIZE = 1000;

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
  EXPECT_EQ(it->first, 5);
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
  EXPECT_EQ(*it, 5);
}

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
      it = tree.erase(it);
    } else {
      ++it;
    }
  }

  EXPECT_EQ(tree.size(), size_t{3});
}

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

TEST(RedBlackTreeSetTest, DecrementIteratorAfterEnd) {
  s21::RedBlackTree<int> tree;
  tree.insert(1);
  tree.insert(3);
  tree.insert(2);
  tree.insert(4);
  tree.insert(5);
  tree.insert(6);

  [[maybe_unused]] auto it = tree.end();

  --it;

  tree.erase(3);

  EXPECT_EQ(tree.size(), size_t{5});
  EXPECT_TRUE(tree.FindNode(3) == nullptr);
}

TEST(RedBlackTreeSetTest, DecrementEndIteratorBehavior) {
  std::set<int> std_set{};
  s21::RedBlackTree<int> s21_tree{};

  auto std_it = std_set.end();
  auto s21_it = s21_tree.end();

  for (int i = 1; i <= 5; ++i) {
    std_set.insert(i);
    s21_tree.insert(i);
  }

  std_it = std_set.end();
  s21_it = s21_tree.end();

  --std_it;
  --s21_it;

  EXPECT_EQ(*std_it, *s21_it);

  std_it = std_set.end();
  s21_it = s21_tree.end();

  --std_it;
  --std_it;
  --std_it;

  --s21_it;
  --s21_it;
  --s21_it;

  EXPECT_EQ(*std_it, *s21_it);
}

TEST(RedBlackTreeMapTest, DecrementEndIteratorBehavior) {
  std::map<int, std::string> std_map{};
  s21::RedBlackTree<int, std::string> s21_tree{};

  for (int i = 1; i <= 5; ++i) {
    std_map.insert({i, "value" + std::to_string(i)});
    s21_tree.insert({i, "value" + std::to_string(i)});
  }

  auto std_it = std_map.end();
  auto s21_it = s21_tree.end();

  --std_it;
  --s21_it;

  EXPECT_EQ(std_it->first, s21_it->first);
  EXPECT_EQ(std_it->second, s21_it->second);

  std_it = std_map.end();
  s21_it = s21_tree.end();

  --std_it;
  --std_it;

  --s21_it;
  --s21_it;

  EXPECT_EQ(std_it->first, s21_it->first);
  EXPECT_EQ(std_it->second, s21_it->second);
}
