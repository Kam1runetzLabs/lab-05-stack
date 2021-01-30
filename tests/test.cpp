// Copyright 2020 Your Name <your_email>

#include <gtest/gtest.h>

#include <iostream>
#include <vector>
#include <node_array.hpp>

class test {
 public:
  explicit test(int x) : _x(x) {
    std::cout << "test()" << std::endl;
  }
  ~test() { std::cout << _x << ": ~test()" << std::endl; }

 private:
  int _x;
};

TEST(Example, EmptyTest) {
  node_array<int> arr(10);
  for (int i = 0; i < 10; ++i) {
    arr.push_front(i);
  }
  for (int i = 0; i < 10; ++i) {
    std::cout << arr[i] << " ";
  }
  arr.pop_back();
  std::cout << std::endl;
  arr.pop_front();
  for (std::size_t i = 0; i < arr.size(); ++i) {
    std::cout << arr[i] << " ";
  }
  EXPECT_TRUE(true);
}
