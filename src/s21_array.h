#ifndef S21_STACK_H
#define S21_STACK_H

#include <initializer_list>
#include <iostream>
#include <iterator>
#include <stdexcept>

#include "s21_base.h"
/*
value_type

Tопределяет тип элемента (T — параметр шаблона)


reference

T &определяет тип ссылки на элемент


const_reference

const T &определяет тип константной ссылки


iterator

T *определяет тип для итерации по контейнеру


const_iterator

const T *определяет тип константы для итерации по контейнеру

size_type

size_tо

array()+

array(std::initializer_list<value_type> const &items)

array(const array &a)

array(array &&a)

~array()

operator=(array &&a)

reference at(size_type pos)

reference operator[](size_type pos)+

const_reference front()

const_reference back()

iterator data()

iterator begin()

iterator end()

bool empty()+

size_type size()+

size_type max_size()+

void swap(array& other)

void fill(const_reference value)
*/
namespace s21 {

template <typename T, std::size_t N>
class array {
  using traits = container_traits<T>;
  using value_type = typename traits::value_type;
  using reference = typename traits::reference;
  using const_reference = typename traits::const_reference;
  using size_type = typename traits::size_type;

 private:
  value_type data[N];

 public:
  array() {
    for (size_type i = 0; i < N; ++i) {
      data[i] = T();
    }
  }

  reference operator[](size_type index) {
    if (index >= N) {
      throw std::logic_error("Index out of range");
    }
    return data[index];
  }

  const_reference operator[](size_type index) const {
    if (index >= N) {
      throw std::logic_error("Index out of range");
    }
    return data[index];
  }

  size_type size() const { return N; }
  bool empty() { return N == 0; }

  void fill(const T& value) {
    for (size_type i = 0; i < N; ++i) {
      data[i] = value;
    }
  }

  void print() const {
    for (size_type i = 0; i < N; ++i) {
      std::cout << data[i] << " ";
    }
    std::cout << std::endl;
  }
  size_type max_size() {
    return (std::numeric_limits<size_type>::max() / sizeof(value_type));
  }

  ~array() = default;
};

}  // namespace s21

#endif