#ifndef TESTOBJECT_H_
#define TESTOBJECT_H_

#include <algorithm>
#include <cstring>
#include <iostream>

// Custom class for testing complex types
class TestObject {
 public:
  TestObject(int val = 0) : value_{val}, data_{new int[100]} {
    std::fill_n(data_, 100, val);
  }
  ~TestObject() noexcept {
    delete[] data_;
    data_ = nullptr;
  }

  TestObject(const TestObject& other) : value_{other.value_}, data_{nullptr} {
    int* new_data = new int[100];
    std::memcpy(new_data, other.data_, 100 * sizeof(int));
    data_ = new_data;
  }

  // Move constructor
  TestObject(TestObject&& other) noexcept
      : value_{other.value_}, data_{other.data_} {
    other.data_ = nullptr;
    other.value_ = 0;
  }

  // Copy assignment with strong exception guarantee
  TestObject& operator=(const TestObject& other) {
    if (this != &other) {      // Self-assignment check
      TestObject temp{other};  // Copy-and-swap idiom
      std::swap(value_, temp.value_);
      std::swap(data_, temp.data_);
    }
    return *this;
  }

  // Move assignment
  TestObject& operator=(TestObject&& other) noexcept {
    if (this != &other) {
      delete[] data_;  // Clean up existing resources
      data_ = other.data_;
      value_ = other.value_;
      other.data_ = nullptr;
      other.value_ = 0;
    }
    return *this;
  }

  bool operator==(const TestObject& other) const {
    return value_ == other.value_ &&
           std::equal(data_, data_ + 100, other.data_);
  }

  bool operator!=(const TestObject& other) const {
    return value_ != other.value_;
  }

  bool operator<(const TestObject& other) const {
    return value_ < other.value_;
  }

  friend std::ostream& operator<<(std::ostream& os, const TestObject& obj) {
    os << obj.value_;
    return os;
  }

  int get_value() const { return value_; }
  const int* get_data() const { return data_; }

 private:
  int value_;
  int* data_;
};

#endif  // TESTOBJECT_H_