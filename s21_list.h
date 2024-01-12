#ifndef SRC_S21_LIST_H_
#define SRC_S21_LIST_H_

#include <initializer_list>
#include <stdexcept>
#include <string>

namespace s21 {

template <typename T>
class list {
 public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;

  class ListConstIterator;
  class ListIterator;

  using iterator = ListIterator;
  using const_iterator = ListConstIterator;

  list();
  explicit list(size_type n);
  list(std::initializer_list<value_type> const& items);
  list(const list& l);
  list(list&& l);
  ~list();

  const list& operator=(list& l);//конст не нужен
  list& operator=(list&& l);

  const_reference front() const;
  const_reference back() const;

  iterator begin() const;
  iterator end() const;
  const_iterator cbegin() const;
  const_iterator cend() const;

  bool empty() const;
  size_type size() const;
  size_type max_size() const;

  void clear();
  iterator insert(iterator pos, const_reference value);
  void erase(iterator pos);
  void push_back(const_reference value);
  void pop_back();
  void push_front(const_reference value);
  void pop_front();
  void swap(list& other);
  void merge(list& other);
  void splice(const_iterator pos, list& other);
  void reverse();
  void unique();
  void sort();

  bool operator==(const list& other);

  template <typename... Args>
  iterator emplace(const_iterator pos, Args&&... args);
  template <typename... Args>
  iterator emplace(iterator pos, Args&&... args);
  template <typename... Args>
  void emplace_back(Args&&... args);
  template <typename... Args>
  void emplace_front(Args&&... args);

 protected:
  struct Node_list {
    value_type value;
    struct Node_list* next;
    struct Node_list* prev;
  };
  Node_list* root_;
  Node_list* end_;

  void ConstructDef();
  void CopyConstruct(const list& l);
};

template <typename T>
class list<T>::ListIterator {
 public:
  ListIterator() { pointer_ = nullptr; }
  ListIterator(Node_list& other) { pointer_ = &other; }
  ~ListIterator() { pointer_ = nullptr; }

  value_type& operator*();
  void operator=(Node_list& node);
  void operator++();
  void operator--();
  bool operator==(const ListIterator& other);
  bool operator!=(const ListIterator& other);

 protected:
  Node_list* pointer_;

 private:
  friend ListIterator list<T>::insert(iterator pos, const_reference value);
  friend void list<T>::erase(ListIterator pos);
  friend void list<T>::reverse();
  friend void list<T>::unique();
};

template <typename T>
class list<T>::ListConstIterator : public ListIterator {
 public:
  const value_type& operator*() { return ListIterator::operator*(); }
  void operator=(Node_list& node);
};

template <typename T>
void list<T>::const_iterator::operator=(Node_list& node) {
  this->pointer_ = &node;
}

template <typename T>
void list<T>::iterator::operator=(Node_list& node) {
  pointer_ = &node;
}

template <typename T>
bool list<T>::iterator::operator!=(const ListIterator& other) {
  return pointer_ != other.pointer_;
}

template <typename T>
typename list<T>::value_type& list<T>::iterator::operator*() {
  return pointer_->value;
}

template <typename T>
bool list<T>::iterator::operator==(const ListIterator& other) {
  return pointer_ == other.pointer_;
}

template <typename T>
void list<T>::iterator::operator++() {
  Node_list* nxt = pointer_->next;
  pointer_ = nxt;
}

template <typename T>
void list<T>::iterator::operator--() {
  Node_list* prv = pointer_->prev;
  pointer_ = prv;
}

template <typename T>
list<T>::list() {
  ConstructDef();
}

template <typename T>
list<T>::list(size_type n) {
  ConstructDef();
  if (n != 0) {
    Node_list* current = root_;
    for (; n > 0; n--) {
      current->value = value_type();
      current->next = new Node_list;
      current->next->prev = current;
      current->next->next = nullptr;
      current = current->next;
    }
    end_ = current;
  }
}

template <typename T>
list<T>::list(std::initializer_list<value_type> const& items) {
  ConstructDef();
  if (items.size() != 0) {
    Node_list* tmp = root_;
    auto i = items.begin();
    for (auto element : items) {
      tmp->value = element;
      tmp->next = new Node_list;
      tmp->next->prev = tmp;
      tmp->next->next = nullptr;
      tmp = tmp->next;
      i++;
    }
    end_ = tmp;
  }
}

template <typename T>
list<T>::list(const list& l) {
  if (this != &l) {
    CopyConstruct(l);
  }
}

template <typename T>
list<T>::list(list&& l) {
  if (this != &l) {
    ConstructDef();
    Node_list* temp = root_;
    Node_list* tempend = end_;
    root_ = l.root_;
    end_ = l.end_;
    l.root_ = temp;
    l.end_ = tempend;
  }
}

template <typename T>
list<T>::~list() {
  clear();
  delete root_;
}

template <typename T>
const list<T>& list<T>::operator=(list& l) {
  if (this != &l) {
    clear();
    delete root_;
    CopyConstruct(l);
  }
  return *this;
}

template <typename T>
list<T>& list<T>::operator=(list&& l) {
  if (this != &l) {
    if (l.root_ != nullptr) {
      clear();
      delete root_;
      ConstructDef();
      Node_list* temp = root_;
      Node_list* tempend = end_;
      root_ = l.root_;
      end_ = l.end_;
      l.root_ = temp;
      l.end_ = tempend;
    }
  }
  return *this;
}

template <typename T>
bool list<T>::operator==(const list& other) {
  bool return_value = true;
  if (this != &other) {
    Node_list* tmp = root_;
    Node_list* tmp2 = other.root_;
    while (tmp || tmp2) {
      if ((((tmp == nullptr || tmp2 == nullptr) &&
            !(tmp == nullptr && tmp2 == nullptr))) ||
          tmp->value != tmp2->value) {
        return_value = false;
        break;
      }
      tmp = tmp->next;
      tmp2 = tmp2->next;
    }
  }
  return return_value;
}

template <typename T>
void list<T>::ConstructDef() {
  root_ = end_ = new Node_list;
  root_->prev = root_->next = nullptr;
  root_->value = value_type();
}

template <typename T>
void list<T>::CopyConstruct(const list& l) {
  if (this != &l) {
    root_ = new Node_list;
    root_->prev = root_->next = nullptr;
    Node_list* tmp = root_;
    Node_list* l_tmp = l.root_;
    while (l_tmp) {
      tmp->value = l_tmp->value;
      if (l_tmp->next) {
        tmp->next = new Node_list;
        tmp->next->prev = tmp;
        tmp->next->next = nullptr;
        tmp = tmp->next;
        l_tmp = l_tmp->next;
      } else {
        break;
      }
    }
    end_ = tmp;
  }
}

// Итераторы списка

template <typename T>
typename list<T>::const_reference list<T>::front() const {
  if (!root_) throw std::out_of_range("size of list equal 0"); // лишнее
  return root_->value;
}

template <typename T>
typename list<T>::const_reference list<T>::back() const {
  if (!root_) throw std::out_of_range("size of list equal 0"); // лишнее
  return end_->prev ? end_->prev->value : end_->value;
}

template <typename T>
typename list<T>::iterator list<T>::begin() const {
  iterator it;
  it = *root_;
  return it;
}

template <typename T>
typename list<T>::iterator list<T>::end() const {
  iterator it;
  it = *end_;
  return it;
}

template <typename T>
typename list<T>::const_iterator list<T>::cbegin() const {
  const_iterator it;
  it = *root_;
  return it;
}

template <typename T>
typename list<T>::const_iterator list<T>::cend() const {
  const_iterator it;
  it = *end_;
  return it;
}

// Методы определения наполнености

template <typename T>
bool list<T>::empty() const {
  return root_ == end_;
}

template <typename T>
typename list<T>::size_type list<T>::size() const {
  size_type count = 0;
  Node_list* tmp = root_;
  while (tmp->next) {
    tmp = tmp->next;
    count++;
  }
  return count;
}

template <typename T>
typename list<T>::size_type list<T>::max_size() const {//компилятор дает половину памяти
  return SIZE_MAX / sizeof(Node_list);
}

// Метды общего списка

template <typename T>
void list<T>::clear() {
  if (root_ != end_) {
    Node_list* cur = root_;
    while (cur->next != nullptr) {
      cur = cur->next;
      delete cur->prev;
      cur->prev = nullptr;
    }
    root_ = cur;
    end_ = root_;
  }
}

template <typename T>
typename list<T>::iterator list<T>::insert(iterator pos,
                                           const_reference value) {
  Node_list* curent = new Node_list;
  curent->next = pos.pointer_;
  if ((pos.pointer_)->prev) {
    curent->prev = pos.pointer_->prev;
    pos.pointer_->prev->next = curent;
  } else {
    root_ = curent;
    curent->prev = nullptr;
  }
  pos.pointer_->prev = curent;
  curent->value = value;
  return *curent;
}

template <typename T>
void list<T>::erase(iterator pos) {
  if (pos == end()) throw std::out_of_range("Out of range in erase");
  Node_list* curent = pos.pointer_;
  if (curent->next && curent->prev) {
    curent->prev->next = curent->next;
    curent->next->prev = curent->prev;
  } else if (curent->next) {
    root_ = curent->next;
    curent->next->prev = nullptr;
  } else if (curent->prev) {
    curent->prev->next = nullptr;
  } else {
    root_ = end_ = nullptr;
  }
  delete curent;
}

template <typename T>
void list<T>::push_back(const_reference value) {
  insert(end(), value);
}

template <typename T>
void list<T>::pop_back() {
  iterator it;
  it = end();
  --it;
  erase(it);
}

template <typename T>
void list<T>::push_front(const_reference value) {
  insert(begin(), value);
}

template <typename T>
void list<T>::pop_front() {
  erase(begin());
}

template <typename T>
void list<T>::swap(list& other) {
  if (this != &other) {
    list tmp(*this);
    clear();
    delete root_;
    CopyConstruct(other);
    other.clear();
    delete other.root_;
    other.CopyConstruct(tmp);
  }
}

template <typename T>
void list<T>::merge(list& other) { // other clear убрать мерджаться только элементы которых нет в первом
  iterator it = other.begin();
  while (it != other.end()) {
    push_back(*it);
    ++it;
  }
  sort();
  other.clear(); //!
}

template <typename T>
void list<T>::splice(const_iterator pos, list& other) {
  iterator it = other.begin();
  while (it != other.end()) {
    insert(pos, *it);
    ++it;
  }
  other.clear();
}

template <typename T>
void list<T>::reverse() {
  if (size() > 1) {
    list temp;
    iterator it_begin, it_end;
    it_begin = begin();
    it_end = end();
    while (it_begin != it_end) {
      temp.push_front(*it_begin);
      ++it_begin;
    }
    *this = temp;
  }
}

template <typename T>  //посмотреть
void list<T>::unique() {
  if (size() > 1) {
    iterator it_begin, it_end;
    it_end = end();
    it_begin = begin();
    while (it_begin.pointer_ != it_end.pointer_) {
      iterator del = it_begin;
      ++del;
      while (del.pointer_ != it_end.pointer_) {
        if (*it_begin == *del) {
          iterator tmp;
          tmp = del;
          ++del;
          erase(tmp);
        } else {
          ++del;
        }
      }
      ++it_begin;
    }
  }
}

template <typename T>
void list<T>::sort() {
  size_type size = this->size();
  if (size > 1) {
    for (size_type i = 0; i < size; i++) {
      iterator it = begin();
      for (size_type j = 0; j < size - 1; j++) {
        iterator tmp = it;
        ++tmp;
        if (*it > *tmp) {
          T swap = *it;
          *it = *tmp;
          *tmp = swap;
        }
        ++it;
      }
    }
  }
}

template <typename T>
template <typename... Args>
typename list<T>::iterator list<T>::emplace(const_iterator pos,
                                            Args&&... args) {
  return insert(pos, value_type(std::forward<Args>(args)...));
}

template <typename T>
template <typename... Args>
typename list<T>::iterator list<T>::emplace(iterator pos, Args&&... args) {
  return insert(pos, value_type(std::forward<Args>(args)...));
}

template <typename T>
template <typename... Args>
void list<T>::emplace_back(Args&&... args) {
  iterator it;
  it = end();
  emplace(it, args...);
}
template <typename T>
template <typename... Args>
void list<T>::emplace_front(Args&&... args) {
  iterator it;
  it = begin();
  emplace(it, args...);
}
}  // namespace s21
#endif  //  SRC_S21_LIST_H_
