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
    value_type data[deque::kChunkSize];
  };

  deque() = default;
  ~deque() = default;

  // at();
  // push_front(value);
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
  // operator[];

 private:
  static constexpr size_type kPageSize{4096};
  static constexpr size_type kChunkSize{kPageSize / sizeof(value_type)};
  static constexpr size_type kInitialMapSize = 8;

  Chunk** map_;               // Pointer to array of chunk pointers
  size_type map_size_;        // Current size of the map array
  size_type front_chunk_;     // Index of the first chunk in map
  size_type back_chunk_;      // Index of the last chunk in map
  size_type front_position_;  // Position in first chunk
  size_type back_position_;   // Position in last chunk
  size_type size_;            // Total number of elements
};
}  // namespace s21

#endif  // S21_DEQUE_H