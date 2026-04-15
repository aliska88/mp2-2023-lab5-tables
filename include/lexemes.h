#pragma once
#include <stdexcept>
#include <string>

enum TokenType { TOKEN_OPERAND, TOKEN_OPERATION, TOKEN_VARIABLE };

class Token {
 public:
  virtual TokenType get_type() const = 0;
  virtual ~Token() = default;
};

class Operand : public Token {
  double value_;

 public:
  explicit Operand(double v) : value_(v) {}
  TokenType get_type() const override { return TOKEN_OPERAND; }
  double get_value() const { return value_; }
};

class Variable : public Token {
  std::string name_;

 public:
  explicit Variable(const std::string& n) : name_(n) {}
  TokenType get_type() const override { return TOKEN_VARIABLE; }
  std::string get_name() const { return name_; }
};

class Operation : public Token {
  char symbol_;

 public:
  explicit Operation(char sym) : symbol_(sym) {}
  TokenType get_type() const override { return TOKEN_OPERATION; }
  char get_symbol() const { return symbol_; }
  int get_priority() const {
    if (symbol_ == '+' || symbol_ == '-') return 1;
    if (symbol_ == '*' || symbol_ == '/') return 2;
    return 0;
  }
  static bool is_expr_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')';
  }
};
