//
// Created by w1ckedente on 28.01.2021.
//

#ifndef INCLUDE_LIST_HPP_
#define INCLUDE_LIST_HPP_

#include <cassert>
#include <node_array.hpp>
#include <utility>

#define _ARGS_TEMPL(args_type_name) template <typename... args_type_name>

template <typename T, std::size_t NodeArrayCapacity = 4>
class unrolled_list {
 public:
  unrolled_list();
  unrolled_list(const unrolled_list&) = delete;
  unrolled_list(unrolled_list&& other) noexcept;
  ~unrolled_list() noexcept;

  unrolled_list& operator=(const unrolled_list&) = delete;
  unrolled_list& operator=(unrolled_list&& other) noexcept;

  T& operator[](std::size_t index) noexcept;

  T& front() noexcept;
  T& back() noexcept;

  void push_front(const T& value);
  void push_front(T&& value);

  void push_back(const T& value);
  void push_back(T&& value);

  void insert(std::size_t, const T& value);
  void insert(std::size_t, T&& value);

  void pop_back() noexcept;
  void pop_front() noexcept;

  _ARGS_TEMPL(args_t)
  void emplace_back(args_t&&... args);

  _ARGS_TEMPL(args_t)
  void emplace_front(args_t&&... args);

  _ARGS_TEMPL(args_t)
  void emplace(std::size_t index, args_t&&... args);

  void insert(const T& value);

  void clear() noexcept;

  std::size_t size() const noexcept;

  bool empty() const noexcept;

 private:
};

#undef _ARGS_TEMPL

#endif  // INCLUDE_LIST_HPP_
