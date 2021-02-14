// Copyright 2021 Kam1runetzLabs <notsoserious2017@gmail.com>

#include <gtest/gtest.h>

#include <list.hpp>
#include <type_traits>

// class for test
class coord_2d {
 public:
  coord_2d(int x, int y) : _x(x), _y(y){};
  coord_2d(const coord_2d &) = delete;
  coord_2d(coord_2d &&other) noexcept {
    _x = std::move(other._x);
    _y = std::move(other._y);
  }

  int x() const noexcept { return _x; }
  int y() const noexcept { return _y; }

 private:
  int _x;
  int _y;
};

TEST(DefaultMethodsTest, DefaultConstructor) {
  list<int> lst;
  EXPECT_TRUE(lst.empty());
  EXPECT_EQ(lst.size(), 0);
  EXPECT_EQ(lst.begin(), lst.end());
}

TEST(DefaultMethodsTest, CopyingConstructor) {
  list<int> lst1;
  for (int i = 0; i != 10; ++i) lst1.push_back(i);

  list<int> lst2(lst1);
  EXPECT_EQ(lst1.size(), lst2.size());

  for (int i = 0; i != 10; ++i) EXPECT_EQ(lst1[i], lst2[i]);
}

TEST(DefaultMethodsTest, MovingConstructor) {
  EXPECT_TRUE(std::is_move_constructible<list<int>>::value);
}

TEST(DefaultMethodsTest, MovingAssign) {
  EXPECT_TRUE(std::is_move_assignable<list<int>>::value);
}

TEST(DefaultMethodsTest, CopyingAssign) {
  list<int> lst1;
  for (int i = 0; i != 10; ++i) lst1.push_back(i);

  list<int> lst2;
  lst2 = lst1;
  EXPECT_EQ(lst1.size(), lst2.size());

  for (int i = 0; i != 10; ++i) EXPECT_EQ(lst1[i], lst2[i]);
}

TEST(InsertMethodsTest, PushBackMethodTest_lvalue) {
  list<int> lst;
  for (int i = 0; i != 10; ++i) lst.push_back(i);
  for (int i = 0; i != 10; ++i) EXPECT_EQ(i, lst[i]);
}

TEST(InsertMethodsTest, PushBackMethodTest_rvalue) {
  list<coord_2d> lst;
  for (int i = 0; i != 10; ++i) {
    coord_2d coord(i, 2 * i);
    lst.push_back(std::move(coord));
  }
  for (int i = 0; i != 10; ++i) {
    EXPECT_EQ(lst[i].x(), i);
    EXPECT_EQ(lst[i].y(), 2 * i);
  }
}

TEST(InsertMethodsTest, PushFrontMethodTest_lvalue) {
  list<int> lst;
  for (int i = 0; i != 10; ++i) lst.push_front(i);
  for (int i = 0; i != 10; ++i) EXPECT_EQ(9 - i, lst[i]);
}

TEST(InsertMethodsTest, PushFrontMethodTest_rvalue) {
  list<coord_2d> lst;
  for (int i = 0; i != 10; ++i) {
    coord_2d coord(i, 2 * i);
    lst.push_front(std::move(coord));
  }
  for (int i = 0; i != 10; ++i) {
    EXPECT_EQ(lst[i].x(), 9 - i);
    EXPECT_EQ(lst[i].y(), 2 * (9 - i));
  }
}

TEST(InsertMethodsTest, InsertMethodTest_lvalue) {
  list<int> lst;
  for (int i = 0; i != 10; ++i) lst.push_back(i);

  lst.insert(lst.begin() + 2, 123);

  EXPECT_EQ(lst[2], 123);
  EXPECT_EQ(lst.size(), 11);
}

TEST(InsertMethodsTest, InsertMethodTest_rvalue) {
  list<coord_2d> lst;
  for (int i = 0; i != 10; ++i) {
    lst.emplace_back(i, 2 * i);
  }

  coord_2d coord(1234, 123);
  lst.insert(lst.begin() + 2, std::move(coord));
  EXPECT_EQ(lst[2].x(), 1234);
  EXPECT_EQ(lst[2].y(), 123);
}

TEST(InsertMethodsTest, EmplaceBackMethod) {
  list<coord_2d> lst;
  for (int i = 0; i != 10; ++i) lst.emplace_back(i, 2 * i);

  for (int i = 0; i != 10; ++i) {
    EXPECT_EQ(lst[i].x(), i);
    EXPECT_EQ(lst[i].y(), 2 * i);
  }
}

TEST(InsertMethodsTest, EmplaceFrontMethod) {
  list<coord_2d> lst;
  for (int i = 0; i != 10; ++i) lst.emplace_front(i, 2 * i);

  for (int i = 0; i != 10; ++i) {
    EXPECT_EQ(lst[i].x(), 9 - i);
    EXPECT_EQ(lst[i].y(), 2 * (9 - i));
  }
}

TEST(InsertMethodsTest, EmplaceMethod) {
  list<coord_2d> lst;
  for (int i = 0; i != 10; ++i) lst.emplace_front(i, 2 * i);

  lst.emplace(lst.begin() + 2, 123, 123);
  EXPECT_EQ(lst[2].x(), 123);
  EXPECT_EQ(lst[2].y(), 123);
}

TEST(DeleteMethodsTest, PopFront) {
  list<int> lst;
  for (int i = 0; i != 10; ++i) lst.push_back(i);
  EXPECT_EQ(0, lst.front());
  EXPECT_EQ(10, lst.size());
  lst.pop_front();
  EXPECT_EQ(1, lst.front());
  EXPECT_EQ(9, lst.size());
}

TEST(DeleteMethodsTest, Remove) {
  list<int> lst;
  for (int i = 0; i != 10; ++i) lst.push_back(i);
  for (const auto &num : lst) std::cout << num << " ";

  EXPECT_EQ(2, lst[2]);
  EXPECT_EQ(10, lst.size());

  lst.remove(lst.begin() + 2);

  EXPECT_EQ(3, lst[2]);
  EXPECT_EQ(9, lst.size());
}

TEST(DeleteMethodsTest, PopBack) {
  list<int> lst;
  for (int i = 0; i != 10; ++i) lst.push_back(i);
  EXPECT_EQ(9, lst.back());
  EXPECT_EQ(10, lst.size());
  lst.pop_back();
  EXPECT_EQ(8, lst.back());
  EXPECT_EQ(9, lst.size());
}

TEST(MiscTesting, FrontAndBackMethods) {
  list<int> lst;
  for (int i = 0; i != 10; ++i) lst.push_back(i);
  EXPECT_EQ(lst.front(), 0);
  EXPECT_EQ(lst.back(), 9);
}

TEST(MiscTesting, ClearMethod) {
  list<int> lst;
  for (int i = 0; i != 10; ++i) lst.push_back(i);
  EXPECT_EQ(lst.size(), 10);
  EXPECT_FALSE(lst.empty());

  lst.clear();
  EXPECT_EQ(lst.size(), 0);
  EXPECT_TRUE(lst.empty());
}
