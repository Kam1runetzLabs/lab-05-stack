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
  arr.insert(arr.cbegin(), 2);
  for (int & it : arr)
    std::cout << it << std::endl;
  EXPECT_TRUE(true);
}
