// Copyright 2021 Kam1runetzLabs <notsoserious2017@gmail.com>

/*
 * 1) const iterators and * operator for const iterator
 * 2) прояснить вопросы по итераторам
 * 3) переделать _tail на not-null
 * 4) переписать с использованием allocator
 * */

#ifndef INCLUDE_LIST_HPP_
#define INCLUDE_LIST_HPP_

#define _ARGS_TEMPL(args_type_name) template <typename... args_type_name>

#include <cassert>
#include <iterator>
#include <type_traits>

template <typename T, typename PointerType>
class list_iterator;

template <typename T>
class list {
 private:
  struct _node;
  friend class list_iterator<T, typename list<T>::_node>;
  friend class list_iterator<T, const typename list<T>::_node>;

 public:
  typedef list_iterator<T, typename list<T>::_node> iterator;
  typedef list_iterator<T, const typename list<T>::_node> const_iterator;

  list() : _head(nullptr), _tail(nullptr), _size(0){};

  list(const list &other) : _head(nullptr), _tail(nullptr), _size(0) {
    for (auto it = other.cbegin(); it != other.cend(); ++it)
      push_back(it.get());
  }

  list(list &&other) noexcept {
    _head = std::move(other._head);
    _tail = std::move(other._tail);
    _size = std::move(other._size);
  }

  ~list() {
    if (_head) clear();
  }

  list &operator=(const list &other) {
    if (&other == this) return *this;
    if (_head) clear();
    for (auto it = other.cbegin(); it != other.cend(); ++it)
      push_back(it.get());
    return *this;
  }

  list &operator=(list &&other) noexcept {
    if (other == *this) return *this;
    if (_head) clear();
    _head = std::move(other._head);
    _tail = std::move(other._tail);
    _size = std::move(other._size);
    return *this;
  }

  T &operator[](std::size_t index) noexcept {
    assert(index < _size);
    return *(begin() + index);
  }

  T operator[](std::size_t index) const noexcept {
    assert(index < _size);
    return (cbegin() + index).get();
  }

  T &front() noexcept { return *begin(); }
  T front() const noexcept { return *begin(); }
  T &back() noexcept { return _tail->value; }
  T back() const noexcept { return _tail->value; }

  void insert(iterator position, const T &value) {
    auto new_node = new _node(value);
    _insert_node(position, new_node);
  }

  _ARGS_TEMPL(args_t)
  void emplace(iterator position, args_t &&...args) {
    auto new_node = new _node(std::forward<args_t>(args)...);
    _insert_node(position, new_node);
  }

  void insert(iterator position, T &&value) {
    emplace(position, std::move(value));
  }

  void push_front(T &&value) { emplace_front(std::move(value)); }

  void push_front(const T &value) { insert(begin(), value); }

  void push_back(T &&value) { emplace_back(std::move(value)); }
  void push_back(const T &value) { insert(end(), value); }

  void pop_back() noexcept {
    assert(_tail != nullptr);
    _delete_node(end());
  }

  void pop_front() noexcept {
    assert(_head != nullptr);
    _delete_node(begin());
  }

  void remove(iterator position) noexcept {
    assert(position._pointer != nullptr || position == end());
    _delete_node(position);
  }

  _ARGS_TEMPL(args_t)
  void emplace_back(args_t &&...args) {
    emplace(end(), std::forward<args_t>(args)...);
  }

  _ARGS_TEMPL(args_t)
  void emplace_front(args_t &&...args) {
    emplace(begin(), std::forward<args_t>(args)...);
  }

  std::size_t size() const noexcept { return _size; }
  bool empty() const noexcept { return !_size; }

  iterator begin() noexcept { return iterator(_head); }
  iterator end() noexcept {
    return _tail == nullptr ? iterator(_tail) : iterator(_tail->next);
  }
  const_iterator cbegin() const noexcept { return const_iterator(_head); }
  const_iterator cend() const noexcept {
    return _tail == nullptr ? const_iterator(_tail)
                            : const_iterator(_tail->next);
  }

  void clear() noexcept {
    for (auto it = _head; it != nullptr;) {
      if (std::is_destructible<T>::value) it->value.~T();
      auto tmp = it->next;
      delete it;
      it = tmp;
    }
    _head = _tail = nullptr;
    _size = 0;
  }

 private:
  struct _node {
    explicit _node(const T &arg_value) noexcept
        : value(arg_value), next(nullptr), prev(nullptr) {}

    explicit _node(T &&arg_value) noexcept
        : value(std::move(arg_value)), next(nullptr), prev(nullptr) {}

    _ARGS_TEMPL(args_t)
    explicit _node(args_t &&...args) noexcept
        : value(T(std::forward<args_t>(args)...)),
          next(nullptr),
          prev(nullptr) {}

    T value;
    _node *next;
    _node *prev;
  };

  inline void _insert_node(iterator &position, _node *new_node) {
    assert(position._pointer != nullptr || position == end());
    ++_size;
    if (position == begin()) {
      new_node->next = _head;
      if (_head) _head->prev = new_node;
      _head = new_node;
      _tail = !_tail ? _head : _tail;
    } else if (position == end()) {
      new_node->prev = _tail;
      if (_tail) _tail->next = new_node;
      _tail = new_node;
      _head = !_head ? _tail : _head;
    } else {
      new_node->prev = position._pointer->prev;
      new_node->next = position._pointer;
      new_node->prev->next = new_node;
      position._pointer = new_node;
    }
  }

  inline void _delete_node(iterator position) {
    --_size;
    if (position == begin()) {
      auto tmp = _head;
      _head = _head->next;
      _head->prev = nullptr;
      if (std::is_destructible<T>::value) tmp->value.~T();
      delete tmp;
    } else if (position == end()) {
      auto tmp = _tail;
      _tail = _tail->prev;
      _tail->next = nullptr;
      if (std::is_destructible<T>::value) tmp->value.~T();
      delete tmp;
    } else {
      auto tmp = position._pointer;
      tmp->prev->next = tmp->next;
      tmp->next->prev = tmp->prev;
      delete tmp;
    }
  }

  _node *_head;
  _node *_tail;

  std::size_t _size;
};

template <typename T, typename PointerType>
class list_iterator : public std::iterator<std::input_iterator_tag, T> {
  friend class list<T>;

 public:
  list_iterator(const list_iterator &other) noexcept
      : _pointer(other._pointer){};

  bool operator==(const list_iterator &other) const noexcept {
    return _pointer == other._pointer;
  }

  bool operator!=(const list_iterator &other) const noexcept {
    return _pointer != other._pointer;
  }

  T &operator*() noexcept { return _pointer->value; }

  T get() const { return _pointer->value; }

  list_iterator &operator++() noexcept {
    _pointer = _pointer->next;
    return *this;
  }

  list_iterator &operator--() noexcept {
    _pointer = _pointer->prev;
    return *this;
  }

  ///@notsafe
  list_iterator operator+(std::size_t number) noexcept {
    auto res = *this;
    for (std::size_t i = 0; i != number; ++i) ++res;
    return res;
  }

 private:
  explicit list_iterator(PointerType *ptr) noexcept : _pointer(ptr) {}
  PointerType *_pointer;
};

#undef _ARGS_TEMPL
#endif  // INCLUDE_LIST_HPP_
