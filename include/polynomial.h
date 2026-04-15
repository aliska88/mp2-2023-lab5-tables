#pragma once
#include <string>
#include "monom.h"

struct Node {
  Monom data;
  Node* next;

  Node(const Monom& m, Node* n = nullptr) : data(m), next(n) {}
};

class Polynomial {
 private:
  Node* head;

  void clear();
  void addMonom(const Monom& m);

 public:
  Polynomial();
  Polynomial(const Polynomial& other);
  ~Polynomial();

  Polynomial& operator=(const Polynomial& other);

  Polynomial operator+(const Polynomial& other) const;
  Polynomial operator-(const Polynomial& other) const;
  Polynomial operator*(double scalar) const;
  Polynomial operator*(const Polynomial& other) const;

  Polynomial& operator+=(const Polynomial& other);
  Polynomial& operator-=(const Polynomial& other);

  void insert(const Monom& m);
  void insert(double coeff, int x, int y, int z);

  bool isEmpty() const;
  int size() const;

  std::string toString() const;
  friend std::ostream& operator<<(std::ostream& os, const Polynomial& p);

  void input();

  bool operator==(const Polynomial& other) const;
};
