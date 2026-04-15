#pragma once
#include "table.h"

class OrderedListTable : public ITable {
  struct Node {
    std::string key;
    Polynomial value;
    Node* next;
    Node(const std::string& k, const Polynomial& v, Node* n = nullptr)
        : key(k), value(v), next(n) {}
  };
  Node* head_;
  int size_;

 public:
  OrderedListTable() : head_(nullptr), size_(0) {}
  ~OrderedListTable() {
    Node* cur = head_;
    while (cur) {
      Node* t = cur;
      cur = cur->next;
      delete t;
    }
  }

  std::string name() const override { return "Упорядоченная таблица (список)"; }
  int size() const override { return size_; }

  void insert(const std::string& key, const Polynomial& val,
              int& ops) override {
    ops = 0;
    Node* prev = nullptr;
    Node* cur = head_;
    while (cur && cur->key < key) {
      ops++;
      prev = cur;
      cur = cur->next;
    }
    ops++;
    if (cur && cur->key == key) {
      cur->value = val;
      ops++;
      return;
    }
    Node* nd = new Node(key, val, cur);
    if (prev)
      prev->next = nd;
    else
      head_ = nd;
    size_++;
    ops++;
  }

  bool find(const std::string& key, Polynomial& val, int& ops) override {
    ops = 0;
    Node* cur = head_;
    while (cur) {
      ops++;
      if (cur->key == key) {
        val = cur->value;
        return true;
      }
      if (cur->key > key) break;
      cur = cur->next;
    }
    return false;
  }

  bool remove(const std::string& key, int& ops) override {
    ops = 0;
    Node* prev = nullptr;
    Node* cur = head_;
    while (cur && cur->key < key) {
      ops++;
      prev = cur;
      cur = cur->next;
    }
    ops++;
    if (!cur || cur->key != key) return false;
    if (prev)
      prev->next = cur->next;
    else
      head_ = cur->next;
    delete cur;
    size_--;
    ops++;
    return true;
  }

  std::vector<std::string> get_keys() const override {
    std::vector<std::string> ks;
    Node* cur = head_;
    while (cur) {
      ks.push_back(cur->key);
      cur = cur->next;
    }
    return ks;
  }
};
