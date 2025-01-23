#include "s21_base.h"

#ifndef S21_DEQUE_H
#define S21_DEQUE_H

namespace s21 {

template <typename T, bool is_const>
class DequeIterator;

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

  using iterator = DequeIterator<T, false>;
  using const_iterator = DequeIterator<T, true>;

  friend class DequeIterator<T, true>;
  friend class DequeIterator<T, false>;

 private:
  static constexpr size_type kPageSize{4096};
  static constexpr size_type kChunkSize{kPageSize / sizeof(value_type)};
  static constexpr size_type kInitialMapSize{8};

 public:
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

  deque(size_type n, const value_type& value) : deque() {
    for (size_type i{0}; i < n; i++) {
      push_back(value);
    }
  }

  deque(const deque& other)
      : map_{nullptr},
        map_size_{other.map_size_},
        front_chunk_index_{other.front_chunk_index_},
        back_chunk_index_{other.back_chunk_index_},
        front_element_index_{other.front_element_index_},
        back_vacant_index_{other.back_vacant_index_},
        size_{other.size_} {
    map_ = new Chunk* [map_size_] {};

    for (size_type i = front_chunk_index_; i <= back_chunk_index_; ++i) {
      if (other.map_[i]) {
        map_[i] = new Chunk();
        std::copy(std::begin(other.map_[i]->data_),
                  std::end(other.map_[i]->data_), std::begin(map_[i]->data_));
      }
    }
  }

  deque(deque&& other) noexcept
      : map_{nullptr},
        map_size_{0},
        front_chunk_index_{0},
        back_chunk_index_{0},
        front_element_index_{0},
        back_vacant_index_{0},
        size_{0} {
    map_ = other.map_;
    map_size_ = other.map_size_;
    front_chunk_index_ = other.front_chunk_index_;
    back_chunk_index_ = other.back_chunk_index_;
    front_element_index_ = other.front_element_index_;
    back_vacant_index_ = other.back_vacant_index_;
    size_ = other.size_;

    other.map_ = nullptr;
    other.map_size_ = 0;
    other.front_chunk_index_ = 0;
    other.back_chunk_index_ = 0;
    other.front_element_index_ = 0;
    other.back_vacant_index_ = 0;
    other.size_ = 0;
  }

  explicit deque(std::initializer_list<value_type> init) : deque() {
    for (const auto& i : init) {
      push_back(i);
    }
  }

  ~deque() {
    for (size_type i{0}; i < map_size_; ++i) {
      delete map_[i];
    }

    delete[] map_;
  };

  deque& operator=(deque&& other) noexcept {
    if (this != &other) {
      for (size_type i = 0; i < map_size_; ++i) {
        delete map_[i];
      }
      delete[] map_;

      map_ = other.map_;
      map_size_ = other.map_size_;
      front_chunk_index_ = other.front_chunk_index_;
      back_chunk_index_ = other.back_chunk_index_;
      front_element_index_ = other.front_element_index_;
      back_vacant_index_ = other.back_vacant_index_;
      size_ = other.size_;

      other.map_ = nullptr;
      other.map_size_ = 0;
      other.front_chunk_index_ = 0;
      other.back_chunk_index_ = 0;
      other.front_element_index_ = 0;
      other.back_vacant_index_ = 0;
      other.size_ = 0;
    }
    return *this;
  }

  deque& operator=(const deque& other) {
    if (this != &other) {
      for (size_type i = 0; i < map_size_; ++i) {
        delete map_[i];
      }
      delete[] map_;

      map_size_ = other.map_size_;
      front_chunk_index_ = other.front_chunk_index_;
      back_chunk_index_ = other.back_chunk_index_;
      front_element_index_ = other.front_element_index_;
      back_vacant_index_ = other.back_vacant_index_;
      size_ = other.size_;

      map_ = new Chunk* [map_size_] {};
      for (size_type i = 0; i < map_size_; ++i) {
        if (other.map_[i]) {
          map_[i] = new Chunk(*other.map_[i]);
        }
      }
    }
    return *this;
  }

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
      if (back_chunk_index_ == map_size_) {
        GrowMap(false);
      }

      if (map_[back_chunk_index_] == nullptr) {
        // ++back_chunk_index_;
        AddChunkAt(back_chunk_index_);
      }
      back_vacant_index_ = 0;
    }

    map_[back_chunk_index_]->data_[back_vacant_index_] = value;

    ++back_vacant_index_;

    back_vacant_index_ %= kChunkSize;
    if (back_vacant_index_ == 0) ++back_chunk_index_;

    ++size_;
  }

  reference at(size_type position) {
    if (position >= size_) {
      throw std::out_of_range("Deque index out of range");
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
      if constexpr (std::is_trivially_destructible_v<value_type> == false) {
        map_[back_chunk_index_]->data_[back_vacant_index_].~value_type();
      }
    }
  }

  size_type size() const { return size_; }

  bool empty() {
    if (size() == 0) {
      return true;
    }
    return false;
  }

  void swap(deque& other) {
    std::swap(other.map_, map_);
    std::swap(other.map_size_, map_size_);
    std::swap(other.front_chunk_index_, front_chunk_index_);
    std::swap(other.back_chunk_index_, back_chunk_index_);
    std::swap(other.front_element_index_, front_element_index_);
    std::swap(other.back_vacant_index_, back_vacant_index_);
    std::swap(other.size_, size_);
  }

  iterator begin() {
    return iterator(this, front_chunk_index_, front_element_index_);
  }

  iterator end() {
    return iterator(this, back_chunk_index_, back_vacant_index_);
  }

  const_iterator begin() const {
    return const_iterator(this, front_chunk_index_, front_element_index_);
  }

  const_iterator end() const {
    return const_iterator(this, back_chunk_index_, back_vacant_index_);
  }

  const_reference front() const {
    return map_[front_chunk_index_]->data_[front_element_index_];
  }

  reference front() {
    return const_cast<reference>(static_cast<const deque&>(*this).front());
  }

  const_reference back() const {
    if (back_vacant_index_ == 0) {
      return map_[back_chunk_index_ - 1]->data_[kChunkSize - 1];
    }
    return map_[back_chunk_index_]->data_[back_vacant_index_ - 1];
  }

  reference back() {
    return const_cast<reference>(static_cast<const deque&>(*this).back());
  }

  const_reference operator[](size_type index) const {
    size_type chunk_offset{(index + front_element_index_) / kChunkSize};

    size_type chunk_index = front_chunk_index_ + chunk_offset;

    size_type element_index_in_chunk{(front_element_index_ + index) %
                                     kChunkSize};

    return map_[chunk_index]->data_[element_index_in_chunk];
  }

  reference operator[](size_type index) {
    return const_cast<reference>(static_cast<const deque&>(*this)[index]);
  }

 private:
  Chunk** map_{nullptr};              // Pointer to array of chunk pointers
  size_type map_size_{0};             // Current size of the map array
  size_type front_chunk_index_{0};    // Index of the first chunk in map
  size_type back_chunk_index_{0};     // Index of the last chunk in map
  size_type front_element_index_{0};  // Index within front chunk
  size_type back_vacant_index_{0};    // Index within back chunk
  size_type size_{0};                 // Total number of elements

  void AddChunkAt(size_type chunk_index) { map_[chunk_index] = new Chunk(); }

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
};

template <typename T, bool is_const>
class DequeIterator {
 public:
  using iterator_category = std::random_access_iterator_tag;
  using size_type = typename deque<T>::size_type;
  using value_type = T;
  using pointer = std::conditional_t<is_const, const T*, T*>;
  using reference = std::conditional_t<is_const, const T&, T&>;
  using difference_type = std::ptrdiff_t;

  template <typename U, bool other_is_const>
  friend class DequeIterator;
  /////////
  using container_type = std::conditional_t<is_const, const deque<T>, deque<T>>;
  using container_pointer = container_type*;
  /////////

  DequeIterator() = default;

  DequeIterator(container_pointer container, size_type current_chunk,
                size_type current_element)
      : container_{container},
        current_chunk_(current_chunk),
        current_element_(current_element) {}

  template <bool other_is_const,
            typename = std::enable_if_t<(is_const || !other_is_const)>>
  DequeIterator(const DequeIterator<T, other_is_const>& other)
      : container_{other.container_},
        current_chunk_(other.current_chunk_),
        current_element_(other.current_element_) {}

  operator pointer() const {
    return &(container_->map_[current_chunk_]->data_[current_element_]);
  }

  reference operator*() {
    return container_->map_[current_chunk_]->data_[current_element_];
  }

  reference operator*() const {
    return container_->map_[current_chunk_]->data_[current_element_];
  }

  reference operator[](difference_type n) const {
    size_type chunk_offset{(n + current_element_) / container_->kChunkSize};
    size_type element_index{(n + current_element_) % container_->kChunkSize};
    return container_->map_[current_chunk_ + chunk_offset]
        ->data_[element_index];
  }

  DequeIterator& operator++() {
    ++current_element_;
    current_element_ %= container_->kChunkSize;
    if (current_element_ == 0) {
      ++current_chunk_;
    };

    return *this;
  }

  DequeIterator operator++(int) {
    DequeIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  DequeIterator& operator--() {
    if (current_element_ == 0) {
      --current_chunk_;
      current_element_ = container_->kChunkSize;
    }
    --current_element_;
    return *this;
  }
  DequeIterator operator--(int) {
    DequeIterator tmp = *this;
    --(*this);
    return tmp;
  }

  bool operator<(const DequeIterator& other) const {
    if (container_ != other.container_) return false;
    if (current_chunk_ != other.current_chunk_)
      return current_chunk_ < other.current_chunk_;
    return current_element_ < other.current_element_;
  }

  bool operator>(const DequeIterator& other) const { return other < *this; }

  bool operator<=(const DequeIterator& other) const { return !(other < *this); }

  bool operator>=(const DequeIterator& other) const { return !(*this < other); }

  bool operator==(const DequeIterator& other) const {
    return container_ == other.container_ &&
           current_chunk_ == other.current_chunk_ &&
           current_element_ == other.current_element_;
  }
  bool operator!=(const DequeIterator& other) const {
    return container_ != other.container_ ||
           current_chunk_ != other.current_chunk_ ||
           current_element_ != other.current_element_;
  }

  DequeIterator& operator+=(difference_type n) {
    auto signed_chunk_size{
        static_cast<difference_type>(container_->kChunkSize)};

    auto index_difference{static_cast<difference_type>(current_element_) + n};

    if (index_difference < 0) {
      auto chunks_back = static_cast<size_type>(
          (-index_difference + signed_chunk_size - 1) / signed_chunk_size);
      current_chunk_ -= chunks_back;
      index_difference +=
          static_cast<difference_type>(chunks_back * container_->kChunkSize);
    }

    current_chunk_ +=
        static_cast<size_type>(index_difference / signed_chunk_size);
    current_element_ =
        static_cast<size_type>(index_difference % signed_chunk_size);

    return *this;
  }

  DequeIterator& operator-=(difference_type n) {
    return operator+=(static_cast<difference_type>(-n));
  }

  DequeIterator operator+(difference_type n) const {
    DequeIterator tmp(*this);
    tmp += n;
    return tmp;
  }

  DequeIterator operator-(difference_type n) const {
    DequeIterator tmp(*this);
    tmp -= n;
    return tmp;
  }

  difference_type operator-(const DequeIterator& other) const {
    if (container_ != other.container_) return 0;
    difference_type chunk_difference =
        static_cast<difference_type>(current_chunk_ - other.current_chunk_);
    difference_type element_difference =
        static_cast<difference_type>(current_element_ - other.current_element_);

    return chunk_difference *
               static_cast<difference_type>(container_->kChunkSize) +
           element_difference;
  }

 private:
  container_pointer container_{nullptr};
  size_type current_chunk_{};
  size_type current_element_{};
};

template <typename T, bool is_const>
DequeIterator<T, is_const> operator+(
    typename DequeIterator<T, is_const>::difference_type n,
    const DequeIterator<T, is_const>& it) {
  return it + n;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, deque<T>& q) {
  for (size_t index{0}; index < q.size(); ++index) {
    out << q[index] << ' ';
  }

  return out;
}

template <typename T>
bool operator==(const s21::deque<T>& first, const std::deque<T>& other) {
  using size_type = typename s21::deque<T>::traits::size_type;

  if (first.size() != other.size()) {
    return false;
  }

  for (size_type i{0}; i < first.size(); ++i) {
    if (first[i] != other[i]) {
      return false;
    }
  }
  return true;
}

template <typename T>
bool operator==(const std::deque<T>& first, const s21::deque<T>& other) {
  return other == first;
}

}  // namespace s21

#endif  // S21_DEQUE_H
