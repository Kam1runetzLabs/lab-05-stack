// Copyright 2020 Your Name <your_email>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_

#include <list>

template <typename T, typename Container = std::list<T>>
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

  void push(T &&value) { _container.push_back(std::forward<T>(value)); }

  template <typename... args_t>
  void emplace(args_t &&...args) {
    return _container.emplace_back(std::forward<args_t>(args)...);
  }

  T pop() noexcept {
    T ret = _container.back();
    _container.pop_back();
    return ret;
  }

  T top() const noexcept {
    T ret = _container.back();
    return ret;
  }

  std::size_t size() const noexcept { return _container.size(); }

  bool empty() const noexcept { return _container.empty(); }

  void clear() noexcept { _container.clear(); }

 private:
  Container _container;
};

#endif  // INCLUDE_HEADER_HPP_
