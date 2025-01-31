#ifndef S21_STACK_H
#define S21_STACK_H

#include <initializer_list>
#include <iostream>
#include <iterator>
#include <stdexcept>

namespace s21 {

template <typename T, std::size_t N>
class array {
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = typename traits::size_type;

 private:
  value_type data[N];

 public:
  array() {
    for (size_type i = 0; i < N; ++i) {
      data[i] = T();
    }
  }

  reference operator[](size_type index) {
    if (index >= N) {
      throw std::logic_error("Index out of range");
    }
    return data[index];
  }

  const_reference operator[](size_type index) const {
    if (index >= N) {
      throw std::logic_error("Index out of range");
    }
    return data[index];
  }

  size_type size() const { return N; }
  bool empty() { return N == 0; }

  void fill(const T& value) {
    for (size_type i = 0; i < N; ++i) {
      data[i] = value;
    }
  }

  void print() const {
    for (size_type i = 0; i < N; ++i) {
      std::cout << data[i] << " ";
    }
    std::cout << std::endl;
  }
  size_type max_size() {
    return (std::numeric_limits<size_type>::max() / sizeof(value_type));
  }

  ~array() = default;
};

}  // namespace s21

#endif