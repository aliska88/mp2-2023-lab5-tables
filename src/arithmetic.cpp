#include <cctype>
#include <stdexcept>
#include <string>
#include "arithmetic.h"
#include "lexemes.h"

Postfix::Postfix()
    : infixTokens_(nullptr),
      infixLen_(0),
      infixMaxLen_(0),
      postfixTokens_(nullptr),
      postfixLen_(0),
      postfixMaxLen_(0) {}

Postfix::~Postfix() { clean(); }

void Postfix::clean() {
  if (infixTokens_) {
    for (int i = 0; i < infixLen_; i++) delete infixTokens_[i];
    delete[] infixTokens_;
  }
  delete[] postfixTokens_;
  infixTokens_ = postfixTokens_ = nullptr;
  infixLen_ = infixMaxLen_ = postfixLen_ = postfixMaxLen_ = 0;
}

void Postfix::add_token(Token**& arr, int& len, int& maxLen, Token* item) {
  if (len >= maxLen) {
    int newMax = (maxLen == 0) ? 10 : maxLen * 2;
    Token** newArr = new Token*[newMax];
    for (int i = 0; i < len; i++) newArr[i] = arr[i];
    if (arr) delete[] arr;
    arr = newArr;
    maxLen = newMax;
  }
  arr[len++] = item;
}

void Postfix::set_infix(const std::string& infix) {
  clean();
  infix_ = infix;
}

void Postfix::parse() {
  for (size_t i = 0; i < infix_.size(); i++) {
    char c = infix_[i];
    if (isspace(c)) continue;

    if (isdigit(c) || c == '.') {
      std::string num;
      bool hasDot = false;
      while (i < infix_.size() && (isdigit(infix_[i]) || infix_[i] == '.')) {
        if (infix_[i] == '.') {
          if (hasDot) throw std::runtime_error("Двойная точка в числе");
          hasDot = true;
        }
        num += infix_[i++];
      }
      i--;
      add_token(infixTokens_, infixLen_, infixMaxLen_,
                new Operand(std::stod(num)));
    } else if (c == 'x' || c == 'y' || c == 'z') {
      add_token(infixTokens_, infixLen_, infixMaxLen_,
                new Variable(std::string(1, c)));
    } else if (Operation::is_expr_operator(c)) {
      add_token(infixTokens_, infixLen_, infixMaxLen_, new Operation(c));
    } else {
      throw std::runtime_error(std::string("Неизвестный символ: ") + c);
    }
  }
}

void Postfix::to_postfix() {
  if (postfixTokens_) delete[] postfixTokens_;
  postfixTokens_ = nullptr;
  postfixLen_ = postfixMaxLen_ = 0;

  TStack<Operation*> opStack(10);

  for (int i = 0; i < infixLen_; i++) {
    Token* t = infixTokens_[i];

    if (t->get_type() == TOKEN_OPERAND || t->get_type() == TOKEN_VARIABLE) {
      add_token(postfixTokens_, postfixLen_, postfixMaxLen_, t);
    } else {
      Operation* op = dynamic_cast<Operation*>(t);
      char sym = op->get_symbol();

      if (sym == '(') {
        opStack.push(op);
      } else if (sym == ')') {
        bool found = false;
        while (!opStack.is_empty()) {
          Operation* top = opStack.pop();
          if (top->get_symbol() == '(') {
            found = true;
            break;
          }
          add_token(postfixTokens_, postfixLen_, postfixMaxLen_, top);
        }
        if (!found)
          throw std::runtime_error("Несбалансированные скобки: нет (");
      } else {
        while (!opStack.is_empty()) {
          Operation* top = opStack.top();
          if (top->get_symbol() == '(') break;
          if (top->get_priority() >= op->get_priority())
            add_token(postfixTokens_, postfixLen_, postfixMaxLen_,
                      opStack.pop());
          else
            break;
        }
        opStack.push(op);
      }
    }
  }

  while (!opStack.is_empty()) {
    Operation* top = opStack.pop();
    if (top->get_symbol() == '(')
      throw std::runtime_error("Несбалансированные скобки: нет )");
    add_token(postfixTokens_, postfixLen_, postfixMaxLen_, top);
  }
}

double Postfix::calculate(double xv, double yv, double zv) {
  TStack<double> calcStack(10);

  for (int i = 0; i < postfixLen_; i++) {
    Token* t = postfixTokens_[i];

    if (t->get_type() == TOKEN_OPERAND) {
      calcStack.push(dynamic_cast<Operand*>(t)->get_value());
    } else if (t->get_type() == TOKEN_VARIABLE) {
      std::string vn = dynamic_cast<Variable*>(t)->get_name();
      double val = (vn == "x") ? xv : (vn == "y") ? yv : zv;
      calcStack.push(val);
    } else {
      Operation* op = dynamic_cast<Operation*>(t);
      if (calcStack.is_empty())
        throw std::runtime_error("Недостаточно операндов");
      double v2 = calcStack.pop();
      if (calcStack.is_empty())
        throw std::runtime_error("Недостаточно операндов");
      double v1 = calcStack.pop();
      double res = 0;
      switch (op->get_symbol()) {
        case '+':
          res = v1 + v2;
          break;
        case '-':
          res = v1 - v2;
          break;
        case '*':
          res = v1 * v2;
          break;
        case '/':
          if (v2 == 0) throw std::runtime_error("Деление на ноль");
          res = v1 / v2;
          break;
      }
      calcStack.push(res);
    }
  }

  if (calcStack.is_empty()) return 0;
  double result = calcStack.pop();
  if (!calcStack.is_empty()) throw std::runtime_error("Лишние операнды");
  return result;
}

std::string Postfix::get_postfix_string() {
  std::string s;
  for (int i = 0; i < postfixLen_; i++) {
    Token* t = postfixTokens_[i];
    if (t->get_type() == TOKEN_OPERAND)
      s += std::to_string(dynamic_cast<Operand*>(t)->get_value()) + " ";
    else if (t->get_type() == TOKEN_VARIABLE)
      s += dynamic_cast<Variable*>(t)->get_name() + " ";
    else
      s += std::string(1, dynamic_cast<Operation*>(t)->get_symbol()) + " ";
  }
  return s;
}
