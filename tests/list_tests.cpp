// Copyright 2021 Kam1runetzLabs <notsoserious2017@gmail.com>

#include <gtest/gtest.h>

#include <iostream>
#include <list.hpp>
#include <vector>

TEST(ExampleList, EmptyTest) {
  list<int> lst;
  for (int i = 0; i < 10; ++i) lst.push_back(i);

  for (const auto &num : lst) std::cout << num << " " << std::endl;
  std::cout << "\n";

  std::cout << *(lst.begin() + 2) << "\n";
  std::cout << lst[2] << "\n";
}
