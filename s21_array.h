#ifndef SRC_S21_ARRAY_H_
#define SRC_S21_ARRAY_H_

#include <initializer_list>

namespace s21 {
template <typename T, size_t N>
class array {
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using size_type = size_t;
  using iterator = T *;
  using const_iterator = const T *;

 public:
  array() {}
  array(std::initializer_list<value_type> const &items);
  array(const array &a);
  array(array &&a);
  ~array() {}
  array &operator=(const array &a);
  array &operator=(array &&a);

  // Итераторы
  reference at(size_type pos);
  const_reference front() const;
  const_reference back() const;

  reference operator[](size_type pos);

  bool empty() const;
  size_type size() const;

  iterator data() { return mass_static_; }
  iterator begin() { return mass_static_; }
  iterator end() { return mass_static_ + N; }
  const_iterator cbegin() const { return mass_static_; }
  const_iterator cend() const { return mass_static_ + N; }

  //  методы
  void swap(array &other);
  void fill(const_reference value);
  size_type max_size() const { return N; };

 private:
  value_type mass_static_[N];
};
// Конструкторы

template <typename T, size_t N>
array<T, N>::array(std::initializer_list<value_type> const &items) {
  if (items.size() > N) {
    throw std::out_of_range("Too many initializers!");
  }
  size_type flag = 0;
  for (auto element : items) {
    if (flag == N) break;
    mass_static_[flag++] = element;
  }
}

template <typename T, size_t N>
array<T, N>::array(const array &a) {
  if (this != &a) {
  std::copy(a.mass_static_, a.mass_static_ + N, mass_static_);
  }
}

template <typename T, size_t N>
array<T, N>::array(array &&a) {
  if (this != &a) {
    *this = std::move(a);
  }
}

// Оператор пяти

template <typename T, size_t N>
typename s21::array<T, N> &array<T, N>::operator=(array &&a) {
  if (this != &a) {
  std::move(a.mass_static_, a.mass_static_ + N, mass_static_);
  }
  return *this; //
}

template <typename T, size_t N>
typename s21::array<T, N> &array<T, N>::operator=(const array &a) {
  if (this != &a) {
  std::copy(a.mass_static_, a.mass_static_ + N, mass_static_);
  }
  return *this;
}

template <typename T, size_t N>
typename s21::array<T, N>::reference s21::array<T, N>::operator[](
    size_type pos) {
  return mass_static_[pos];
}

// Размер

template <typename T, size_t N>
bool s21::array<T, N>::empty() const {
  return N == 0;
}

template <typename T, size_t N>
typename s21::array<T, N>::size_type s21::array<T, N>::size() const {
  return N;
}

template <typename T, size_t N>
typename s21::array<T, N>::reference s21::array<T, N>::at(size_type pos) {
  if (pos > N) throw std::out_of_range("Index out of array bounds!");
  return mass_static_[pos];
}
template <typename T, size_t N>
typename s21::array<T, N>::const_reference s21::array<T, N>::front() const {
  if (empty()) throw std::invalid_argument("Empty Array!");
  return mass_static_[0];
}

template <typename T, size_t N>
typename s21::array<T, N>::const_reference s21::array<T, N>::back() const {
  if (empty()) throw std::invalid_argument("Empty Array!");
  return mass_static_[N - 1];
}

// Методы клааса

template <typename T, size_t N>
void array<T, N>::swap(array &other) { 
  if (mass_static_ != other.mass_static_) {
    array temp(std::move(other)); // поправить
    other = std::move(*this); 
    *this = std::move(temp);
  }
}

template <typename T, size_t N>
void array<T, N>::fill(const_reference value) {
  for (size_type i = 0; i < N; i++) {
    mass_static_[i] = value;
  }
}

}  // namespace s21
#endif  //  SRC_S21_ARRAY_H_
