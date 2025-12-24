#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

using namespace std;

template <typename KeyT, typename ValT>
class BSTMap {
 private:
  struct BSTNode {
    const KeyT key;  // Key is fixed after creation
    ValT value;
    BSTNode* parent;
    BSTNode* left;
    BSTNode* right;

    BSTNode(KeyT key, ValT value, BSTNode* parent)
        : key(key), value(value), parent(parent), left(nullptr), right(nullptr) {}
  };

  BSTNode* root;
  size_t sz;
  BSTNode* curr;

  BSTNode* findNode(const KeyT& key) const {
    BSTNode* current = root;
    while (current != nullptr) {
      if (key == current->key) return current;
      else if (key < current->key) current = current->left;
      else current = current->right;
    }
    return nullptr;
  }

  void clearHelper(BSTNode* node) {
    if (!node) return;
    clearHelper(node->left);
    clearHelper(node->right);
    delete node;
  }

  void copyHelper(BSTNode*& node, BSTNode* otherNode, BSTNode* parent) {
    if (!otherNode) {
      node = nullptr;
      return;
    }
    node = new BSTNode(otherNode->key, otherNode->value, parent);
    copyHelper(node->left, otherNode->left, node);
    copyHelper(node->right, otherNode->right, node);
  }

  void toStringHelper(BSTNode* node, ostringstream& ss) const {
    if (!node) return;
    toStringHelper(node->left, ss);
    ss << node->key << ": " << node->value << endl;
    toStringHelper(node->right, ss);
  }

 public:
  BSTMap() : root(nullptr), sz(0), curr(nullptr) {}

  bool empty() const { return sz == 0; }

  size_t size() const { return sz; }

  void insert(KeyT key, ValT value) {
    if (!root) {
      root = new BSTNode(key, value, nullptr);
      sz = 1;
      return;
    }

    BSTNode* current = root;
    BSTNode* parent = nullptr;

    while (current) {
      parent = current;
      if (key == current->key) return;
      else if (key < current->key) current = current->left;
      else current = current->right;
    }

    BSTNode* newNode = new BSTNode(key, value, parent);
    if (key < parent->key) parent->left = newNode;
    else parent->right = newNode;
    sz++;
  }

  ValT& at(const KeyT& key) const {
    BSTNode* node = findNode(key);
    if (!node) throw out_of_range("Key not found");
    return node->value;
  }

  bool contains(const KeyT& key) const { return findNode(key) != nullptr; }

  void clear() {
    clearHelper(root);
    root = nullptr;
    sz = 0;
  }

  ~BSTMap() { clear(); }

  string to_string() const {
    ostringstream ss;
    toStringHelper(root, ss);
    return ss.str();
  }

  BSTMap(const BSTMap& other) : root(nullptr), sz(0), curr(nullptr) {
    copyHelper(root, other.root, nullptr);
    sz = other.sz;
  }

  BSTMap& operator=(const BSTMap& other) {
    if (this == &other) return *this;
    clear();
    copyHelper(root, other.root, nullptr);
    sz = other.sz;
    return *this;
  }

  pair<KeyT, ValT> remove_min() {
    if (!root) throw runtime_error("Tree is empty");

    BSTNode* current = root;
    BSTNode* parent = nullptr;

    while (current->left) {
      parent = current;
      current = current->left;
    }

    pair<KeyT, ValT> result = {current->key, current->value};

    if (!parent) {
      root = current->right;
      if (root) root->parent = nullptr;
    } else if (current->right) {
      parent->left = current->right;
      current->right->parent = parent;
    } else {
      parent->left = nullptr;
    }

    delete current;
    sz--;
    return result;
  }

  bool operator==(const BSTMap& other) const {
    if (sz != other.sz) return false;
    if (!root && !other.root) return true;

    BSTMap this_copy(*this);
    BSTMap other_copy(other);

    this_copy.begin();
    other_copy.begin();

    KeyT key1, key2;
    ValT val1, val2;
    while (true) {
      bool has_next1 = this_copy.next(key1, val1);
      bool has_next2 = other_copy.next(key2, val2);
      if (has_next1 != has_next2) return false;
      if (!has_next1 && !has_next2) return true;
      if (key1 != key2 || val1 != val2) return false;
    }
  }

  void begin() {
    curr = root;
    if (!curr) return;
    while (curr->left) curr = curr->left;
  }

  bool next(KeyT& key, ValT& val) {
    if (!curr) return false;
    key = curr->key;
    val = curr->value;

    if (curr->right) {
      curr = curr->right;
      while (curr->left) curr = curr->left;
    } else {
      BSTNode* parent = curr->parent;
      while (parent && curr == parent->right) {
        curr = parent;
        parent = parent->parent;
      }
      curr = parent;
    }

    return true;
  }

  ValT erase(const KeyT& key) {
    BSTNode* current = root;
    BSTNode* parent = nullptr;

    while (current && current->key != key) {
      parent = current;
      if (key < current->key) current = current->left;
      else current = current->right;
    }

    if (!current) throw out_of_range("Key not found");

    ValT value_to_return = current->value;

    if (!current->left && !current->right) {
      if (!parent) root = nullptr;
      else if (parent->left == current) parent->left = nullptr;
      else parent->right = nullptr;
      delete current;
      sz--;
      return value_to_return;
    }

    if (!current->left || !current->right) {
      BSTNode* child = (current->left) ? current->left : current->right;
      if (!parent) root = child;
      else if (parent->left == current) parent->left = child;
      else parent->right = child;
      if (child) child->parent = parent;
      delete current;
      sz--;
      return value_to_return;
    }

    BSTNode* successor = current->right;
    BSTNode* successorParent = current;
    while (successor->left) {
      successorParent = successor;
      successor = successor->left;
    }

    const_cast<KeyT&>(current->key) = successor->key;
    current->value = successor->value;

    if (successorParent->left == successor)
      successorParent->left = successor->right;
    else
      successorParent->right = successor->right;

    if (successor->right) successor->right->parent = successorParent;

    delete successor;
    sz--;
    return value_to_return;
  }

  void* getRoot() const { return this->root; }
};