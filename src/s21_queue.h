#ifndef S21_QUEUE_H
#define S21_QUEUE_H

#include <initializer_list>
#include <iostream>
#include <iterator>

#include "s21_base.h"
#include "s21_deque.h"

namespace s21 {

template <typename T>
class queue {
 private:
  deque<T> data_;

 public:
  using traits = container_traits<T>;
  using value_type = typename traits::value_type;
  using reference = typename traits::reference;
  using const_reference = typename traits::const_reference;
  queue() : data_() {};

  queue(std::initializer_list<T> const &init) : data_(init) {}
  queue(const queue &other) : data_(other.data_) {}

  queue(queue &&other) noexcept : data_(std::move(other.data_)) {}
  void push(const_reference value) { data_.push_back(value); }

  void pop() {
    if (!empty()) {
      data_.pop_front();
    } else {
      throw std::out_of_range("Queue is empty");
    }
  }

  const_reference front() const {
    if (!empty()) {
      return data_.front();
    } else {
      throw std::out_of_range("Queue is empty");
    }
  }
  const_reference back() const {
    if (!empty()) {
      return data_.back();
    } else {
      throw std::out_of_range("Queue is empty");
    }
  }
  void swap(queue &other) noexcept { std::swap(data_, other.data_); }

  size_t size() const { return data_.size(); }

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

  ~queue() { clear(); }
};

}  // namespace s21

#endif