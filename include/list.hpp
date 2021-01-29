//
// Created by w1ckedente on 28.01.2021.
//

#ifndef INCLUDE_LIST_HPP_
#define INCLUDE_LIST_HPP_

#include <cassert>
#include <utility>

template <typename T, std::size_t NodeArraySize = 4>
class unrolled_list {
 public:
  unrolled_list() : _head(nullptr), _tail(nullptr), _size(0){};
  unrolled_list(const unrolled_list &) = delete;
  unrolled_list(unrolled_list &&other) noexcept {
    _head = std::move(other._head);
    _tail = std::move(other._tail);
    _size = std::move(other._size);
  }

  ~unrolled_list() { clear(); }

  unrolled_list &operator=(const unrolled_list &) = delete;
  unrolled_list &operator=(unrolled_list &&other) noexcept {
    clear();
    _head = std::move(other._head);
    _tail = std::move(other._tail);
    _size = std::move(other._size);
  }

  T &operator[](std::size_t index) {
    assert(index >= 0 && index < _size);
    // todo solve this problem
  }

  T &front() const {
    assert(_size != 0);
    return _head->node_array[0];
  }

  T &back() const {
    assert(_size != 0);
    return _tail->node_array[_tail->cur_index];
  }

  void push_back(T &&value) {
    if (_tail == nullptr) _head = _tail = new _unrolled_list_node;
    if (_tail->cur_index == NodeArraySize) {
      auto tmp = new _unrolled_list_node;
      tmp->prev = _tail;
      tmp->next = nullptr;
      _tail->next = tmp;
      _tail = tmp;
    }
    _tail->node_array[_tail->cur_index++] = value;
  }
  void push_front(T &&value);

  T pop_back();
  T pop_front();

  template <typename... args_t>
  void emplace_back(args_t &&...args) {
    if (_tail == nullptr) _head = _tail = new _unrolled_list_node;
    if (_tail->cur_index == NodeArraySize) {
      auto tmp = new _unrolled_list_node;
      tmp->prev = _tail;
      tmp->next = nullptr;
      _tail->next = tmp;
      _tail = tmp;
    }
    _tail->node_array[_tail->cur_index++](std::forward<args_t>(args)...);
  }

  template <typename... args_t>
  void emplace_front(args_t &&...args);

  template <typename... args_t>
  void emplace(std::size_t index, args_t &&...args);

  void insert(std::size_t index, T &&value);

  void clear() {
    for (auto it = _head; it != nullptr;) {
      auto tmp = it->next;
      delete it;
      it = tmp;
    }
    _head = _tail = nullptr;
    _size = 0;
  }

  std::size_t size() const { return _size; }
  bool empty() const { return _size == 0; }

 private:
  struct _unrolled_list_node {
    _unrolled_list_node() : next(nullptr), prev(nullptr), cur_index(0){};
    T node_array[NodeArraySize];
    _unrolled_list_node *next;
    _unrolled_list_node *prev;
    std::size_t cur_index;
  };

  _unrolled_list_node *_head;
  _unrolled_list_node *_tail;
  std::size_t _size;
};

#endif  // INCLUDE_LIST_HPP_
