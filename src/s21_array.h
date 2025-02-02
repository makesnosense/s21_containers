#ifndef S21_STACK_H
#define S21_STACK_H

#include <initializer_list>
#include <iostream>
#include <iterator>
#include <stdexcept>

namespace s21 {
template <typename T>
class ArrayIterator;
template <typename T, std::size_t N>
class array {
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = std::size_t;
  using iterator = s21::ArrayIterator<T>;

 private:
  value_type data_[N];

 public:
  constexpr array() {
    for (size_type i = 0; i < N; ++i) {
      data_[i] = T();
    }
  }
  constexpr array(array&& other) noexcept {
    for (size_type i = 0; i < N; ++i) {
      data_[i] = std::move(other.data_[i]);
    }
  }
  constexpr array(const array& other) {
    for (size_type i = 0; i < N; ++i) {
      data_[i] = other.data_[i];
    }
  }
  constexpr array(std::initializer_list<value_type> const& items) {
    if (items.size() > N) {
      throw std::logic_error("Index out of range");
    }
    std::copy(items.begin(), items.end(), data_);
    for (size_type i = items.size(); i < N; ++i) {
      data_[i] = T();
    }
  }
  constexpr reference operator[](size_type index) {
    if (index >= N) {
      throw std::logic_error("Index out of range");
    }
    return data_[index];
  }

  constexpr const_reference operator[](size_type index) const {
    if (index >= N) {
      throw std::logic_error("Index out of range");
    }
    return data_[index];
  }
  constexpr array& operator=(array&& other) noexcept {
    if (this != &other) {
      for (size_type i = 0; i < N; ++i) {
        data_[i] = std::move(other.data_[i]);
      }
    }
    return *this;
  }

  constexpr size_type size() const { return N; }
  constexpr bool empty() { return N == 0; }

  constexpr void fill(const T& value) {
    for (size_type i = 0; i < N; ++i) {
      data_[i] = value;
    }
  }
  constexpr reference at(size_type pos) { return data_[pos]; }
  constexpr reference front() {
    if (N == 0) {
      throw std::logic_error("Index out of range");
    }
    return data_[0];
  }
  constexpr reference back() {
    if (N == 0) {
      throw std::logic_error("Index out of range");
    }
    return data_[N - 1];
  }

  constexpr void swap(array& other) {
    for (size_type i = 0; i < N; ++i) {
      std::swap(data_[i], other.data_[i]);
    }
  }

  constexpr void print() const {
    for (size_type i = 0; i < N; ++i) {
      std::cout << data_[i] << " ";
    }
    std::cout << std::endl;
  }
  constexpr size_type max_size() {
    return (std::numeric_limits<size_type>::max() / sizeof(value_type));
  }
  iterator begin() { return iterator(data_); }

  iterator end() { return iterator(data_ + N); }
  iterator data() { return iterator(data_); }
  ~array() = default;
};
template <typename T>
class ArrayIterator {
 public:
  using value_type = T;
  using reference = T&;
  using pointer = T*;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::random_access_iterator_tag;

 private:
  pointer ptr;

 public:
  explicit ArrayIterator(pointer p) : ptr(p) {}

  reference operator*() { return *ptr; }

  pointer operator->() { return ptr; }

  ArrayIterator& operator++() {
    ++ptr;
    return *this;
  }
  ArrayIterator operator++(int) {
    ArrayIterator temp = *this;
    ++ptr;
    return temp;
  }

  ArrayIterator& operator--() {
    --ptr;
    return *this;
  }

  ArrayIterator operator--(int) {
    ArrayIterator temp = *this;
    --ptr;
    return temp;
  }

  ArrayIterator operator+(difference_type n) const {
    return ArrayIterator(ptr + n);
  }

  ArrayIterator operator-(difference_type n) const {
    return ArrayIterator(ptr - n);
  }

  bool operator==(const ArrayIterator& other) const { return ptr == other.ptr; }

  bool operator!=(const ArrayIterator& other) const { return ptr != other.ptr; }

  bool operator<(const ArrayIterator& other) const { return ptr < other.ptr; }

  bool operator<=(const ArrayIterator& other) const { return ptr <= other.ptr; }

  bool operator>(const ArrayIterator& other) const { return ptr > other.ptr; }

  bool operator>=(const ArrayIterator& other) const { return ptr >= other.ptr; }
};

}  // namespace s21

#endif