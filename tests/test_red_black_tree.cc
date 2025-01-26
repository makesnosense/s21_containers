#include "s21_red_black_tree.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#include <gtest/gtest.h>
#pragma GCC diagnostic pop

TEST(RedBlackTreeTest, Meow) {
  s21::Tree<int, int> meow;
  meow.insert({10, 999});
  meow.insert({20, 999});
  print_tree(meow);
  meow.insert({30, 999});

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
  print_tree(meow);

  // meow.RotateRight(meow.get_root());
  // print_tree(meow);
}
