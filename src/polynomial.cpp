#include <cctype>
#include <cmath>
#include "polynomial.h"

Polynomial::Polynomial() { head = new Node(Monom(0, 0)); }

Polynomial::Polynomial(const Polynomial& other) {
  head = new Node(Monom(0, 0));
  Node* curr = other.head->next;

  while (curr != nullptr) {
    addMonom(curr->data);
    curr = curr->next;
  }
}

Polynomial::~Polynomial() {
  clear();
  delete head;
}

void Polynomial::clear() {
  Node* curr = head->next;
  while (curr != nullptr) {
    Node* temp = curr;
    curr = curr->next;
    delete temp;
  }
  head->next = nullptr;
}

Polynomial& Polynomial::operator=(const Polynomial& other) {
  if (this != &other) {
    clear();
    Node* curr = other.head->next;

    while (curr != nullptr) {
      addMonom(curr->data);
      curr = curr->next;
    }
  }
  return *this;
}

void Polynomial::addMonom(const Monom& m) {
  if (std::abs(m.getCoeff()) < 1e-10) {
    return;
  }

  Node* curr = head;

  while (curr->next != nullptr && curr->next->data > m) {
    curr = curr->next;
  }

  if (curr->next != nullptr && curr->next->data.isSimilar(m)) {
    double newCoeff = curr->next->data.getCoeff() + m.getCoeff();
    if (std::abs(newCoeff) < 1e-10) {
      Node* temp = curr->next;
      curr->next = temp->next;
      delete temp;
    } else {
      curr->next->data.setCoeff(newCoeff);
    }
  } else {
    curr->next = new Node(m, curr->next);
  }
}

void Polynomial::insert(const Monom& m) { addMonom(m); }

void Polynomial::insert(double coeff, int x, int y, int z) {
  addMonom(Monom(coeff, x, y, z));
}

bool Polynomial::isEmpty() const { return head->next == nullptr; }

int Polynomial::size() const {
  int count = 0;
  Node* curr = head->next;
  while (curr != nullptr) {
    count++;
    curr = curr->next;
  }
  return count;
}

Polynomial Polynomial::operator+(const Polynomial& other) const {
  Polynomial result;

  Node* curr1 = head->next;
  Node* curr2 = other.head->next;

  while (curr1 != nullptr && curr2 != nullptr) {
    if (curr1->data > curr2->data) {
      result.addMonom(curr1->data);
      curr1 = curr1->next;
    } else if (curr1->data < curr2->data) {
      result.addMonom(curr2->data);
      curr2 = curr2->next;
    } else {
      double newCoeff = curr1->data.getCoeff() + curr2->data.getCoeff();
      if (std::abs(newCoeff) > 1e-10) {
        result.addMonom(Monom(newCoeff, curr1->data.getDegree()));
      }
      curr1 = curr1->next;
      curr2 = curr2->next;
    }
  }

  while (curr1 != nullptr) {
    result.addMonom(curr1->data);
    curr1 = curr1->next;
  }

  while (curr2 != nullptr) {
    result.addMonom(curr2->data);
    curr2 = curr2->next;
  }

  return result;
}

Polynomial Polynomial::operator-(const Polynomial& other) const {
  return *this + (other * (-1.0));
}

Polynomial Polynomial::operator*(double scalar) const {
  Polynomial result;

  Node* curr = head->next;
  while (curr != nullptr) {
    result.addMonom(curr->data * scalar);
    curr = curr->next;
  }

  return result;
}

Polynomial Polynomial::operator*(const Polynomial& other) const {
  Polynomial result;

  Node* curr1 = head->next;
  while (curr1 != nullptr) {
    Node* curr2 = other.head->next;
    while (curr2 != nullptr) {
      try {
        Monom product = curr1->data * curr2->data;
        result.addMonom(product);
      } catch (const std::overflow_error& e) {
        throw;
      }
      curr2 = curr2->next;
    }
    curr1 = curr1->next;
  }

  return result;
}

Polynomial& Polynomial::operator+=(const Polynomial& other) {
  *this = *this + other;
  return *this;
}

Polynomial& Polynomial::operator-=(const Polynomial& other) {
  *this = *this - other;
  return *this;
}

std::string Polynomial::toString() const {
  if (isEmpty()) return "0";

  std::string res = "";
  Node* curr = head->next;
  bool first = true;

  while (curr != nullptr) {
    std::string mStr = curr->data.toString();

    if (curr->data.getCoeff() > 0 && !first) {
      res += " + ";
    } else if (!first) {
      res += " ";
    }

    res += mStr;
    first = false;
    curr = curr->next;
  }
  return res;
}

std::ostream& operator<<(std::ostream& os, const Polynomial& p) {
  os << p.toString();
  return os;
}

void Polynomial::input() {
  int count;
  std::cout << "Введите количество мономов: ";
  std::cin >> count;

  for (int i = 0; i < count; ++i) {
    double c;
    int x, y, z;
    std::cout << "Моном " << (i + 1) << " (формат: Коэф. x y z): ";
    std::cin >> c >> x >> y >> z;
    try {
      insert(c, x, y, z);
    } catch (...) {
      std::cout << "Ошибка ввода монома\n";
    }
  }
}

bool Polynomial::operator==(const Polynomial& other) const {
  Node* curr1 = head->next;
  Node* curr2 = other.head->next;

  while (curr1 != nullptr && curr2 != nullptr) {
    if (curr1->data.getDegree() != curr2->data.getDegree() ||
        std::abs(curr1->data.getCoeff() - curr2->data.getCoeff()) > 1e-9) {
      return false;
    }
    curr1 = curr1->next;
    curr2 = curr2->next;
  }

  return curr1 == nullptr && curr2 == nullptr;
}
