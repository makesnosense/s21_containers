#include "s21_base.h"

#ifndef S21_DEQUE_H
#define S21_DEQUE_H

namespace s21 {
template <typename T>
class DequeIterator {
 public:
  using iterator_category = std::random_access_iterator_tag;
  using value_type = T;
  using pointer = T*;
  using reference = T&;
  using difference_type = std::ptrdiff_t;

  DequeIterator() : ptr_(nullptr) {}
  explicit DequeIterator(pointer ptr) : ptr_{ptr} {}
  operator pointer() const { return ptr_; }

  reference operator*() { return *ptr_; }
  reference operator[](difference_type n) { return ptr_[n]; }

  DequeIterator& operator++() {
    ++ptr_;
    return *this;
  }

  DequeIterator operator++(int) {
    DequeIterator tmp = *this;
    ++ptr_;
    return tmp;
  }

  DequeIterator& operator--() {
    --ptr_;
    return *this;
  }
  DequeIterator operator--(int) {
    DequeIterator tmp = *this;
    --ptr_;
    return tmp;
  }

  DequeIterator& operator+=(difference_type n) {
    ptr_ += n;
    return *this;
  }

  DequeIterator operator+(difference_type n) const {
    return DequeIterator(ptr_ + n);
  }

  friend DequeIterator operator+(difference_type n, const DequeIterator& iter) {
    return DequeIterator(iter.ptr_ + n);
  }

  DequeIterator& operator-=(difference_type n) {
    ptr_ -= n;
    return *this;
  }

  difference_type operator-(const DequeIterator& other) const {
    return ptr_ - other.ptr_;
  }

  DequeIterator operator-(difference_type n) const {
    return DequeIterator(ptr_ - n);
  }

  bool operator==(const DequeIterator& other) const {
    return ptr_ == other.ptr_;
  }
  bool operator!=(const DequeIterator& other) const {
    return ptr_ != other.ptr_;
  }

  bool operator<(const DequeIterator& other) const { return ptr_ < other.ptr_; }

  bool operator>(const DequeIterator& other) const { return ptr_ > other.ptr_; }

  bool operator<=(const DequeIterator& other) const {
    return ptr_ <= other.ptr_;
  }

  bool operator>=(const DequeIterator& other) const {
    return ptr_ >= other.ptr_;
  }

 private:
  pointer ptr_;
};

template <typename T>
class deque {
 public:
  using traits = container_traits<T>;
  using value_type = typename traits::value_type;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = typename traits::reference;
  using const_reference = typename traits::const_reference;
  using size_type = typename traits::size_type;
  using const_iterator = const DequeIterator<T>;
  using reverse_iterator = DequeIterator<T>;
  using const_reverse_iterator = const DequeIterator<T>;

 private:
  // static constexpr size_type kPageSize{4096};
  // static constexpr size_type kChunkSize{kPageSize / sizeof(value_type)};
  // static constexpr size_type kChunkSize{64 * 2 / sizeof(T)};

  static constexpr size_type kChunkSize{10};
  static constexpr size_type kInitialMapSize{10};

 public:
  using iterator = DequeIterator<T>;

  struct Chunk {
    value_type data_[deque::kChunkSize];
  };

  deque() : map_{nullptr} {
    map_ = new Chunk* [kInitialMapSize] {};
    map_size_ = kInitialMapSize;
    front_chunk_index_ = back_chunk_index_ = map_size_ / 2;
    front_element_index_ = back_vacant_index_ = kChunkSize / 2;

    AddChunkAt(front_chunk_index_);
  }

  deque(const deque& other) = default;

  // deque(deque&& other) noexcept
  //     : data_{other.data_}, size_{other.size_}, capacity_{other.capacity_} {
  //   other.data_ = nullptr;
  //   other.size_ = 0;
  //   other.capacity_ = 0;
  // }

  explicit deque(std::initializer_list<value_type> init) : deque() {
    for (const auto& i : init) {
      push_back(i);
    }
  }

  ~deque() {
    for (size_type i{front_chunk_index_}; i <= back_chunk_index_; ++i) {
      delete map_[i];
    }

    delete[] map_;
  };

  deque& operator=(const deque& other) = default;

  void push_front(const_reference value) {
    if (front_element_index_ == 0) {
      if (front_chunk_index_ == 0) {
        GrowMap(true);
      }

      if (map_[front_chunk_index_ - 1] == nullptr) {
        --front_chunk_index_;
        AddChunkAt(front_chunk_index_);
      }
      front_element_index_ = kChunkSize;
    }

    --front_element_index_;

    // std::cout << "front_chunk: " << front_chunk_index_ << '\n'
    //           << "front_element_index_: " << front_element_index_ << '\n'
    //           << "value: " << value << '\n';
    map_[front_chunk_index_]->data_[front_element_index_] = value;

    ++size_;
  }

  void push_back(const_reference value) {
    if (back_vacant_index_ == 0) {
      if (back_chunk_index_ == map_size_ - 1) {
        GrowMap(false);
      }

      if (map_[back_chunk_index_ + 1] == nullptr) {
        ++back_chunk_index_;
        AddChunkAt(back_chunk_index_);
      }
      back_vacant_index_ = 0;
    }
    // std::cout << "back_chunk: " << back_chunk_index_ << '\n'
    //           << "back_vacant_index_: " << back_vacant_index_ << '\n'
    //           << "value: " << value << '\n';
    map_[back_chunk_index_]->data_[back_vacant_index_] = value;

    ++back_vacant_index_;

    back_vacant_index_ %= kChunkSize;

    ++size_;
  }

  void GrowMap(bool to_the_front) {
    size_type new_size_map{map_size_ * 2};
    Chunk** new_map = new Chunk* [new_size_map] {};

    size_type new_front_chunk_index{};
    size_type new_back_chunk_index{};

    if (to_the_front == true) {
      new_front_chunk_index = map_size_;
      new_back_chunk_index = map_size_ + back_chunk_index_;

      std::move(map_, map_ + map_size_, new_map + map_size_);

    } else {
      new_front_chunk_index = front_chunk_index_;
      new_back_chunk_index = back_chunk_index_;

      std::move(map_, map_ + map_size_, new_map);
    }
    delete[] map_;
    front_chunk_index_ = new_front_chunk_index;
    back_chunk_index_ = new_back_chunk_index;
    map_size_ = new_size_map;
    map_ = new_map;
  }

  void AddChunkAt(size_type chunk_index) { map_[chunk_index] = new Chunk(); }
  // void AddChunkToFront() {
  //   --front_chunk_index_;
  //   AddChunkAt(front_chunk_index_);
  // }
  // void AddChunkToBack() {
  //   ++back_chunk_index_;
  //   AddChunkAt(front_chunk_index_);
  // }

  reference at(size_type position) {
    if (position >= size_ || position < 0) {
      throw std::out_of_range("Vector index out of range");
    }
    size_type chunk_offset{(position + front_element_index_) / kChunkSize};
    size_type chunk_index = front_chunk_index_ + chunk_offset;
    size_type element_index_in_chunk{(front_element_index_ + position) %
                                     kChunkSize};

    return map_[chunk_index]->data_[element_index_in_chunk];
  }
  void pop_front() {
    if (size_ > 0) {
      --size_;
      map_[front_chunk_index_]->data_[front_element_index_].~value_type();
      if (front_element_index_ == kChunkSize - 1) {
        front_element_index_ = 0;
        ++front_chunk_index_;
      } else {
        ++front_element_index_;
      }
    }
  }
  void pop_back() {
    if (size_ > 0) {
      --size_;
      if (back_vacant_index_ == 0) {
        back_vacant_index_ = kChunkSize - 1;
        --back_chunk_index_;
      } else {
        --back_vacant_index_;
      }
      map_[back_chunk_index_]->data_[back_vacant_index_].~value_type();
    }
  }

  size_type size() const { return size_; }
  bool empty() {
    if (size() == 0) {
      return true;
    }
    return false;
  }

  // clear();
  // shrink_to_fit();
  // void swap(queue& other);

  iterator begin() {
    return iterator(map_[front_chunk_index_]->data_ + front_element_index_);
  }

  iterator end() {
    return iterator(map_[back_chunk_index_]->data_ + back_vacant_index_ - 1);
  }

  reference front() {
    return map_[front_chunk_index_]->data_[front_element_index_];
  }

  reference back() {
    if (back_vacant_index_ == 0) {
      return map_[back_chunk_index_]->data_[kChunkSize - 1];
    } else {
      return map_[back_chunk_index_]->data_[back_vacant_index_ - 1];
    }
  }

  reference operator[](size_type index) {
    size_type chunk_offset{(index + front_element_index_) / kChunkSize};

    size_type chunk_index = front_chunk_index_ + chunk_offset;

    size_type element_index_in_chunk{(front_element_index_ + index) %
                                     kChunkSize};

    return map_[chunk_index]->data_[element_index_in_chunk];
  }

 private:
  Chunk** map_{nullptr};              // Pointer to array of chunk pointers
  size_type map_size_{0};             // Current size of the map array
  size_type front_chunk_index_{0};    // Index of the first chunk in map
  size_type back_chunk_index_{0};     // Index of the last chunk in map
  size_type front_element_index_{0};  // Index within front chunk
  size_type back_vacant_index_{0};    // Index within back chunk
  size_type size_{0};                 // Total number of elements
};

template <typename T>
std::ostream& operator<<(std::ostream& out, deque<T>& q) {
  for (size_t index{0}; index < q.size(); ++index) {
    out << q[index] << ' ';
  }

  return out;
}

}  // namespace s21

#endif  // S21_DEQUE_H
