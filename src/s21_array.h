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
  using size_type = std::size_t;

 private:
  value_type data[N];

 public:
  constexpr array() {
    for (size_type i = 0; i < N; ++i) {
      data[i] = T();
    }
  }

  constexpr reference operator[](size_type index) {
    if (index >= N) {
      throw std::logic_error("Index out of range");
    }
    return data[index];
  }

  constexpr const_reference operator[](size_type index) const {
    if (index >= N) {
      throw std::logic_error("Index out of range");
    }
    return data[index];
  }

  constexpr size_type size() const { return N; }
  constexpr bool empty() { return N == 0; }

  constexpr void fill(const T& value) {
    for (size_type i = 0; i < N; ++i) {
      data[i] = value;
    }
  }
  constexpr reference at(size_type pos) { return data[pos]; }
  constexpr reference front() {
    if (N == 0) {
      throw std::logic_error("Index out of range");
    }
    return data[0];
  }
  constexpr reference back() {
    if (N == 0) {
      throw std::logic_error("Index out of range");
    }
    return data[N - 1];
  }

  constexpr void swap(array& other) {
    for (size_type i = 0; i < N; ++i) {
      std::swap(data[i], other.data[i]);
    }
  }

  constexpr void print() const {
    for (size_type i = 0; i < N; ++i) {
      std::cout << data[i] << " ";
    }
    std::cout << std::endl;
  }
  constexpr size_type max_size() {
    return (std::numeric_limits<size_type>::max() / sizeof(value_type));
  }

  ~array() = default;
};

}  // namespace s21

#endif