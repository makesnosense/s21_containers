#include <cstdint>

#include "s21_base.h"
namespace s21 {

enum class NodeColor : int8_t { RED = 0, BLACK = 1 };

template <typename T>
class Tree {
 public:
  using traits = container_traits<T>;
  using value_type = typename traits::value_type;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = typename traits::reference;
  using const_reference = typename traits::const_reference;
  using size_type = typename traits::size_type;

  struct Node {
    value_type value;
    size_type key;
  };
};
}  // namespace s21