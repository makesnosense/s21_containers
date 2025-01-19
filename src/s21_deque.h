#include "s21_base.h"

#ifndef S21_DEQUE_H
#define S21_DEQUE_H

namespace s21 {
#if 0
template <typename T>
class DequeIterator {
 public:
  using iterator_category = std::random_access_iterator_tag;
  using value_type = T;
  using pointer = T*;
  using reference = T&;
  using difference_type = std::ptrdiff_t;

  explicit DequeIterator(pointer ptr);

  reference operator*();
  reference operator[](difference_type n);

  DequeIterator& operator++();

  DequeIterator operator++(int);

  DequeIterator& operator--();
  DequeIterator operator--(int);

  DequeIterator& operator+=(difference_type n);

  DequeIterator operator+(difference_type n);

  friend DequeIterator operator+(difference_type n,
                                  const DequeIterator& iter);

  DequeIterator& operator-=(difference_type n);

  difference_type operator-(const DequeIterator& other);

  DequeIterator operator-(difference_type n);

  bool operator==(const DequeIterator& other);
  bool operator!=(const DequeIterator& other);

  bool operator<(const DequeIterator& other);

  bool operator>(const DequeIterator& other);

  bool operator<=(const DequeIterator& other);

  bool operator>=(const DequeIterator& other);

 private:
  pointer ptr_;
};
#endif

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

  // using iterator = DequeIterator<T>;

  // using const_iterator = const DequeIterator<T>;
  // using reverse_iterator = DequeIterator<T>;
  // using const_reverse_iterator = const DequeIterator<T>;

  struct Chunk {
    value_type data_[deque::kChunkSize];
  };

  deque() : map_{nullptr} {
    map_ = new Chunk* [kInitialMapSize] {};
    map_size_ = kInitialMapSize;
    front_chunk_index_ = back_chunk_index_ = map_size_ / 2;
    front_element_index_ = back_element_index_ = kChunkSize / 2;

    AddChunkAt(front_chunk_index_);
  }
  ~deque() {
    for (size_type i{front_chunk_index_}; i <= back_chunk_index_; ++i) {
      delete map_[i];
    }

    delete[] map_;
  };
  deque(const deque& other) = default;
  deque& operator=(const deque& other) = default;
  void push_front(const_reference value) {
    if (front_element_index_ == 0) {
      if (front_chunk_index_ == 0) {
        // Handle reallocation/map growth
        // or throw if we can't grow
        return;
      }

      if (map_[front_chunk_index_ - 1] == nullptr) {
        --front_chunk_index_;
        AddChunkAt(front_chunk_index_);
      }
      front_element_index_ = kChunkSize;  // we will decrement it right below;
    }

    --front_element_index_;

    std::cout << "front_chunk: " << front_chunk_index_ << '\n'
              << "front_element_index_: " << front_element_index_ << '\n'
              << "value: " << value << '\n';
    map_[front_chunk_index_]->data_[front_element_index_] = value;

    ++size_;
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

  // Chunk** map_;               // Pointer to array of chunk pointers
  // size_type map_size_;        // Current size of the map array
  // size_type front_chunk_;     // Index of the first chunk in map
  // size_type back_chunk_;      // Index of the last chunk in map
  // size_type front_position_;  // Position in first chunk
  // size_type back_position_;   // Position in last chunk
  // size_type size_;            // Total number of elements

  // at();
  // push_back(value);
  // pop_front();
  // pop_back();
  // resize();
  // insert();
  // erase();
  // iterator begin();
  // iterator end();
  // reverse_iterator rbegin();
  // reverse_iterator rend();
  // clear();
  // size_type size();
  // shrink_to_fit();
  // front();
  // back();

  size_type size() const { return size_; }

  reference operator[](size_type index) {
    size_type chunk_offset{(index + front_element_index_) / kChunkSize};

    size_type chunk_index = front_chunk_index_ + chunk_offset;

    size_type element_index_in_chunk{(front_element_index_ + index) %
                                     kChunkSize};

    return map_[chunk_index]->data_[element_index_in_chunk];
  }

  // size_type get_from_chunk_index() const { return front_chunk_index_; }
  // size_type get_back_chunk_index() const { return back_chunk_index_; }
  // size_type get_front_element_index() const { return front_element_index_; }
  // size_type get_back_element_index() const { return back_element_index_; }
  // size_type get_chunk_size() const { return kChunkSize; }
  // size_type get_map_size() const { return map_size_; }
  // size_type get_element(const size_type index) const {
  //   return map_[front_chunk_index]->data_[front_element_index];
  // }

  // get_element_by_chunk_and_element_index(size_type chunk_index, size_type
  // element_index) {

  // }

 private:
  // static constexpr size_type kPageSize{4096};
  // static constexpr size_type kChunkSize{kPageSize / sizeof(value_type)};
  // static constexpr size_type kChunkSize{64 * 2 / sizeof(T)};
  static constexpr size_type kChunkSize{10};
  static constexpr size_type kInitialMapSize{10};

  Chunk** map_{nullptr};              // Pointer to array of chunk pointers
  size_type map_size_{0};             // Current size of the map array
  size_type front_chunk_index_{0};    // Index of the first chunk in map
  size_type back_chunk_index_{0};     // Index of the last chunk in map
  size_type front_element_index_{0};  // Index within front chunk
  size_type back_element_index_{0};   // Index within back chunk
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
