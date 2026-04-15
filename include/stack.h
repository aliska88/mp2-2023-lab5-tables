#ifndef TSTACK_H
#define TSTACK_H

#include <algorithm>
#include <stdexcept>

template <class T>
class TStack {
 public:
  TStack(int size = 10) {
    if (size <= 0)
      throw std::invalid_argument("Размер должен быть положительным");

    size_ = size;
    top_ = -1;
    pMem_ = new T[size_];
  }

  ~TStack() { delete[] pMem_; }

  void push(const T& val) {
    if (is_full()) resize();
    pMem_[++top_] = val;
  }

  T pop() {
    if (is_empty()) throw std::underflow_error("Стек пустой");
    return pMem_[top_--];
  }

  T top() const {
    if (is_empty()) throw std::underflow_error("Стек пустой");
    return pMem_[top_];
  }

  bool is_empty() const { return top_ == -1; }

  int get_count() const { return top_ + 1; }

  void clear() { top_ = -1; }

 private:
  T* pMem_;
  int size_;
  int top_;

  bool is_full() const { return top_ == size_ - 1; }

  void resize() {
    int newSize = size_ * 2;
    T* newMem = new T[newSize];

    std::copy(pMem_, pMem_ + size_, newMem);

    delete[] pMem_;
    pMem_ = newMem;
    size_ = newSize;
  }
};

#endif
