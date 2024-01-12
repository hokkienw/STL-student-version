#ifndef SRC_S21_QUEUE_H_
#define SRC_S21_QUEUE_H_

#include "s21_list.h"

namespace s21 {

template <typename T>
class queue {
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;

 public:
  queue() : conteiner_(){};
  queue(size_type n) : conteiner_(n) {}
  queue(std::initializer_list<value_type> const& items) : conteiner_(items) {}
  queue(const queue& q) : conteiner_(q.conteiner_) {}
  queue(queue&& q) : conteiner_(std::move(q.conteiner_)) {}
  ~queue() = default;

  const queue& operator=(queue& q);
  queue& operator=(queue&& q);

  const_reference front();
  const_reference back();

  bool empty();
  size_type size();

  void pop();
  void push(const_reference value);
  void swap(queue& other);

  template <typename... Args>
  void emplace_back(Args&&... args);

 private:
  s21::list<T> conteiner_;
};

template <typename T>
const queue<T>& queue<T>::operator=(queue& q) {
  if (this != &q) {
    conteiner_ = q.conteiner_;
  }
  return *this;
}

template <typename T>
queue<T>& queue<T>::operator=(queue&& q) {
  if (this != &q) {
    conteiner_ = std::move(q.conteiner_);
  }
  return *this;
}

template <typename T>
typename queue<T>::const_reference queue<T>::front() {
  return conteiner_.front();
}

template <typename T>
typename queue<T>::const_reference queue<T>::back() {
  return conteiner_.back();
}

template <typename T>
bool queue<T>::empty() {
  return conteiner_.empty();
}

template <typename T>
typename queue<T>::size_type queue<T>::size() {
  return conteiner_.size();
}

template <typename T>
void queue<T>::pop() {
  conteiner_.pop_front();
}

template <typename T>
void queue<T>::push(const_reference value) {
  conteiner_.push_back(value);
}

template <typename T>
void queue<T>::swap(queue& other) {
  conteiner_.swap(other.conteiner_);
}

template <typename T>
template <typename... Args>
void queue<T>::emplace_back(Args&&... args) {
  conteiner_.emplace_back(std::forward<Args>(args)...);
}

}  // namespace s21
#endif  //  SRC_S21_QUEUE_H_