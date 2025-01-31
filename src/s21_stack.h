#ifndef S21_STACK_H
#define S21_STACK_H

#include <initializer_list>
#include <iostream>

#include "s21_deque.h"
namespace s21 {

template <typename T>
class stack {
 private:
  deque<T> data_;

 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;

  stack() : data_() {}
  stack(const stack &other) : data_(other.data_) {}
  stack(stack &&other) noexcept : data_(std::move(other.data_)) {}
  stack(std::initializer_list<T> const &init) : stack(init) {}

  void push(const_reference value) { data_.push_front(value); }

  void pop() {
    if (!empty()) {
      data_.pop_back();
    } else {
      throw std::logic_error("is empty");
    }
  }

  const_reference top() const {
    if (!empty()) {
      return data_.back();
    } else {
      throw std::logic_error("is empty");
    }
  }

  void swap(stack &other) noexcept { std::swap(data_, other.data_); }

  size_t size() const { return data_.size(); }

  bool empty() const { return const_cast<deque<T> &>(data_).empty(); }

  stack &operator=(const stack &other) {
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

  ~stack() { clear(); }
};

}  // namespace s21

#endif