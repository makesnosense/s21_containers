
#ifndef S21_LIST_H
#define S21_LIST_H

#include <initializer_list>
#include <iostream>
#include <iterator>

#include "s21_base.h"
namespace s21 {

template <typename T>
class ListIterator;
template <typename T>
class list {
 public:
  friend class ListIterator<T>;
  using traits = container_traits<T>;
  using value_type = typename traits::value_type;
  using reference = typename traits::reference;
  using const_reference = typename traits::const_reference;

  using pointer = T*;

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

  std::pair<Node*, Node*> split(Node* head) {
    Node* slow = head;
    Node* fast = head->next_;

    while (fast != nullptr && fast->next_ != nullptr) {
      slow = slow->next_;
      fast = fast->next_->next_;
    }

    Node* secondHalf = slow->next_;
    slow->next_ = nullptr;
    if (secondHalf != nullptr) {
      secondHalf->pre_ = nullptr;
    }

    return {head, secondHalf};
  }

  Node* merge(Node* first, Node* second) {
    if (first == nullptr) return second;
    if (second == nullptr) return first;

    Node* mergedHead = nullptr;

    if (first->data_ <= second->data_) {
      mergedHead = first;
      first = first->next_;
    } else {
      mergedHead = second;
      second = second->next_;
    }

    Node* current = mergedHead;

    while (first != nullptr && second != nullptr) {
      if (first->data_ <= second->data_) {
        current->next_ = first;
        first->pre_ = current;
        first = first->next_;
      } else {
        current->next_ = second;
        second->pre_ = current;
        second = second->next_;
      }
      current = current->next_;
    }

    if (first != nullptr) {
      current->next_ = first;
      first->pre_ = current;
    } else if (second != nullptr) {
      current->next_ = second;
      second->pre_ = current;
    }

    return mergedHead;
  }

  Node* mergeSort(Node* head) {
    if (head == nullptr || head->next_ == nullptr) {
      return head;
    }

    auto [firstHalf, secondHalf] = split(head);

    firstHalf = mergeSort(firstHalf);
    secondHalf = mergeSort(secondHalf);

    return merge(firstHalf, secondHalf);
  }

 public:
  using iterator = ListIterator<T>;
  // using reverse_iterator = ListIterator<T>;
  using const_iterator = const ListIterator<T>;
  using size_type = typename traits::size_type;

  list() : head_{nullptr}, tail_{nullptr}, size_{0} {}
  explicit list(std::initializer_list<value_type> init)
      : head_{nullptr}, tail_{nullptr}, size_{0} {
    // for (const T& value : init) {
    //   std::cout << value << ' ';
    // }
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
  list(const s21::list<T>& other) : head_(nullptr), tail_(nullptr), size_(0) {
    if (other.head_ != nullptr) {
      Node* current = other.head_;
      while (current != nullptr) {
        push_back(current->data_);
        current = current->next_;
      }
    }
  }
  list(list&& l) noexcept : head_(l.head_), tail_(l.tail_), size_(l.size_) {
    l.head_ = nullptr;
    l.tail_ = nullptr;
    l.size_ = 0;
  }
  iterator insert(iterator pos, const_reference value) {
    Node* newNode = new Node(value);

    if (pos.getCurrent() == nullptr) {
      if (tail_ != nullptr) {
        tail_->next_ = newNode;
        newNode->pre_ = tail_;
        tail_ = newNode;
      } else {
        head_ = newNode;
        tail_ = newNode;
      }
    } else {
      Node* currentNode = pos.getCurrent();

      if (currentNode->pre_ != nullptr) {
        currentNode->pre_->next_ = newNode;
        newNode->pre_ = currentNode->pre_;
      } else {
        head_ = newNode;
      }

      newNode->next_ = currentNode;
      currentNode->pre_ = newNode;
    }

    size_++;
    return iterator(newNode);
  }

  void splice(const_iterator pos, list& other) {
    if (other.empty()) return;

    Node* posNode = pos.getCurrent();
    Node* otherHead = other.head_;
    Node* otherTail = other.tail_;

    if (posNode == head_) {
      head_ = otherHead;
    } else {
      posNode->pre_->next_ = otherHead;
      otherHead->pre_ = posNode->pre_;
    }

    if (posNode != nullptr) {
      otherTail->next_ = posNode;
      posNode->pre_ = otherTail;
    } else {
      tail_ = otherTail;
    }

    size_ += other.size_;

    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.size_ = 0;
  }

  void merge(list& other) {
    if (other.head_ == nullptr) {
      return;
    }

    if (head_ == nullptr) {
      head_ = other.head_;
      tail_ = other.tail_;
      size_ = other.size_;

      other.head_ = nullptr;
      other.tail_ = nullptr;
      other.size_ = 0;
      return;
    }

    Node* current1 = head_;
    Node* current2 = other.head_;
    Node* mergedHead = nullptr;
    Node* mergedTail = nullptr;

    if (current1->data_ <= current2->data_) {
      mergedHead = current1;
      current1 = current1->next_;
    } else {
      mergedHead = current2;
      current2 = current2->next_;
    }

    mergedTail = mergedHead;

    while (current1 != nullptr && current2 != nullptr) {
      if (current1->data_ <= current2->data_) {
        mergedTail->next_ = current1;
        current1->pre_ = mergedTail;
        mergedTail = current1;
        current1 = current1->next_;
      } else {
        mergedTail->next_ = current2;
        current2->pre_ = mergedTail;
        mergedTail = current2;
        current2 = current2->next_;
      }
    }

    if (current1 != nullptr) {
      mergedTail->next_ = current1;
      current1->pre_ = mergedTail;
    } else if (current2 != nullptr) {
      mergedTail->next_ = current2;
      current2->pre_ = mergedTail;
    }

    head_ = mergedHead;
    tail_ = mergedTail;
    size_ += other.size_;

    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.size_ = 0;
  }
  void swap(list& other) noexcept {
    if (this != &other) {
      std::swap(head_, other.head_);
      std::swap(tail_, other.tail_);
      std::swap(size_, other.size_);
    }
  }

  void unique() {
    if (head_ == nullptr) return;

    Node* current = head_;

    while (current != nullptr) {
      Node* nextNode = current->next_;

      while (nextNode != nullptr && nextNode->data_ == current->data_) {
        Node* duplicateNode = nextNode;
        nextNode = nextNode->next_;
        size_--;

        delete duplicateNode;
      }

      current->next_ = nextNode;
      if (nextNode != nullptr) {
        nextNode->pre_ = current;
      } else {
        tail_ = current;
      }

      current = current->next_;
    }
  }
  void sort() {
    if (head_ == nullptr || head_->next_ == nullptr) {
      return;
    }

    head_ = mergeSort(head_);

    tail_ = head_;
    while (tail_->next_ != nullptr) {
      tail_ = tail_->next_;
    }
  }
  void erase(ListIterator<T> pos) {
    if (pos.getCurrent() == nullptr) return;

    Node* nodeToDelete = pos.getCurrent();

    if (nodeToDelete->pre_) {
      nodeToDelete->pre_->next_ = nodeToDelete->next_;
    } else {
      head_ = nodeToDelete->next_;
    }

    if (nodeToDelete->next_) {
      nodeToDelete->next_->pre_ = nodeToDelete->pre_;
    } else {
      tail_ = nodeToDelete->pre_;
    }

    delete nodeToDelete;
    size_--;
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
    return (std::numeric_limits<size_type>::max() / sizeof(value_type));
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
  bool operator<=(const list& other) const {
    if (size_ < other.size_) {
      return true;
    } else if (size_ > other.size_) {
      return false;
    }

    Node* current1 = head_;
    Node* current2 = other.head_;

    while (current1 != nullptr && current2 != nullptr) {
      if (current1->data_ > current2->data_) {
        return false;
      }
      current1 = current1->next_;
      current2 = current2->next_;
    }

    return true;
  }

  list& operator=(const list& other) {
    if (this != &other) {
      clear();

      Node* current = other.head_;
      while (current != nullptr) {
        push_back(current->data_);
        current = current->next_;
      }
    }
    return *this;
  }
  bool operator==(const std::list<T>& first) {
    if (first.size() != this->size()) {
      return false;
    }

    auto it1 = first.begin();
    auto it2 = this->begin();

    while (it1 != first.end() && it2 != this->end()) {
      if (*it1 != *it2) {
        return false;
      }
      ++it1;
      ++it2;
    }

    return true;
  }
  /////////////////////////
  iterator begin() { return iterator(head_); }
  iterator end() { return nullptr; }
  /////////////////////////

  int get_size() const;

  void clear() {
    Node* current = head_;
    while (current != nullptr) {
      Node* nextNode = current->next_;
      delete current;
      current = nextNode;
    }
    head_ = nullptr;
    tail_ = nullptr;
    size_ = 0;
  }

  ~list() { clear(); };
  ///////////////////////////////////
 private:
  Node* head_;
  Node* tail_;
  size_type size_;
};
template <typename T>
class ListIterator {
 public:
  using iterator_category = std::forward_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using reference = T&;
  using value_type = T;
  using pointer = T*;
  using Node = typename list<T>::Node;

  ListIterator(Node* node) : current_(node) {}

  reference operator*() { return current_->data_; }

  pointer operator->() { return &current_->data_; }
  Node* getCurrent() const { return current_; }

  ListIterator& operator++() {
    current_ = current_->next_;
    return *this;
  }

  ListIterator operator++(int) {
    ListIterator temp = *this;
    ++(*this);
    return temp;
  }

  ListIterator& operator--() {
    if (current_ != nullptr) {
      current_ = current_->pre_;
    }
    return *this;
  }

  ListIterator operator--(int) {
    ListIterator temp = *this;
    --(*this);
    return temp;
  }
  ListIterator operator+(int n) const {
    ListIterator temp = *this;
    for (int i = 0; i < n && temp.current_ != nullptr; ++i) {
      temp++;
    }
    return temp;
  }
  ListIterator& operator+=(int n) {
    if (n >= 0) {
      for (int i = 0; i < n && current_ != nullptr; ++i) {
        current_ = current_->next_;
      }
    } else {
      for (int i = 0; i < -n && current_ != nullptr; ++i) {
        current_ = current_->pre_;
      }
    }
    return *this;
  }
  ListIterator operator-(int n) const {
    ListIterator temp = *this;
    for (int i = 0; i < n && temp.current_ != nullptr; ++i) {
      temp.current_ = temp.current_->pre_;
    }
    return temp;
  }
  ListIterator& operator-=(int n) { return *this += -n; }
  reference operator[](int n) {
    ListIterator temp = *this;
    for (int i = 0; i < n && temp.current_ != nullptr; ++i) {
      temp.current_ = temp.current_->next_;
    }
    if (temp.current_ == nullptr) {
      throw std::out_of_range("Index out of range");
    }
    return *temp;
  }
  bool operator==(const ListIterator& other) const {
    return current_ == other.current_;
  }
  bool operator!=(const ListIterator& other) const { return !(*this == other); }

  ~ListIterator() = default;

 private:
  Node* current_;
};

}  // namespace s21

#endif  // S21_LIST_H
