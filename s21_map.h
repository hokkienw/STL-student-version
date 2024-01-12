#ifndef SRC_S21_MAP_H_
#define SRC_S21_MAP_H_

#include "s21_set.h"

namespace s21 {
template <typename Key, typename T>
class map : public set<std::pair<Key, T>> {
 public:
  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<key_type, mapped_type>;
  using reference = value_type&;
  using const_reference = const value_type&;
  using Tree_Node = typename set<value_type>::Tree_Node;

  class MapIterator;
  using iterator = MapIterator;
  class MapConstIterator;
  using const_iterator = MapConstIterator;

  map() : set<value_type>::set() {}
  map(std::initializer_list<value_type> const& itemss);
  map(const map& m);
  map(map&& m);
  ~map() {}

  map<Key, T>& operator=(const map& m);
  map<Key, T>& operator=(map&& m);

  T& at(const Key& key);
  T& operator[](const Key& key);

  iterator begin() const;
  iterator end() const;
  const_iterator cbegin() const;
  const_iterator cend() const;

  std::pair<typename map<Key, T>::iterator, bool> insert(value_type value);
  std::pair<iterator, bool> insert(const Key& key, const T& obj);
  std::pair<typename map<Key, T>::iterator, bool> insert_or_assign(
      const Key& key, const T& obj);
  typename map<Key, T>::iterator find(const Key& key) const;
  void erase(iterator pos);

  bool contains(const Key& key) const;

  void merge(map& other);

 private:
  Tree_Node* ApendNode(const value_type value, Tree_Node* node,
                       bool& insert_flag);
};

// Класс Мап Конст итератор

template <typename Key, typename T>
class map<Key, T>::MapConstIterator
    : public set<std::pair<Key, T>>::SetConstIterator {
 public:
  MapConstIterator() : set<std::pair<Key, T>>::SetConstIterator() {}
  MapConstIterator(const const_iterator& cit)
      : set<std::pair<Key, T>>::SetConstIterator(cit) {}

  const_iterator& operator=(const const_iterator& it) {
    this->DestroyIterator();
    IterCopy(it);
    this->error_value_ = new value_type();
    return (*this);
  }
};

// Класс Мап итератор

template <typename Key, typename T>
class map<Key, T>::MapIterator : public MapConstIterator {
 public:
  MapIterator() : MapConstIterator() {}
  MapIterator(const iterator& it) : MapConstIterator(it) {}

  value_type& operator*() const {
    if (this->mass_ && (this->cur_position_ != this->mass_ + this->size_))
      return (*(this->cur_position_))->value;
    else
      return *(this->error_value_);
  }

  iterator& operator=(const iterator& it) {
    this->DestroyIterator();
    this->IterCopy(it);
    this->error_value_ = new value_type();
    return (*this);
  }
};

template <typename Key, typename T>
map<Key, T>::map(const map& m) : set<value_type>::set(m) {}

template <typename Key, typename T>
map<Key, T>::map(map&& m) : set<value_type>::set(std::move(m)) {}

template <typename Key, typename T>
map<Key, T>::map(std::initializer_list<value_type> const& itemss)
    : set<value_type>::set(itemss) {}

template <typename Key, typename T>
typename s21::map<Key, T>& map<Key, T>::operator=(const map& m) {
  if (this != &m) {
    this->clear();
    for (const auto& elem : m) insert(elem);
  }
  return *this;
}

template <typename Key, typename T>
typename s21::map<Key, T>& map<Key, T>::operator=(map&& m) {
  if (this != &m) {
    for (const auto& elem : m) insert(elem);
    m.clear();
  }
  return *this;
}

template <typename Key, typename T>
std::pair<typename map<Key, T>::iterator, bool> map<Key, T>::insert(
    value_type value) {
  bool insert_flag = true;
  this->root_ = ApendNode(value, this->root_, insert_flag);
  std::pair<iterator, bool> pr(begin(), insert_flag);
  if (pr.second) this->size_++;
  pr.first = find(value.first);
  return pr;
}

template <typename Key, typename T>
std::pair<typename map<Key, T>::iterator, bool> map<Key, T>::insert(
    const Key& key, const T& obj) {
  std::pair<key_type, mapped_type> insert_val(key, obj);
  return this->insert(insert_val);
}

template <typename Key, typename T>
std::pair<typename map<Key, T>::iterator, bool> map<Key, T>::insert_or_assign(
    const Key& key, const T& obj) {
  std::pair<iterator, bool> insert_val = insert(key, obj);
  if (!insert_val.second) {
    (*(insert_val.first)).second = obj;
  }
  return insert_val;
}

template <typename Key, typename T>
bool map<Key, T>::contains(const Key& key) const {
  if (this->root_) {
    for (iterator it((*this).begin()); it != (*this).end(); ++it)
      if ((*it).first == key) return true;
  }
  return false;
}

template <typename Key, typename T>
void map<Key, T>::merge(map& other) {
  if (this->root_ != other.root_ && other.root_) {
    iterator it_begin = other.begin();
    while (it_begin != other.end()) {
      insert(*it_begin);
      ++it_begin;
    }
  }
}

template <typename Key, typename T>
typename map<Key, T>::iterator map<Key, T>::find(const Key& key) const {
  iterator it;
  iterator it_begin = begin();
  while (it_begin != end()) {
    if ((*it_begin).first == key) {
      break;
    }
    ++it_begin;
  }
  return it_begin;
}

// Итераторы

template <typename Key, typename T>
typename map<Key, T>::iterator map<Key, T>::begin() const {
  iterator it;
  it.Convert(this->root_, this->size_);
  it.BeginSeter();
  return it;
}

template <typename Key, typename T>
typename map<Key, T>::iterator map<Key, T>::end() const {
  iterator it;
  it.Convert(this->root_, this->size_);
  it.EndSeter();
  return it;
}

template <typename Key, typename T>
typename map<Key, T>::const_iterator map<Key, T>::cbegin() const {
  const_iterator it;
  it.Convert(this->root_, this->size_);
  it.BeginSeter();
  return it;
}

template <typename Key, typename T>
typename map<Key, T>::const_iterator map<Key, T>::cend() const {
  const_iterator it;
  it.Convert(this->root_, this->size_);
  it.EndSeter();
  return it;
}

template <typename Key, typename T>
void map<Key, T>::erase(iterator pos) {
  map new_map;
  iterator it = this->begin();
  iterator insert_pos;
  while (it != this->end()) {
    if (it == pos) {
      insert_pos = it;
      ++it;
      continue;
    }
    new_map.insert(*it);
    ++it;
  }
  *this = new_map;

  pos = insert_pos;
}

template <typename Key, typename T>
T& map<Key, T>::at(const Key& key) {
  iterator it = find(key);
  if (it == end()) throw std::out_of_range("Index out of map bounds!");
  return (*it).second;
}

// Операторы

template <typename Key, typename T>
T& map<Key, T>::operator[](const Key& key) {
  iterator it = find(key);
  if (it == end()) {
    std::pair<iterator, bool> key_iter(it, true);
    std::pair<Key, T> insert_pair(key, 0);
    key_iter = insert(insert_pair);
    return (*(key_iter.first)).second;
  }
  return (*it).second;
}

template <typename Key, typename T>
typename map<Key, T>::Tree_Node* map<Key, T>::ApendNode(const value_type value,
                                                        Tree_Node* node,
                                                        bool& insert_flag) {
  if (!node) {
    Tree_Node* NewNode = new Tree_Node;
    NewNode->value = value;
    NewNode->left = nullptr;
    NewNode->right = nullptr;
    node = NewNode;
  } else if (value.first < node->value.first) {
    node->left = ApendNode(value, node->left, insert_flag);
  } else if (value.first > node->value.first) {
    node->right = ApendNode(value, node->right, insert_flag);
  } else if (value.first == node->value.first) {
    insert_flag = false;
  }
  return node;
}

}  // namespace s21
#endif  //  SRC_S21_MAP_H_
