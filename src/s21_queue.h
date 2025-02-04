#ifndef S21_QUEUE_H
#define S21_QUEUE_H

#include <initializer_list>

#include "s21_deque.h"

namespace s21 {

template <typename T>
class queue {
 public:
  using size_type = std::size_t;
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;

  queue() : data_() {};
  queue(std::initializer_list<T> const &init) : data_(init) {}
  queue(const queue &other) : data_(other.data_) {}
  queue(queue &&other) noexcept : data_(std::move(other.data_)) {}
  ~queue() { clear(); }

  template <typename... Args>
  void insert_many_back(Args &&...args) {
    (push(std::forward<Args>(args)), ...);
  }

  void push(const_reference value) { data_.push_back(value); }

  void pop() {
    if (!empty()) {
      data_.pop_front();
    } else {
      throw std::logic_error("Queue is empty");
    }
  }

  const_reference front() const {
    if (!empty()) {
      return data_.front();
    } else {
      throw std::logic_error("Queue is empty");
    }
  }
  const_reference back() const {
    if (!empty()) {
      return data_.back();
    } else {
      throw std::logic_error("Queue is empty");
    }
  }
  void swap(queue &other) noexcept { std::swap(data_, other.data_); }

  size_type size() const { return data_.size(); }

  bool empty() const { return const_cast<deque<T> &>(data_).empty(); }

  queue &operator=(queue &&other) noexcept {
    if (this != &other) {
      clear();
      data_ = std::move(other.data_);
      other.clear();
    }
    return *this;
  }
  queue &operator=(const queue &other) {
    if (this != &other) {
      data_ = other.data_;
    }
    return *this;
  }
  void clear() {
    while (!empty()) {
      pop();
    }
  }

 private:
  deque<T> data_;
};

}  // namespace s21

#endif
