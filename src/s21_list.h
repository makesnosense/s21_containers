
#ifndef S21_LIST_H
#define S21_LIST_H

#include <initializer_list>
#include <iostream>
#include <iterator>

#include "s21_base.h"
namespace s21 {

template <typename T>
class list {
 public:
  using traits = container_traits<T>;
  using value_type = typename traits::value_type;
  using reference = typename traits::reference;
  using const_reference = typename traits::const_reference;
  // using iterator = ListIterator<T>;
  // using reverse_iterator = ListIterator<T>;
  // using const_iterator = const ListIterator<T>;
  using size_type = typename traits::size_type;
  list() : head_{nullptr}, tail_{nullptr}, size_{0} {}
  explicit list(std::initializer_list<value_type> init)
      : head_{nullptr}, tail_{nullptr}, size_{0} {
    for (const T& value : init) {
      std::cout << value << ' ';
    }
    for (const T& value : init) {
      Node* new_node = new Node(value);
      if (!head_) {
        head_ = new_node;
        tail_ = new_node;
      } else {
        tail_->next_ = new_node;
        new_node->pre_ = tail_;
        tail_ = new_node;
      }
      size_++;
    }
  }
  list(size_type n) : head_{nullptr}, tail_{nullptr}, size_{0} {
    for (size_type i = 0; i < n; i++) {
      Node* new_node = new Node();
      if (!head_) {
        head_ = new_node;
        tail_ = new_node;
      } else {
        tail_->next_ = new_node;
        new_node->pre_ = tail_;
        tail_ = new_node;
      }
      size_++;
    }
  }
  list(const s21::list<T>& other) {
    (void)other;
    if (other.head_ != nullptr) {
      Node* new_node = new Node();
    }
  }

  void pop_back() {
    if (empty()) {
      throw std::out_of_range("List is empty");
    }

    if (head_ == tail_) {
      delete head_;
      head_ = nullptr;
      tail_ = nullptr;
      size_--;
      return;
    }

    tail_ = tail_->pre_;

    tail_->next_ = nullptr;

    size_--;
  }
  void push_front(const_reference value) {
    Node* new_node = new Node(value);
    if (empty()) {
      head_ = new_node;
      tail_ = new_node;
    } else {
      head_->pre_ = new_node;
      new_node->next_ = head_;
      head_ = new_node;
    }

    size_++;
  }
  void push_back(const_reference value) {
    Node* new_node = new Node(value);
    if (empty()) {
      head_ = new_node;
      tail_ = new_node;
    } else {
      new_node->pre_ = tail_;
      tail_->next_ = new_node;

      tail_ = new_node;
    }

    size_++;
  }

  void reverse() {
    if (size_ <= 1) return;

    Node* current = head_;
    Node* prev = nullptr;
    Node* next = nullptr;

    while (current) {
      next = current->next_;
      current->next_ = prev;
      current->pre_ = next;
      prev = current;
      current = next;
    }

    head_ = tail_;
    tail_ = current;
  }
  void pop_front() {
    if (empty()) {
      throw std::out_of_range("List is empty");
    }

    if (head_ == tail_) {
      delete head_;
      head_ = nullptr;
      tail_ = nullptr;
      size_--;
      return;
    }
    head_ = head_->next_;
    head_->pre_ = nullptr;
  }

  size_type max_size() const noexcept {
    return (std::numeric_limits<size_type>::max() / sizeof(value_type)) / 4;
  }

  reference get_element(size_type index) {
    if (index >= size_) {
      throw std::out_of_range("Index out of range");
    }
    Node* current = head_;
    for (size_type i = 0; i < index; ++i) {
      current = current->next_;
    }
    return current->data_;
  }
  reference front() { return head_->data_; }

  reference back() { return tail_->data_; }

  size_type size() const { return size_; }

  bool empty() const noexcept { return size_ == 0; }

  T& operator[](size_type index) {
    if (index >= size_) {
      throw std::out_of_range("Index out of range");
    }
    Node* current = head_;
    for (size_type i = 0; i < index; ++i) {
      current = current->next_;
    }
    return current->data_;
  }

  const T& operator[](size_type index) const {
    if (index >= size_) {
      throw std::out_of_range("Index out of range");
    }
    Node* current = head_;
    for (size_type i = 0; i < index; ++i) {
      current = current->next_;
    }
    return current->data_;
  }
  list& operator=(const list& other) = default;

  int get_size() const;

  void clear();

  ~list() = default;
  ///////////////////////////////////
 private:
#ifdef __GNUC__  // For GCC/Clang
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#endif
  struct Node {
    alignas(alignof(value_type)) value_type data_;
    Node* next_;
    Node* pre_;

    Node() : data_(T()), next_(nullptr), pre_(nullptr) {}
    Node(value_type val) : data_(val), next_(nullptr), pre_(nullptr) {}
    Node(const Node&) = default;
    Node& operator=(const Node&) = default;
    ~Node() = default;
  };

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif

  Node* head_;
  Node* tail_;
  size_type size_;
};

}  // namespace s21

#endif  // S21_LIST_H
