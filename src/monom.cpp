#include "monom.h"
#include <cmath>

Monom::Monom(double c, int deg) : coeff(c), degree(deg) {}

Monom::Monom(double c, int x, int y, int z) : coeff(c) { setDegree(x, y, z); }

void Monom::setDegree(int x, int y, int z) {
  if (x < 0 || x > 9 || y < 0 || y > 9 || z < 0 || z > 9) {
    throw std::invalid_argument(
        "—тепени переменных должны быть в диапазоне от 0 до 9");
  }
  degree = x * 100 + y * 10 + z;
}

Monom Monom::operator*(double scalar) const {
  return Monom(coeff * scalar, degree);
}

Monom Monom::operator*(const Monom& other) const {
  int newX = getDegreeX() + other.getDegreeX();
  int newY = getDegreeY() + other.getDegreeY();
  int newZ = getDegreeZ() + other.getDegreeZ();

  if (newX > 9 || newY > 9 || newZ > 9) {
    throw std::overflow_error("—тепень переменной превышает 9");
  }

  return Monom(coeff * other.coeff, newX, newY, newZ);
}

bool Monom::operator<(const Monom& other) const {
  return degree < other.degree;
}

bool Monom::operator>(const Monom& other) const {
  return degree > other.degree;
}

bool Monom::operator==(const Monom& other) const {
  return degree == other.degree;
}

bool Monom::isSimilar(const Monom& other) const {
  return degree == other.degree;
}

std::string Monom::toString() const {
  if (std::abs(coeff) < 1e-10) return "";

  if (degree == 0) {
    return std::to_string(coeff);
  }

  std::string res = "";

  if (std::abs(coeff - 1.0) < 1e-10) {
    res += "";
  } else if (std::abs(coeff + 1.0) < 1e-10) {
    res += "-";
  } else {
    res += std::to_string(coeff);
  }

  int x = getDegreeX();
  int y = getDegreeY();
  int z = getDegreeZ();

  if (x > 0) {
    res += "x";
    if (x > 1) res += "^" + std::to_string(x);
  }
  if (y > 0) {
    res += "y";
    if (y > 1) res += "^" + std::to_string(y);
  }
  if (z > 0) {
    res += "z";
    if (z > 1) res += "^" + std::to_string(z);
  }

  return res;
}

std::ostream& operator<<(std::ostream& os, const Monom& m) {
  os << m.toString();
  return os;
}
