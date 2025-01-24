#include "s21_red_black_tree.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#include <gtest/gtest.h>
#pragma GCC diagnostic pop

TEST(RedBlackTreeTest, Meow) {
  s21::Tree<int, int> meow;
  meow.insert({33, 999});

  meow.insert({65, 999});
  meow.insert({77, 999});
  meow.insert({12, 999});

  meow.insert({1, 999});
  meow.insert({42, 999});

  meow.insert({55, 999});
  meow.insert({66, 999});
  print_tree(meow);
}
