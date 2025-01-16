#ifndef S21_VECTOR_H
#define S21_VECTOR_H

#include <iterator>

#include "s21_base.h"

namespace s21 {

template <typename T>
class VectorIterator {
 public:
  using iterator_category = std::random_access_iterator_tag;
  using value_type = T;
  using pointer = T*;
  using reference = T&;
  using difference_type = std::ptrdiff_t;

  explicit VectorIterator(pointer ptr) : ptr_(ptr) {}

  reference operator*() { return *ptr_; }
  reference operator[](difference_type n) const { return ptr_[n]; }

  VectorIterator& operator++() {
    ++ptr_;
    return *this;
  }

  VectorIterator operator++(int) {
    VectorIterator tmp = *this;
    ++ptr_;
    return tmp;
  }

  VectorIterator& operator--() {
    --ptr_;
    return *this;
  }
  VectorIterator operator--(int) {
    VectorIterator tmp = *this;
    --ptr_;
    return tmp;
  }

  VectorIterator& operator+=(difference_type n) {
    ptr_ += n;
    return *this;
  }

  VectorIterator operator+(difference_type n) const {
    return VectorIterator(ptr_ + n);
  }

  friend VectorIterator operator+(difference_type n,
                                  const VectorIterator& iter) {
    return VectorIterator(iter.ptr_ + n);
  }

  VectorIterator& operator-=(difference_type n) {
    ptr_ -= n;
    return *this;
  }

  difference_type operator-(const VectorIterator& other) const {
    return ptr_ - other.ptr_;
  }

  VectorIterator operator-(difference_type n) const {
    return VectorIterator(ptr_ - n);
  }

  bool operator==(const VectorIterator& other) const {
    return ptr_ == other.ptr_;
  }
  bool operator!=(const VectorIterator& other) const {
    return ptr_ != other.ptr_;
  }

  bool operator<(const VectorIterator& other) const {
    return ptr_ < other.ptr_;
  }

  bool operator>(const VectorIterator& other) const {
    return ptr_ > other.ptr_;
  }

  bool operator<=(const VectorIterator& other) const {
    return ptr_ <= other.ptr_;
  }

  bool operator>=(const VectorIterator& other) const {
    return ptr_ >= other.ptr_;
  }

 private:
  pointer ptr_;
};

template <typename T>
class vector {
 public:
  using traits = container_traits<T>;
  using value_type = typename traits::value_type;
  using reference = typename traits::reference;
  using const_reference = typename traits::reference;
  using iterator = VectorIterator<T>;
  using reverse_iterator = VectorIterator<T>;
  using const_iterator = const VectorIterator<T>;
  using size_type = typename traits::size_type;

  explicit vector(size_type n = kMinN)
      : data_{nullptr}, size_{n}, capacity_{n} {
    if (n > 0) {
      if (n > max_size()) {
        throw std::length_error("too much length");
      }
      data_ = new value_type[n]();
    }
  }

  vector(const vector& other)
      : data_{other.size_ ? new value_type[other.size_] : nullptr},
        size_{other.size_},
        capacity_{other.size_} {
    std::copy(other.data_, other.data_ + size_, data_);
  }

  vector(vector&& other) noexcept
      : data_{other.data_}, size_{other.size_}, capacity_{other.capacity_} {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }

  explicit vector(size_type size, value_type value)
      : data_{nullptr}, size_{0}, capacity_{0} {
    if (size > 0) {
      if (size > max_size()) {
        throw std::length_error("too much length");
      }
      data_ = new value_type[size]();
      size_ = size;
      capacity_ = size;

      for (size_type i{0}; i < size; i++) {
        data_[i] = value;
      }
    }
  }

  explicit vector(std::initializer_list<value_type> init)
      : data_{new value_type[init.size()]},
        size_{init.size()},
        capacity_{init.size()} {
    std::copy(init.begin(), init.end(), data_);
  }
  ~vector() { delete[] data_; }

  reference at(size_type position) {
    if (position >= size_ || position < 0) {
      throw std::out_of_range("Vector index out of range");
    }
    return data_[position];
  }

  size_type capacity() { return capacity_; }

  iterator begin() { return iterator(data_); }

  iterator end() { return iterator(data_ + size_); }

  reverse_iterator rbegin() { return reverse_iterator(data_ + size_ - 1); }

  reverse_iterator rend() { return reverse_iterator(data_ - 1); }

  reference front() { return data_[0]; }

  reference back() { return data_[size_ - 1]; }

  bool empty() const noexcept { return size_ == 0; }

  size_type size() const { return size_; }

  size_type max_size() const noexcept {
    return std::numeric_limits<size_type>::max() / sizeof(value_type);
  }

  void clear() {
    for (size_type i{0}; i < size_; i++) {
      data_[i].~value_type();
    }
    size_ = 0;
  }

  void resize(size_t n) {
    if (n < size_) {
      // Destroy excess elements
      for (size_type i = n; i < size_; i++) {
        data_[i].~value_type();
      }
    } else if (n > size_) {
      if (n > capacity_) {
        // Create new storage
        value_type* new_data = new value_type[n];
        // Move existing elements
        for (size_type i = 0; i < size_; i++) {
          new_data[i] = std::move(data_[i]);
        }
        // Initialize new elements
        for (size_type i = size_; i < n; i++) {
          new_data[i] =
              value_type();  // Use regular constructor instead of placement new
        }
        // Clean up old storage
        delete[] data_;
        data_ = new_data;
        capacity_ = n;
      } else {
        // Initialize new elements in existing storage
        for (size_type i = size_; i < n; i++) {
          data_[i] = value_type();  // Use assignment instead of placement new
        }
      }
    }
    size_ = n;
  }

  void reserve(size_type n) {
    if (n > max_size()) {
      throw std::length_error("too much length");
    }
    if (n > capacity_) {
      value_type* new_data = new value_type[n];
      for (size_type i{0}; i < size_; i++) {
        new_data[i] = std::move(data_[i]);
      }
      delete[] data_;

      data_ = new_data;
      capacity_ = n;
    }
  }

  void shrink_to_fit() {
    if (size_ < capacity_) {
      value_type* new_data = new value_type[size_];
      for (size_type i{0}; i < size_; i++) {
        new_data[i] = std::move(data_[i]);
      }
      delete[] data_;
      data_ = new_data;
      capacity_ = size_;
    }
  }

  void push_back(const value_type& value) {
    if (size_ == capacity_) {
      size_type new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
      if (capacity_ * 2 > max_size()) {
        throw std::length_error("too much length");
      }
      reserve(new_capacity);
    }
    data_[size_++] = value;
  }

  void pop_back() noexcept {
    if (size_ > 0) {
      --size_;
      data_[size_].~T();
    }
  }

  iterator insert(const_iterator position, const value_type& value) {
    size_type index = static_cast<size_type>(std::distance(begin(), position));
    resize(size() + 1);

    for (size_type i = size() - 1; i > index; --i) {
      data_[i] = std::move(data_[i - 1]);
    }
    data_[index] = value;

    return iterator(data_ + index);
  }

  template <class InputIter>
  void insert(const_iterator position, InputIter first, InputIter last) {
    size_type index = static_cast<size_type>(position - begin());
    size_type count = static_cast<size_type>(std::distance(first, last));

    if (count == 0) return;

    resize(size() + count);

    for (size_type i{size() - 1}; i >= index + count; --i) {
      data_[i] = std::move(data_[i - count]);
    }

    for (size_type i{0}; i < count; ++i) {
      data_[index + i] = *(first++);
    }
  }

  void insert(const_iterator position, size_type count,
              const value_type& value) {
    size_type index = static_cast<size_type>(std::distance(begin(), position));

    resize(size() + count);

    for (size_type i = size() - 1; i >= index + count; --i) {
      data_[i] = std::move(data_[i - count]);
    }

    for (size_type i{0}; i < count; ++i) {
      data_[index + i] = value;
    }
  }

  vector& operator=(const vector& other) {
    if (this != &other) {
      delete[] data_;
      size_ = other.size_;
      capacity_ = other.capacity_;
      if (capacity_ > 0) {
        data_ = new value_type[capacity_];
        std::copy(other.data_, other.data_ + size_, data_);
      } else {
        data_ = nullptr;
      }
    }
    return *this;
  }

  vector& operator=(vector&& other) noexcept {
    if (this != &other) {
      delete[] data_;
      data_ = other.data_;
      size_ = other.size_;
      capacity_ = other.capacity_;
      other.data_ = nullptr;
      other.size_ = 0;
      other.capacity_ = 0;
    }
    return *this;
  }

  bool operator==(const vector& other) const {
    if (size_ != other.size_) return false;
    for (size_type i{0}; i < size_; ++i) {
      if (data_[i] != other.data_) return false;
    }
    return true;
  }

  reference operator[](size_type position) { return data_[position]; }
  const_reference operator[](size_type position) const {
    return data_[position];
  }

 private:
  value_type* data_{nullptr};
  size_type size_{kMinN};
  size_type capacity_{kMinN};
  static constexpr size_type kMinN{0};
};

template <typename T>
bool operator==(const s21::vector<T>& first, const std::vector<T>& other) {
  if (first.size() != other.size()) {
    return false;
  }

  for (size_t i = 0; i < first.size(); ++i) {
    if (first[i] != other[i]) {
      return false;
    }
  }
  return true;
}

template <typename T>
bool operator==(const std::vector<T>& first, const s21::vector<T>& other) {
  return other == first;
}

}  // namespace s21

#endif  // S21_VECTOR_H
