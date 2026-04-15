#pragma once
#include "table.h"

class UnorderedTable : public ITable {
  struct Entry {
    std::string key;
    Polynomial value;
  };
  Entry* data_;
  int size_;
  int capacity_;

  void resize() {
    int nc = capacity_ * 2;
    Entry* nd = new Entry[nc];
    for (int i = 0; i < size_; i++) nd[i] = data_[i];
    delete[] data_;
    data_ = nd;
    capacity_ = nc;
  }

 public:
  explicit UnorderedTable(int cap = 16) : size_(0), capacity_(cap) {
    data_ = new Entry[capacity_];
  }
  ~UnorderedTable() { delete[] data_; }

  std::string name() const override {
    return "Неупорядоченная таблица (массив)";
  }
  int size() const override { return size_; }

  void insert(const std::string& key, const Polynomial& val,
              int& ops) override {
    ops = 0;
    for (int i = 0; i < size_; i++) {
      ops++;
      if (data_[i].key == key) {
        data_[i].value = val;
        ops++;
        return;
      }
    }
    if (size_ >= capacity_) resize();
    data_[size_++] = {key, val};
    ops++;
  }

  bool find(const std::string& key, Polynomial& val, int& ops) override {
    ops = 0;
    for (int i = 0; i < size_; i++) {
      ops++;
      if (data_[i].key == key) {
        val = data_[i].value;
        return true;
      }
    }
    return false;
  }

  bool remove(const std::string& key, int& ops) override {
    ops = 0;
    for (int i = 0; i < size_; i++) {
      ops++;
      if (data_[i].key == key) {
        data_[i] = data_[--size_];
        ops++;
        return true;
      }
    }
    return false;
  }

  std::vector<std::string> get_keys() const override {
    std::vector<std::string> ks;
    for (int i = 0; i < size_; i++) ks.push_back(data_[i].key);
    return ks;
  }
};
