// Copyright 2021 Kam1runetzLabs <notsoserious2017@gmail.com>

#ifndef INCLUDE_STACK_HPP_
#define INCLUDE_STACK_HPP_

#include <list.hpp>

template <typename T, typename Container = list<T>>
class stack {
 public:
  stack() : _container(){};

  stack(const stack &) = delete;

  stack(stack &&other) noexcept { _container = std::move(other._container); }

  ~stack() = default;

  stack &operator=(const stack &) = delete;

  stack &operator=(stack &&other) noexcept {
    _container = std::move(other._container);
  }

  void push(T &&value) { _container.push_back(std::move(value)); }

  template <typename... args_t>
  void emplace(args_t &&...args) {
    return _container.emplace_back(std::forward<args_t>(args)...);
  }

  void pop() noexcept { _container.pop_back(); }

  T &top() const noexcept { return _container.back(); }

  std::size_t size() const noexcept { return _container.size(); }

  bool empty() const noexcept { return _container.empty(); }

  void clear() noexcept { _container.clear(); }

 private:
  Container _container;
};

#endif  // INCLUDE_STACK_HPP_
