#ifndef S21_STACK_H
#define S21_STACK_H

#include <initializer_list>
#include <iterator>
#include <limits>
#include <stdexcept>

namespace s21 {
template <typename T, bool is_const>
class ArrayIterator;
template <typename T, std::size_t N>
class array {
 public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = std::size_t;
  using iterator = ArrayIterator<T, false>;
  using const_iterator = ArrayIterator<T, true>;

 public:
  constexpr array() noexcept : data_{} {}

  constexpr array(const array& other) noexcept : data_{} {
    for (size_type i = 0; i < N; ++i) {
      data_[i] = other.data_[i];
    }
  }

  constexpr array(array&& other) noexcept : data_{} {
    // Initialize first with empty array
    for (size_type i = 0; i < N; ++i) {
      data_[i] = std::move(other.data_[i]);
    }
  }

  constexpr array(std::initializer_list<value_type> const& items) noexcept {
    // First zero-initialize all elements
    for (size_type i = 0; i < N; ++i) {
      data_[i] = T{};
    }

    // Then copy from initializer list up to N elements
    size_type i = 0;
    for (const auto& item : items) {
      if (i >= N) break;  // Instead of throwing, just stop
      data_[i++] = item;
    }
  }

  constexpr reference operator[](size_type index) noexcept {
    return data_[index];
  }

  constexpr const_reference operator[](size_type index) const noexcept {
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
  constexpr bool empty() const noexcept { return N == 0; }

  constexpr void fill(const T& value) {
    for (size_type i{0}; i < N; ++i) {
      data_[i] = value;
    }
  }
  constexpr reference at(size_type pos) { return data_[pos]; }

  constexpr reference front() noexcept { return data_[0]; }
  constexpr const_reference front() const noexcept { return data_[0]; }

  constexpr reference back() noexcept { return data_[N - 1]; }
  constexpr const_reference back() const noexcept { return data_[N - 1]; }

  constexpr void swap(array& other) {
    for (size_type i{0}; i < N; ++i) {
      std::swap(data_[i], other.data_[i]);
    }
  }

  constexpr size_type max_size() const noexcept { return N; }

  iterator begin() { return iterator(data_); }
  iterator end() { return iterator(data_ + N); }
  const_iterator begin() const { return const_iterator(data_); }
  const_iterator end() const { return const_iterator(data_ + N); }
  const_iterator cbegin() const { return const_iterator(data_); }
  const_iterator cend() const { return const_iterator(data_ + N); }

  iterator data() noexcept { return iterator(data_); }
  const_iterator data() const noexcept { return const_iterator(data_); }

  ~array() = default;

 private:
  value_type data_[N];
};

template <typename T, bool is_const>
class ArrayIterator {
 public:
  using value_type = T;
  using reference = std::conditional_t<is_const, const T&, T&>;
  using pointer = std::conditional_t<is_const, const T*, T*>;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::random_access_iterator_tag;

 public:
  explicit ArrayIterator(pointer other) : ptr_(other) {}

  operator pointer() const { return ptr_; }

  reference operator*() { return *ptr_; }

  pointer operator->() { return ptr_; }

  ArrayIterator& operator++() {
    ++ptr_;
    return *this;
  }
  ArrayIterator operator++(int) {
    ArrayIterator temp = *this;
    ++ptr_;
    return temp;
  }

  ArrayIterator& operator--() {
    --ptr_;
    return *this;
  }

  ArrayIterator operator--(int) {
    ArrayIterator temp = *this;
    --ptr_;
    return temp;
  }

  ArrayIterator operator+(difference_type n) const {
    return ArrayIterator(ptr_ + n);
  }

  ArrayIterator operator-(difference_type n) const {
    return ArrayIterator(ptr_ - n);
  }

  bool operator==(const ArrayIterator& other) const {
    return ptr_ == other.ptr_;
  }

  bool operator!=(const ArrayIterator& other) const {
    return ptr_ != other.ptr_;
  }

  bool operator<(const ArrayIterator& other) const { return ptr_ < other.ptr_; }

  bool operator<=(const ArrayIterator& other) const {
    return ptr_ <= other.ptr_;
  }

  bool operator>(const ArrayIterator& other) const { return ptr_ > other.ptr_; }

  bool operator>=(const ArrayIterator& other) const {
    return ptr_ >= other.ptr_;
  }

 private:
  pointer ptr_;
};

}  // namespace s21

#endif
