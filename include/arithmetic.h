#pragma once
#include <string>
#include "lexemes.h"
#include "stack.h"

class Postfix {
 public:
  Postfix();
  ~Postfix();

  void set_infix(const std::string& infix);
  std::string get_postfix_string();

  void parse();
  void to_postfix();
  double calculate(double xv = 0.0, double yv = 0.0, double zv = 0.0);

 private:
  std::string infix_;

  Token** infixTokens_;
  int infixLen_;
  int infixMaxLen_;

  Token** postfixTokens_;
  int postfixLen_;
  int postfixMaxLen_;

  void add_token(Token**& arr, int& len, int& maxLen, Token* item);
  void clean();
};
