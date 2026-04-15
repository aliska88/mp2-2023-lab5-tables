#pragma once
#include <iostream>
#include <string>

class Monom {
 private:
  double coeff;
  int degree;

 public:
  Monom(double c = 0.0, int deg = 0);
  Monom(double c, int x, int y, int z);

  double getCoeff() const { return coeff; }
  int getDegree() const { return degree; }
  int getDegreeX() const { return degree / 100; }
  int getDegreeY() const { return (degree / 10) % 10; }
  int getDegreeZ() const { return degree % 10; }

  void setCoeff(double c) { coeff = c; }
  void setDegree(int deg) { degree = deg; }
  void setDegree(int x, int y, int z);

  Monom operator*(double scalar) const;
  Monom operator*(const Monom& other) const;

  bool operator<(const Monom& other) const;
  bool operator>(const Monom& other) const;
  bool operator==(const Monom& other) const;

  bool isSimilar(const Monom& other) const;

  std::string toString() const;
  friend std::ostream& operator<<(std::ostream& os, const Monom& m);
};
