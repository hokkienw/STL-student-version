#ifndef SRC_S21_MULTISET_H_
#define SRC_S21_MULTISET_H_

#include <initializer_list>

#include "s21_set.h"

namespace s21 {

template <typename Key>
class multiset : public set<Key> {
 public:
  using key_type = Key;
  using value_type = Key;
  using reference = value_type&;
  using const_reference = const value_type&;
  using size_type = size_t;

  using MultisetIterator = typename s21::set<Key>::SetIterator;
  using MultisetConstIterator = typename s21::set<Key>::SetConstIterator;

  using iterator = MultisetIterator;
  using const_iterator = MultisetConstIterator;
  using Tree_Node = typename set<Key>::Tree_Node;

  multiset() : set<Key>::set() {}
  multiset(std::initializer_list<Key> const& Tree_Nodes);
  multiset(const multiset& ms);
  multiset(multiset&& ms);
  ~multiset() {}

  multiset& operator=(multiset&& s);
  multiset& operator=(const multiset& s);

  std::pair<iterator, bool> insert(const value_type value);
  void erase(iterator pos);

  size_t count(const Key& key);
  iterator lower_bound(const Key& key);
  iterator upper_bound(const Key& key);
  std::pair<iterator, iterator> equal_range(const Key& key);

 private:
  Tree_Node* ApendNode(const Key value, Tree_Node* node, bool& was_insert);
};

template <typename Key>
multiset<Key>::multiset(const multiset& ms) : set<Key>(ms) {}

template <typename Key>
multiset<Key>::multiset(multiset&& ms) : set<Key>(std::move(ms)) {}

template <typename Key>
multiset<Key>::multiset(std::initializer_list<value_type> const& Tree_Nodes) {
  set<Key>::size_ = 0;
  set<Key>::root_ = nullptr;
  for (auto it = Tree_Nodes.begin(); it != Tree_Nodes.end(); ++it) {
    bool was_insert = true;
    set<Key>::root_ = ApendNode(*it, set<Key>::root_, was_insert);
    if (was_insert) set<Key>::size_++;
  }
}

template <typename Key>
typename s21::multiset<Key>& multiset<Key>::operator=(const multiset& ms) {
  if (this == &ms) {
    return *this;
  }
  this->clear();
  for (const auto& elem : ms) insert(elem);
  return *this;
}

template <typename Key>
typename s21::multiset<Key>& multiset<Key>::operator=(multiset&& ms) {
  if (this == &ms) return *this;
  for (const auto& elem : ms) insert(elem);
  ms.clear();
  return *this;
}

template <typename Key>
std::pair<typename multiset<Key>::iterator, bool> multiset<Key>::insert(
    const value_type value) {
  bool was_insert = true;
  this->root_ = ApendNode(value, set<Key>::root_, was_insert);
  std::pair<iterator, bool> pr((*this).begin(), was_insert);
  if (pr.second) set<Key>::size_++;
  pr.first = set<Key>::find(value);
  return pr;
}

template <typename Key>
void multiset<Key>::erase(iterator pos) {
  multiset new_multiset;
  iterator it = this->begin();
  iterator was_insert;
  while (it != this->end()) {
    if (it == pos) {
      was_insert = it;
      ++it;
      continue;
    }
    new_multiset.insert(*it);
    ++it;
  }
  *this = new_multiset;

  pos = was_insert;
}

template <typename Key>
typename s21::multiset<Key>::size_type multiset<Key>::count(const Key& key) {
  size_type ret = 0;
  iterator it_begin = multiset::begin();
  iterator it_end = multiset::end();
  while (it_begin != it_end) {
    if (*it_begin == key) {
      ret++;
    }
    ++it_begin;
  }
  return ret;
}

template <typename Key>
typename s21::multiset<Key>::iterator multiset<Key>::lower_bound(
    const Key& key) {
  iterator it_begin = multiset::begin();
  iterator it_end = multiset::end();
  while (it_begin != it_end) {
    if (*it_begin == key) {
      return (it_begin);
    }
    ++it_begin;
  }
  return it_end;
}

template <typename Key>
typename s21::multiset<Key>::iterator multiset<Key>::upper_bound(
    const Key& key) {
  iterator it = multiset::begin();
  while (it != multiset::end() && *it <= key) {
    ++it;
  }
  return it;
}

template <typename Key>
std::pair<typename multiset<Key>::iterator, typename multiset<Key>::iterator>
multiset<Key>::equal_range(const Key& key) {
  return std::pair<typename multiset<Key>::iterator,
                   typename multiset<Key>::iterator>(lower_bound(key),
                                                     upper_bound(key));
}

template <typename value_type>
typename multiset<value_type>::Tree_Node* multiset<value_type>::ApendNode(
    const value_type value, Tree_Node* node, bool& was_insert) {
  if (!node) {
    Tree_Node* NewNode = new Tree_Node;
    NewNode->value = value;
    NewNode->left = nullptr;
    NewNode->right = nullptr;
    node = NewNode;
  } else if (value < node->value) {
    node->left = ApendNode(value, node->left, was_insert);
  } else {
    node->right = ApendNode(value, node->right, was_insert);
  }
  return node;
}

}  // namespace s21
#endif  //  SRC_S21_MULTISET_H_