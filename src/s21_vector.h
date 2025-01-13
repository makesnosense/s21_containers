#ifndef S21_VECTOR_H
#define S21_VECTOR_H

#include <iterator>

#include "s21_base.h"

namespace s21 {

template <typename T>
class VectorIterator : public s21::Iterator<T> {
 public:
  using iterator_category = std::random_access_iterator_tag;
  using value_type = T;
  using pointer = T*;
  using reference = T&;
  using difference_type = std::ptrdiff_t;

  explicit VectorIterator(pointer ptr) : ptr_(ptr) {}

  reference operator*() override { return *ptr_; }
  VectorIterator& operator++() override {
    ++ptr_;
    return *this;
  }

  VectorIterator operator++(int) override {
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
  bool operator==(const VectorIterator& other) const override {
    return ptr_ == other.ptr_;
  }
  bool operator!=(const VectorIterator& other) const override {
    return ptr_ != other.ptr_;
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
  using const_iterator = const VectorIterator<T>;
  using size_type = typename traits::size_type;

  explicit vector(size_type n = kMinN)
      : data_{nullptr}, size_{n}, capacity_{n} {
    if (n > 0) {
      data_ = new value_type[n]();
    }
  }

  vector(const vector& other)
      : data_{nullptr}, size_{other.size_}, capacity_{other.capacity_} {
    delete[] data_;
    data_ = new value_type[other.size_];
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
      data_ = new value_type[size]();
      size_ = size;
      capacity_ = size;

      for (size_type i = 0; i < size; i++) {
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
  VectorIterator<value_type> begin() {
    return VectorIterator<value_type>(data_);
  }
  VectorIterator<value_type> end() {
    return VectorIterator<value_type>(data_ + size_);
  }

  size_type size() { return size_; }

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

  reference operator[](size_type position) { return data_[position]; }
  const_reference operator[](size_type position) const {
    return data_[position];
  }

 private:
  // using traits = container_traits<T>;

  value_type* data_;
  size_type size_;
  size_type capacity_;
  static constexpr size_type kMinN{0};

  void resize(size_type new_capacity) {
    value_type* new_data = new value_type[new_capacity];
    size_type copy_size = std::min(size_, new_capacity);
    for (size_type i = 0; i < copy_size; ++i) {
      new_data[i] = std::move(data_[i]);
    }
    delete[] data_;
    data_ = new_data;
    capacity_ = new_capacity;
    size_ = copy_size;
  }
};

}  // namespace s21

#endif  // S21_VECTOR_H
