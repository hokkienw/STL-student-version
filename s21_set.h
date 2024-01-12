#ifndef SRC_S21_SET_H_
#define SRC_S21_SET_H_

#include <cstddef>
#include <cstring>
#include <iostream>
#include <utility>
#include <vector>

namespace s21 {

// Класс Сет

template <typename Key>
class set {
 public:
  using key_type = Key;
  using value_type = Key;
  using reference = value_type&;
  using const_reference = const value_type&;
  using size_type = size_t;

  class SetIterator;
  using iterator = SetIterator;
  class SetConstIterator;
  using const_iterator = SetConstIterator;

  set();
  set(std::initializer_list<value_type> const& Tree_Nodes);
  set(const set& s);
  set(set&& s);
  ~set();

  set& operator=(set&& s);
  set& operator=(const set& s);

  iterator begin() const;
  iterator end() const;
  const_iterator cbegin() const;
  const_iterator cend() const;

  bool empty() const { return root_ == nullptr; }
  size_t size() const { return size_; }
  size_t max_size() const {
    return (SIZE_MAX / (sizeof(Tree_Node) + sizeof(size_type)));
  }

  void clear();
  std::pair<iterator, bool> insert(const value_type value);
  void erase(iterator pos);
  void swap(set& other);
  void merge(set& other);

  iterator find(const Key& key) const;
  bool contains(const Key& key) const;

  template <class... Args>
  std::vector<std::pair<iterator, bool>> emplace(Args&&... args);

 protected:
  struct Tree_Node {
    value_type value;
    Tree_Node* left;
    Tree_Node* right;
  };

  Tree_Node* root_;
  size_type size_;

  void Destroy(Tree_Node* ptr);

  void NodesCopy(Tree_Node* other_root);

  Tree_Node* ApendNode(const value_type value, Tree_Node* node,
                       bool& insert_flag);
  // private:
  // friend s21::map::Tree_Node* ApendNode(const value_type value, Tree_Node* node,
  //                      bool& insert_flag);
};

// Класс Конст итератор

template <typename Key>
class set<Key>::SetConstIterator {
 public:
  SetConstIterator();
  SetConstIterator(const const_iterator& it);
  SetConstIterator(const iterator& it);
  ~SetConstIterator();

  const_iterator& operator++();
  const_iterator& operator--();
  const value_type& operator*() const;

  bool operator==(const const_iterator& it) const;
  bool operator!=(const const_iterator& it) const;
  const_iterator& operator=(const const_iterator& it);
  void Convert(Tree_Node* root, size_t Tree_size);

  void BeginSeter() { cur_position_ = mass_; }
  void EndSeter() { cur_position_ = mass_ + size_; }

 protected:
  Tree_Node** mass_;
  Tree_Node** cur_position_;
  value_type* error_value_;
  int size_;

  void NewArr(int Tree_size);
  void IterCopy(const const_iterator& it);
  void AddNodesToArray(Tree_Node* root, int& cur_index);
  void DestroyIterator();
};

// Сет Класс итератор

template <typename Key>
class set<Key>::SetIterator : public SetConstIterator {
 public:
  SetIterator() : SetConstIterator() {}
  SetIterator(const const_iterator& cit) : SetConstIterator(cit) {}
};

// Кострукторы сет

template <typename Key>
set<Key>::set() {
  root_ = nullptr;
  size_ = 0;
  ;
}

template <typename Key>
set<Key>::set(std::initializer_list<value_type> const& Tree_Nodes) {
  size_ = 0;
  root_ = nullptr;
  for (auto it = Tree_Nodes.begin(); it != Tree_Nodes.end(); ++it) {
    bool insert_flag = true;
    root_ = ApendNode(*it, root_, insert_flag);
    if (insert_flag) size_++;
  }
}

template <typename Key>
set<Key>::set(const set& s) {
  root_ = nullptr;
  size_ = 0;
  *this = s;
}

template <typename Key>
set<Key>::set(set&& s) {
  root_ = nullptr;
  size_ = 0;
  *this = std::move(s);
}

template <typename Key>
set<Key>::~set() {
  this->clear();
}

// Операторы Сет

template <typename Key>
typename s21::set<Key>& set<Key>::operator=(const set& s) {
  if (this != &s) {
    this->clear();
    size_ = s.size_;
    NodesCopy(s.root_);
  }
  return *this;
}

template <typename Key>
typename s21::set<Key>& set<Key>::operator=(set&& s) {
  if (s.root_ != nullptr) {
    if (this != &s) {
      this->clear();
      root_ = s.root_;
      size_ = s.size_;
      s.root_ = nullptr;
      s.size_ = 0;
    }
  }
  return *this;
}

// Итераторы Сет

template <typename Key>
typename set<Key>::iterator set<Key>::begin() const {
  iterator it;
  it.Convert(root_, size_);
  it.BeginSeter();
  return it;
}

template <typename Key>
typename set<Key>::iterator set<Key>::end() const {
  iterator it;
  it.Convert(root_, size_);
  it.EndSeter();
  return it;
}

template <typename Key>
typename set<Key>::const_iterator set<Key>::cbegin() const {
  const_iterator it;
  it.Convert(root_, size_);
  it.BeginSeter();
  return it;
}

template <typename Key>
typename set<Key>::const_iterator set<Key>::cend() const {
  const_iterator it;
  it.Convert(root_, size_);
  it.EndSeter();
  return it;
}

// Методы модификаторы Сет

template <typename Key>
void set<Key>::clear() {
  Destroy(root_);
  root_ = nullptr;
  size_ = 0;
}

template <typename Key>
std::pair<typename set<Key>::iterator, bool> set<Key>::insert(
    const value_type value) {
  bool insert_flag = true;
  root_ = ApendNode(value, root_, insert_flag);
  std::pair<iterator, bool> pr((*this).begin(), insert_flag);
  if (pr.second) size_++;
  pr.first = find(value);
  return pr;
}

template <typename Key>
void set<Key>::erase(iterator pos) {
  set new_set;
  iterator it = this->begin();
  iterator insert_pos;
  while (it != this->end()) {
    if (it == pos) {
      insert_pos = it;
      ++it;
      continue;
    }
    new_set.insert(*it);
    ++it;
  }
  *this = new_set;

  pos = insert_pos;
}

template <typename Key>
void set<Key>::swap(set& other) {
  set swapit(other);
  other = (*this);
  *this = swapit;
}

template <typename Key>
void set<Key>::merge(set& other) {
  if (root_ != other.root_ && other.root_) {
    iterator it_begin = other.begin();
    while (it_begin != other.end()) {
      insert(*it_begin);
      ++it_begin;
    }
    other.clear();
  }
}

template <typename Key>
bool set<Key>::contains(const Key& key) const {
  if (root_) {
    iterator it_begin = begin();
    while (it_begin != end()) {
      if (*it_begin == key) {
        return true;
      }
      ++it_begin;
    }
  }
  return false;
}

template <typename Key>
typename set<Key>::iterator set<Key>::find(const Key& key) const {
  iterator it_begin = begin();
  while (it_begin != end()) {
    if (*it_begin == key) {
      break;
    }
    ++it_begin;
  }
  return it_begin;
}

template <typename Key>
template <typename... Args>
std::vector<std::pair<typename set<Key>::iterator, bool>> set<Key>::emplace(
    Args&&... args) {
  std::vector<std::pair<iterator, bool>> ret;
  std::vector<value_type> arg = {args...};
  for (size_type i = 0; i < arg.size(); i++) {
    ret.push_back(insert(arg[i]));
  }
  return ret;
}

// прайват функции
template <typename Key>
typename set<Key>::Tree_Node* set<Key>::ApendNode(const value_type value,
                                                  Tree_Node* node,
                                                  bool& insert_flag) {
  if (!node) {
    Tree_Node* NewNode = new Tree_Node{};
    NewNode->value = value;
    NewNode->left = nullptr;
    NewNode->right = nullptr;
    node = NewNode;
  } else if (value == node->value) {
    insert_flag = false;
  } else if (value < node->value) {
    node->left = ApendNode(value, node->left, insert_flag);
  } else {
    node->right = ApendNode(value, node->right, insert_flag);
  }
  return node;
}

template <typename Key>
void set<Key>::Destroy(Tree_Node* ptr) {
  if (ptr) {
    Destroy(ptr->left);
    Destroy(ptr->right);
    delete ptr;
  }
}

template <typename Key>
void set<Key>::NodesCopy(Tree_Node* other_root) {
  if (other_root) {
    bool insert_flag = true;
    root_ = ApendNode(other_root->value, root_, insert_flag);
    NodesCopy(other_root->left);
    NodesCopy(other_root->right);
  }
}

// КонстИтератор
template <typename Key>
set<Key>::const_iterator::SetConstIterator() {
  mass_ = nullptr;
  size_ = 0;
  cur_position_ = nullptr;
  error_value_ = new value_type();
}

template <typename Key>
set<Key>::const_iterator::SetConstIterator(const const_iterator& it) {
  IterCopy(it);
  error_value_ = new value_type();
}

template <typename Key>
set<Key>::const_iterator::SetConstIterator(const iterator& it) {
  IterCopy(it);
  error_value_ = new value_type();
}

template <typename Key>
set<Key>::const_iterator::~SetConstIterator() {
  DestroyIterator();
}

// Операторы Подкласса

template <typename Key>
const Key& set<Key>::const_iterator::operator*() const {
  if (mass_ && cur_position_ != mass_ + size_)
    return (*cur_position_)->value;
  else
    return *error_value_;
}

template <typename Key>
typename set<Key>::const_iterator& set<Key>::const_iterator::operator++() {
  if (cur_position_ == mass_ + size_)
    --cur_position_;
  else
    ++cur_position_;
  return (*this);
}

template <typename Key>
typename set<Key>::const_iterator& set<Key>::const_iterator::operator--() {
  if (cur_position_ != mass_) --cur_position_;
  return (*this);
}

template <typename Key>
bool set<Key>::const_iterator::operator==(const const_iterator& it) const {
  if ((mass_ && !it.mass_) || (!mass_ && it.mass_)) {
    return false;
  } else if (!mass_ && !it.mass_) {
    return true;
  } else {
    return (mass_[0] == it.mass_[0] &&
            ((mass_ + size_ - cur_position_) ==
             (it.mass_ + it.size_ - it.cur_position_)));
  }
}

template <typename Key>
bool set<Key>::const_iterator::operator!=(const const_iterator& it) const {
  return !(operator==(it));
}

template <typename Key>
typename set<Key>::const_iterator& set<Key>::const_iterator::operator=(
    const const_iterator& it) {
  DestroyIterator();
  IterCopy(it);
  error_value_ = new value_type();
  return (*this);
}

template <typename Key>
void set<Key>::const_iterator::AddNodesToArray(Tree_Node* node,
                                               int& cur_index) {
  if (node) {
    AddNodesToArray(node->left, cur_index);
    mass_[cur_index++] = node;
    AddNodesToArray(node->right, cur_index);
  }
}

template <typename Key>
void set<Key>::const_iterator::Convert(Tree_Node* root, size_t Tree_size) {
  if (root && mass_ == nullptr) {
    NewArr(Tree_size);
    int index = 0;
    AddNodesToArray(root, index);
  }
}

template <typename Key>
void set<Key>::const_iterator::NewArr(int arr_size) {
  mass_ = new Tree_Node*[arr_size + 1];
  size_ = arr_size;
  (*(mass_ + size_)) = nullptr;
}

template <typename Key>
void set<Key>::const_iterator::IterCopy(const const_iterator& it) {
  if (it.mass_) {
    int arr_size = it.size_;
    NewArr(arr_size);
    for (int i = 0; i < arr_size; i++) mass_[i] = it.mass_[i];
    cur_position_ = mass_ + (it.cur_position_ - it.mass_);
  } else {
    mass_ = nullptr;
    cur_position_ = nullptr;
    size_ = 0;
  }
}

template <typename Key>
void set<Key>::const_iterator::DestroyIterator() {
  if (mass_) {
    delete[] mass_;
    mass_ = nullptr;
  }
  if (error_value_) {
    delete error_value_;
    error_value_ = nullptr;
  }
}

}  // namespace s21
#endif  //  SRC_S21_SET_H_
