#ifndef S21_BASE_H
#define S21_BASE_H

namespace s21 {

template <typename T>
class Iterator {
 public:
  using value_type = T;
  using pointer = T*;
  using reference = T&;
  using difference_type = std::ptrdiff_t;

  // Essential iterator operations
  virtual reference operator*() = 0;
  virtual Iterator& operator++() = 0;    // prefix
  virtual Iterator operator++(int) = 0;  // postfix
  virtual bool operator==(const Iterator& other) const = 0;
  virtual bool operator!=(const Iterator& other) const = 0;
};

template <typename T>
struct container_traits {
  using value_type = T;
  using size_type = std::size_t;
  using reference = T&;
  using const_reference = const T&;
};

}  // namespace s21

#endif  // S21_BASE_H
