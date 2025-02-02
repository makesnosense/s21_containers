
#ifndef S21_LIST_H
#define S21_LIST_H

#include <initializer_list>
#include <iostream>
#include <iterator>
#include <stdexcept>

namespace s21 {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
template <typename T>
struct Node {
  using value_type = T;
  alignas(alignof(value_type)) value_type data_;
  Node* next_;
  Node* pre_;

  Node() : data_(T()), next_(nullptr), pre_(nullptr) {}
  Node(value_type val) : data_(val), next_(nullptr), pre_(nullptr) {}
  Node(const Node&) = default;
  Node& operator=(const Node&) = default;
  ~Node() = default;
};
#pragma GCC diagnostic pop

template <typename T, bool is_const>
class ListIterator;
template <typename T>
class list {
 public:
  friend struct Node<T>;
  friend class ListIterator<T, false>;
  friend class ListIterator<T, true>;

  using value_type = T;
  using reference = T&;
  using const_reference = const T&;

  using pointer = T*;

  using iterator = ListIterator<T, false>;
  using node_type = Node<T>;
  using const_iterator = const ListIterator<T, true>;
  using size_type = std::size_t;

  list() : head_{nullptr}, tail_{nullptr}, size_{0} {}
  explicit list(std::initializer_list<value_type> init)
      : head_{nullptr}, tail_{nullptr}, size_{0} {
    for (const T& value : init) {
      node_type* new_node = new node_type(value);
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
      node_type* new_node = new node_type();
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
  list(const list& other) : head_(nullptr), tail_(nullptr), size_(0) {
    if (other.head_ != nullptr) {
      node_type* current = other.head_;
      while (current != nullptr) {
        push_back(current->data_);
        current = current->next_;
      }
    }
  }
  list(list&& other) noexcept
      : head_(other.head_), tail_(other.tail_), size_(other.size_) {
    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.size_ = 0;
  }
  explicit list(size_type n, const value_type& value)
      : head_{nullptr}, tail_{nullptr}, size_{0} {
    for (size_type i = 0; i < n; ++i) {
      node_type* new_node = new node_type(value);
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
  std::pair<node_type*, node_type*> Split(node_type* head) {
    node_type* slow = head;
    node_type* fast = head->next_;

    while (fast != nullptr && fast->next_ != nullptr) {
      slow = slow->next_;
      fast = fast->next_->next_;
    }

    node_type* second_half = slow->next_;
    slow->next_ = nullptr;
    if (second_half != nullptr) {
      second_half->pre_ = nullptr;
    }

    return {head, second_half};
  }

  node_type* MergeForSort(node_type* first, node_type* second) {
    if (first == nullptr) return second;
    if (second == nullptr) return first;

    node_type* merged_head = nullptr;

    if (first->data_ <= second->data_) {
      merged_head = first;
      first = first->next_;
    } else {
      merged_head = second;
      second = second->next_;
    }

    node_type* current = merged_head;

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

    return merged_head;
  }

  node_type* MergeSort(node_type* head) {
    if (head == nullptr || head->next_ == nullptr) {
      return head;
    }

    auto [first_half, second_half] = Split(head);

    first_half = MergeSort(first_half);
    second_half = MergeSort(second_half);

    return MergeForSort(first_half, second_half);
  }
  iterator insert(iterator pos, const_reference value) {
    node_type* new_node = new node_type(value);

    if (pos.GetCurrent() == nullptr) {
      if (tail_ != nullptr) {
        tail_->next_ = new_node;
        new_node->pre_ = tail_;
        tail_ = new_node;
      } else {
        head_ = new_node;
        tail_ = new_node;
      }
    } else {
      node_type* current_node = pos.GetCurrent();

      if (current_node->pre_ != nullptr) {
        current_node->pre_->next_ = new_node;
        new_node->pre_ = current_node->pre_;
      } else {
        head_ = new_node;
      }

      new_node->next_ = current_node;
      current_node->pre_ = new_node;
    }

    size_++;
    return iterator(new_node);
  }
  template <typename... Args>
  iterator insert_many(const_iterator pos, Args&&... args) {
    list<T> temp_list{std::forward<Args>(args)...};

    if (temp_list.empty()) {
      return iterator(const_cast<node_type*>(pos.GetCurrent()));
    }

    node_type* pos_node = const_cast<node_type*>(pos.GetCurrent());
    node_type* new_head = temp_list.head_;
    node_type* new_tail = temp_list.tail_;

    if (pos_node) {
      if (pos_node->pre_) {
        pos_node->pre_->next_ = new_head;
        new_head->pre_ = pos_node->pre_;
      } else {
        head_ = new_head;
      }
      new_tail->next_ = pos_node;
      pos_node->pre_ = new_tail;
    } else {
      tail_->next_ = new_head;
      new_head->pre_ = tail_;
      tail_ = new_tail;
    }

    size_ += temp_list.size_;
    temp_list.head_ = nullptr;
    temp_list.tail_ = nullptr;
    temp_list.size_ = 0;

    return iterator(new_head);
  }
  template <typename... Args>
  void insert_many_back(Args&&... args) {
    list<T> temp_list{std::forward<Args>(args)...};

    if (temp_list.empty()) {
      return;
    }

    if (empty()) {
      head_ = temp_list.head_;
      tail_ = temp_list.tail_;
    } else {
      tail_->next_ = temp_list.head_;
      temp_list.head_->pre_ = tail_;

      tail_ = temp_list.tail_;
    }

    size_ += temp_list.size_;

    temp_list.head_ = nullptr;
    temp_list.tail_ = nullptr;
    temp_list.size_ = 0;
  }
  template <typename... Args>
  void insert_many_front(Args&&... args) {
    list<T> temp_list{std::forward<Args>(args)...};

    if (temp_list.empty()) {
      return;
    }

    if (empty()) {
      head_ = temp_list.head_;
      tail_ = temp_list.tail_;
    } else {
      temp_list.tail_->next_ = head_;
      head_->pre_ = temp_list.tail_;
      head_ = temp_list.head_;
    }

    size_ += temp_list.size_;

    temp_list.head_ = nullptr;
    temp_list.tail_ = nullptr;
    temp_list.size_ = 0;
  }
  void splice(const_iterator pos, list& other) {
    if (other.empty()) return;

    node_type* pos_node = const_cast<node_type*>(pos.GetCurrent());
    node_type* other_head = other.head_;
    node_type* other_tail = other.tail_;

    if (pos_node == head_) {
      head_ = other_head;
    } else {
      pos_node->pre_->next_ = other_head;
      other_head->pre_ = pos_node->pre_;
    }

    if (pos_node != nullptr) {
      other_tail->next_ = pos_node;
      pos_node->pre_ = other_tail;
    } else {
      tail_ = other_tail;
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

    node_type* current1 = head_;
    node_type* current2 = other.head_;
    node_type* merged_head = nullptr;
    node_type* merged_tail = nullptr;

    if (current1->data_ <= current2->data_) {
      merged_head = current1;
      current1 = current1->next_;
    } else {
      merged_head = current2;
      current2 = current2->next_;
    }

    merged_tail = merged_head;

    while (current1 != nullptr && current2 != nullptr) {
      if (current1->data_ <= current2->data_) {
        merged_tail->next_ = current1;
        current1->pre_ = merged_tail;
        merged_tail = current1;
        current1 = current1->next_;
      } else {
        merged_tail->next_ = current2;
        current2->pre_ = merged_tail;
        merged_tail = current2;
        current2 = current2->next_;
      }
    }

    if (current1 != nullptr) {
      merged_tail->next_ = current1;
      current1->pre_ = merged_tail;
    } else if (current2 != nullptr) {
      merged_tail->next_ = current2;
      current2->pre_ = merged_tail;
    }

    head_ = merged_head;
    tail_ = merged_tail;
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

    node_type* current = head_;

    while (current != nullptr) {
      node_type* next_node = current->next_;

      while (next_node != nullptr && next_node->data_ == current->data_) {
        node_type* duplicate_node = next_node;
        next_node = next_node->next_;
        size_--;

        delete duplicate_node;
      }

      current->next_ = next_node;
      if (next_node != nullptr) {
        next_node->pre_ = current;
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

    head_ = MergeSort(head_);

    tail_ = head_;
    while (tail_->next_ != nullptr) {
      tail_ = tail_->next_;
    }
  }
  void erase(iterator pos) {
    if (pos.GetCurrent() == nullptr) return;

    node_type* node_to_delete = pos.GetCurrent();

    if (node_to_delete->pre_) {
      node_to_delete->pre_->next_ = node_to_delete->next_;
    } else {
      head_ = node_to_delete->next_;
    }

    if (node_to_delete->next_) {
      node_to_delete->next_->pre_ = node_to_delete->pre_;
    } else {
      tail_ = node_to_delete->pre_;
    }

    delete node_to_delete;
    size_--;
  }

  void pop_back() {
    if (empty()) {
      throw std::logic_error("List is empty");
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
    node_type* new_node = new node_type(value);
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
    node_type* new_node = new node_type(value);
    if (empty()) {
      head_ = new_node;
      tail_ = new_node;
    } else {
      new_node->pre_ = tail_;
      tail_->next_ = new_node;
      out_of_range
    }

    size_++;
  }

  void reverse() {
    if (size_ <= 1) return;

    node_type* current = head_;
    node_type* prev = nullptr;
    node_type* next = nullptr;

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
      throw std::logic_error("List is empty");
    }

    if (head_ == tail_) {
      delete head_;
      head_ = nullptr;
      tail_ = nullptr;
      size_--;
      return;
    }
    head_ = head_->next_;
    delete head_->pre_;
    head_->pre_ = nullptr;
  }

  size_type max_size() const noexcept {
    return (std::numeric_limits<size_type>::max() / sizeof(value_type));
  }

  reference GetElement(size_type index) {
    if (index >= size_) {
      throw std::out_of_range("Index out of range");
    }
    node_type* current = head_;
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
    node_type* current = head_;
    for (size_type i = 0; i < index; ++i) {
      current = current->next_;
    }
    return current->data_;
  }

  const T& operator[](size_type index) const {
    if (index >= size_) {
      throw std::out_of_range("Index out of range");
    }
    node_type* current = head_;
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

    node_type* current1 = head_;
    node_type* current2 = other.head_;

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

      node_type* current = other.head_;
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

  iterator begin() { return iterator(head_); }
  iterator end() { return nullptr; }

  int get_size() const;

  void clear() {
    node_type* current = head_;
    while (current != nullptr) {
      node_type* next_node = current->next_;
      delete current;
      current = next_node;
    }
    head_ = nullptr;
    tail_ = nullptr;
    size_ = 0;
  }

  ~list() { clear(); };

 private:
  node_type* head_;
  node_type* tail_;
  size_type size_;
};
template <typename T, bool is_const>
class ListIterator {
 public:
  using iterator_category = std::forward_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using reference = std::conditional_t<is_const, const T&, T&>;
  using value_type = T;
  using pointer = std::conditional_t<is_const, const T*, T*>;
  using node_type = Node<T>;
  using node_pointer =
      std::conditional_t<is_const, const node_type*, node_type*>;
  template <typename U, bool other_is_const>
  friend class ListIterator;
  ListIterator() = default;
  ListIterator(node_pointer node) : current_(node) {}
  template <bool other_is_const,
            typename = std::enable_if_t<is_const || !other_is_const>>
  ListIterator(const ListIterator<T, other_is_const>& other)
      : current_(other.current_) {}

  reference operator*() { return current_->data_; }

  pointer operator->() { return &current_->data_; }
  node_pointer GetCurrent() const { return current_; }

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
  difference_type operator-(const ListIterator& other) const {
    if (current_ == other.current_) {
      return 0;
    }

    difference_type distance = 0;
    ListIterator temp = other;

    while (temp != *this) {
      ++distance;
      ++temp;
    }

    return distance;
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
  node_pointer current_;
};

}  // namespace s21

#endif  // S21_LIST_H
