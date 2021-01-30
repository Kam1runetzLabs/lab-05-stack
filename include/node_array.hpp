//
// Created by w1ckedente on 29.01.2021.
//

#ifndef STACK_NODE_ARRAY_HPP
#define STACK_NODE_ARRAY_HPP

#include <cassert>
#include <memory>
#include <utility>

#define _ARGS_TEMPL(args_type_name) template <typename... args_type_name>
// todo iterators

template <typename T, typename Allocator = std::allocator<T>>
class node_array {
 public:
  explicit node_array(std::size_t capacity)
      : _allocator(), _size(0), _capacity(capacity) {
    _data = _allocator.allocate(_capacity);
  }

  node_array(const node_array &) = delete;

  node_array(node_array &&other) noexcept
      : _allocator(std::move(other._allocator)),
        _data(std::move(other._data)),
        _size(std::move(other._size)),
        _capacity(std::move(other._capacity)) {}

  ~node_array() noexcept {
    if (_data) clear();
  }

  node_array &operator=(const node_array &) = delete;
  node_array &operator=(node_array &&other) noexcept {
    if (*this == other) return *this;
    if (_data) clear();
    _allocator = std::move(other._allocator);
    _data = std::move(other._data);
    _size = std::move(other._size);
    _capacity = std::move(other._capacity);
    return *this;
  }

  T &operator[](std::size_t index) noexcept {
    assert(index < _size);
    return _data[index];
  }

  T &front() noexcept {
    assert(_size != 0);
    return _data[0];
  }

  T &back() noexcept {
    assert(_size != 0);
    return _data[_size - 1];
  }

  void push_back(const T &value) {
    assert(_size != _capacity);
    _allocator.construct(&_data[_size++], value);
  }

  void push_back(T &&value) { emplace_back(std::move(value)); }

  void push_front(const T &value) {
    assert(_size != _capacity);
    for (std::size_t i = _size; i != 0; --i) {
      _data[i] = _data[i - 1];
    }
    _allocator.construct(&_data[0], value);
    _size++;
  }

  void push_front(T &&value) { emplace_front(std::move(value)); }

  void pop_back() noexcept {
    assert(_size != 0);
    --_size;
    _allocator.destroy(&_data[_size]);
  }
  void pop_front() noexcept {
    assert(_size != 0);
    for (std::size_t i = 0; i < _size - 1; ++i) {
      _data[i] = _data[i + 1];
    }
    --_size;
    _allocator.destroy(&_data[_size]);
  }

  std::size_t size() const noexcept { return _size; }

  std::size_t capacity() const noexcept { return _capacity; }

  _ARGS_TEMPL(args_t)
  void emplace_back(args_t &&...args) {
    assert(_size != _capacity);
    _allocator.construct(&_data[_size++], std::forward<args_t>(args)...);
  }

  _ARGS_TEMPL(args_t)
  void emplace_front(args_t &&...args) {
    assert(_size != _capacity);
    for (std::size_t i = _size; i != 0; --i) {
      _data[i] = _data[i - 1];
    }
    _allocator.construct(&_data[0], std::forward<args_t>(args)...);
    _size++;
  }

  _ARGS_TEMPL(args_t)
  void emplace(std::size_t index, args_t &&...args) {
    assert(_size != _capacity);
    for (std::size_t i = _size; i != index; --i) {
      _data[i] = _data[i - 1];
    }
    _allocator.construct(&_data[index], std::forward<args_t>(args)...);
    ++_size;
  }

  void insert(std::size_t index, const T &value) {
    assert(_size != _capacity);
    for (std::size_t i = _size; i != index; --i) {
      _data[i] = _data[i - 1];
    }
    _allocator.construct(&_data[index], value);
    ++_size;
  }

  void insert(std::size_t index, T &&value) {
    emplace(index, std::move(value));
  }

  bool empty() const noexcept { return !!_size; }

  void clear() noexcept {
    for (std::size_t i = 0; i < _size; ++i) _allocator.destroy(&_data[i]);
    _allocator.deallocate(_data, _capacity);
    _data = nullptr;
    _size = _capacity = 0;
  }

  // todo reserve
  void reserve(std::size_t capacity) {}

  // todo remove
  void remove(std::size_t index) {}

 private:
  Allocator _allocator;
  T *_data;
  std::size_t _size;
  std::size_t _capacity;
};

#undef _ARGS_TEMPL

#endif  // STACK_NODE_ARRAY_HPP
