#ifndef S21_BASE_H
#define S21_BASE_H

namespace s21 {

template <typename T>
struct container_traits {
  using value_type = T;
  using size_type = std::size_t;
  using reference = T&;
  using const_reference = const T&;
};

}  // namespace s21

#endif  // S21_BASE_H
