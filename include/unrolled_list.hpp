//
// Created by w1ckedente on 28.01.2021.
//

#ifndef INCLUDE_LIST_HPP_
#define INCLUDE_LIST_HPP_

#include <cassert>
#include <type_traits>
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

  T &front() const;

  T &back() const;

  void push_back(T &&value);
  void push_front(T &&value);

  T pop_back();
  T pop_front();

  template <typename... args_t>
  void emplace_back(args_t &&...args);

  template <typename... args_t>
  void emplace_front(args_t &&...args);

  template <typename... args_t>
  void emplace(std::size_t index, args_t &&...args);

  void insert(std::size_t index, T &&value);

  void clear();

  /// @debug
  std::size_t nodes_count() const {
    return _size / NodeArraySize + !!(_size % NodeArraySize);
  }

  std::size_t size() const { return _size; }
  bool empty() const { return _size == 0; }

 private:
  struct _node {
    _node()
        : node_array(new T[NodeArraySize]),
          next(nullptr),
          prev(nullptr),
          size(0){};

    ~_node() { delete[] node_array; }

    T &operator[](std::size_t index) {
      assert(index >= 0 && index < size);
      return node_array[index];
    }

    void push_back(T &&value) {
      assert(size != NodeArraySize);
      node_array[size++] = value;
    }

    T pop_back() {
      assert(size != 0);
      return node_array[--size];
    }

    void push_front(T &&value) {
      assert(size != NodeArraySize);
      for (std::size_t i = 0; i < NodeArraySize - 1; ++i) {
        node_array[i + 1] = node_array[i];
      }
      node_array[0] = value;
    }

    T pop_front() {
      assert(size != 0);
      T ret = node_array[0];
      for (std::size_t i = 0; i < NodeArraySize + 1; ++i) {
        node_array[i] = node_array[i + 1];
      }
      --size;
      return ret;
    }

    T &back() const {
      assert(size != 0);
      return node_array[size - 1];
    }

    T &front() const {
      assert(size != 0);
      return node_array[0];
    }

    void clear() {
      for (std::size_t i = 0; i < size; ++i) {
        if (std::is_destructible_v<T>) node_array[i].~T();
      }
      size = 0;
    }

    void insert(std::size_t index, T &&value) {
      assert(size != NodeArraySize);

    }

    template <typename... args_t>
    void emplace(std::size_t index, args_t &&...args) {}

    template <typename... args_t>
    void emplace_back(args_t &&...args) {
      assert(size != NodeArraySize);
      node_array[size++].T(std::forward<args_t>(args)...);
    }

    template <typename... args_t>
    void emplace_front(args_t &&...args) {
      assert(size != NodeArraySize);
      for (std::size_t i = 0; i < NodeArraySize - 1; ++i) {
        node_array[i + 1] = node_array[i];
      }
      node_array[0].T(std::forward<args_t>(args)...);
    }

    T *node_array;
    _node *next;
    _node *prev;
    std::size_t size;
  };

  _node *_head;
  _node *_tail;
  std::size_t _size;
};

#endif  // INCLUDE_LIST_HPP_
