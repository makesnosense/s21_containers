
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
      } else {
        Node* current = head_;
        while (current->next_) {
          current = current->next_;
        }
      }
    }
  }

  list(const s21::list<T>&) = default;
  ~list() = default;
  list& operator=(const list& other) = default;

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

    Node* temp = tail_;
    tail_ = tail_->pre_;

    tail_->next_ = nullptr;

    free(temp);
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
  void pop_front();

  const_reference get_front() const {
    if (!empty()) {
      return head_->data_;
    }
    throw std::out_of_range("List is empty");
  }
  const_reference get_back() const;

  int get_size() const;
  void clear();
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

  size_type size() const { return size_; }

  bool empty() const noexcept { return size_ == 0; }

 private:
  struct Node {
    value_type data_;
    Node* next_;
    Node* pre_;
    Node(value_type val) : data_(val), next_(nullptr), pre_(nullptr) {}
    Node(const Node&) = default;
    Node& operator=(const Node&) = delete;
    ~Node() = default;
  };

  Node* head_;
  Node* tail_;
  size_type size_;
};

}  // namespace s21

#endif  // S21_LIST_H
