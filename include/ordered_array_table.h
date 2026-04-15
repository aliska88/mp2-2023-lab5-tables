#pragma once
#include "table.h"

class OrderedArrayTable : public ITable {
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

  int bsearch(const std::string& key, int& ops) const {
    int lo = 0, hi = size_ - 1;
    while (lo <= hi) {
      ops++;
      int mid = (lo + hi) / 2;
      if (data_[mid].key == key) return mid;
      if (data_[mid].key < key)
        lo = mid + 1;
      else
        hi = mid - 1;
    }
    return -(lo + 1);
  }

 public:
  explicit OrderedArrayTable(int cap = 16) : size_(0), capacity_(cap) {
    data_ = new Entry[capacity_];
  }
  ~OrderedArrayTable() { delete[] data_; }

  std::string name() const override { return "Упорядоченная таблица (массив)"; }
  int size() const override { return size_; }

  void insert(const std::string& key, const Polynomial& val,
              int& ops) override {
    ops = 0;
    int pos = bsearch(key, ops);
    if (pos >= 0) {
      data_[pos].value = val;
      ops++;
      return;
    }
    int ins = -(pos + 1);
    if (size_ >= capacity_) resize();
    for (int i = size_; i > ins; i--) {
      data_[i] = data_[i - 1];
      ops++;
    }
    data_[ins] = {key, val};
    size_++;
    ops++;
  }

  bool find(const std::string& key, Polynomial& val, int& ops) override {
    ops = 0;
    int pos = bsearch(key, ops);
    if (pos >= 0) {
      val = data_[pos].value;
      return true;
    }
    return false;
  }

  bool remove(const std::string& key, int& ops) override {
    ops = 0;
    int pos = bsearch(key, ops);
    if (pos < 0) return false;
    for (int i = pos; i < size_ - 1; i++) {
      data_[i] = data_[i + 1];
      ops++;
    }
    size_--;
    ops++;
    return true;
  }

  std::vector<std::string> get_keys() const override {
    std::vector<std::string> ks;
    for (int i = 0; i < size_; i++) ks.push_back(data_[i].key);
    return ks;
  }
};
