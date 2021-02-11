// Copyright 2021 Kam1runetzLabs <notsoserious2017@gmail.com>

#include <gtest/gtest.h>

#include <stack.hpp>
#include <type_traits>

// class for test
class coord_2d {
 public:
  coord_2d(int x, int y) : _x(x), _y(y){};
  coord_2d(const coord_2d &other) {
    _x = other._x;
    _y = other._y;
  }
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

TEST(DefaultMethods, MovingConstructor) {
  EXPECT_TRUE(std::is_move_constructible<stack<coord_2d>>::value);
}

TEST(DefaultMethods, MovingAssign) {
  EXPECT_TRUE(std::is_move_assignable<stack<coord_2d>>::value);
}

TEST(PushMethods, PushMethod_lvalue) {
  coord_2d coord(1, 2);
  stack<coord_2d> st;
  st.push(coord);

  EXPECT_EQ(st.top().x(), 1);
  EXPECT_EQ(st.top().y(), 2);
}

TEST(PushMethods, PushMethod_rvalue) {
  coord_2d coord(1, 2);
  stack<coord_2d> st;
  st.push(std::move(coord));

  EXPECT_EQ(st.top().x(), 1);
  EXPECT_EQ(st.top().y(), 2);
}

TEST(PushMethods, EmplaceMethod) {
  stack<coord_2d> st;
  st.emplace(1, 2);
  EXPECT_EQ(st.top().x(), 1);
  EXPECT_EQ(st.top().y(), 2);
}

TEST(PopMethod, PopMethod) {
  stack<int> st;
  for (int i = 0; i != 10; ++i) st.push(i);
  EXPECT_EQ(st.top(), 9);
  EXPECT_EQ(st.size(), 10);

  st.pop();

  EXPECT_EQ(st.top(), 8);
  EXPECT_EQ(st.size(), 9);
}

TEST(MiscTesting, ClearStackMethod) {
  stack<int> st;
  for (int i = 0; i != 10; ++i) st.push(i);
  EXPECT_EQ(st.size(), 10);
  EXPECT_FALSE(st.empty());

  st.clear();

  EXPECT_EQ(st.size(), 0);
  EXPECT_TRUE(st.empty());
}
