#include <cctype>
#include <stdexcept>
#include <vector>
#include "poly_calculator.h"

static int polyOpPrio(char c) {
  if (c == '+' || c == '-') return 1;
  if (c == '*') return 2;
  return 0;
}

enum PType { PT_POLY, PT_NUM, PT_OP };
struct PToken {
  PType type;
  std::string name;
  double num;
  char op;
};

Polynomial evalPolyExpr(const std::string& expr,
                        const std::map<std::string, Polynomial>& vars) {
  std::vector<PToken> tokens;
  size_t i = 0;
  while (i < expr.size()) {
    if (isspace((unsigned char)expr[i])) {
      i++;
      continue;
    }
    if (isalpha((unsigned char)expr[i]) || expr[i] == '_') {
      std::string nm;
      while (i < expr.size() &&
             (isalnum((unsigned char)expr[i]) || expr[i] == '_'))
        nm += expr[i++];
      tokens.push_back({PT_POLY, nm, 0, 0});
    } else if (isdigit((unsigned char)expr[i]) || expr[i] == '.') {
      std::string ns;
      while (i < expr.size() &&
             (isdigit((unsigned char)expr[i]) || expr[i] == '.'))
        ns += expr[i++];
      tokens.push_back({PT_NUM, "", std::stod(ns), 0});
    } else if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' ||
               expr[i] == '(' || expr[i] == ')') {
      tokens.push_back({PT_OP, "", 0, expr[i++]});
    } else {
      throw std::runtime_error(std::string("Неизвестный символ: ") + expr[i]);
    }
  }

  std::vector<PToken> postfix;
  std::vector<char> opStack;

  for (auto& t : tokens) {
    if (t.type == PT_POLY || t.type == PT_NUM) {
      postfix.push_back(t);
    } else {
      char sym = t.op;
      if (sym == '(') {
        opStack.push_back(sym);
      } else if (sym == ')') {
        bool found = false;
        while (!opStack.empty()) {
          char top = opStack.back();
          opStack.pop_back();
          if (top == '(') {
            found = true;
            break;
          }
          postfix.push_back({PT_OP, "", 0, top});
        }
        if (!found) throw std::runtime_error("Несбалансированные скобки");
      } else {
        while (!opStack.empty() && opStack.back() != '(' &&
               polyOpPrio(opStack.back()) >= polyOpPrio(sym)) {
          postfix.push_back({PT_OP, "", 0, opStack.back()});
          opStack.pop_back();
        }
        opStack.push_back(sym);
      }
    }
  }
  while (!opStack.empty()) {
    if (opStack.back() == '(')
      throw std::runtime_error("Несбалансированные скобки");
    postfix.push_back({PT_OP, "", 0, opStack.back()});
    opStack.pop_back();
  }

  std::vector<Polynomial> stack;
  for (auto& t : postfix) {
    if (t.type == PT_POLY) {
      auto it = vars.find(t.name);
      if (it == vars.end())
        throw std::runtime_error("Полином не найден: " + t.name);
      stack.push_back(it->second);
    } else if (t.type == PT_NUM) {
      Polynomial p;
      if (t.num != 0.0) p.insert(t.num, 0, 0, 0);
      stack.push_back(p);
    } else {
      if (stack.size() < 2) throw std::runtime_error("Недостаточно операндов");
      Polynomial b = stack.back();
      stack.pop_back();
      Polynomial a = stack.back();
      stack.pop_back();
      Polynomial res;
      switch (t.op) {
        case '+':
          res = a + b;
          break;
        case '-':
          res = a - b;
          break;
        case '*':
          res = a * b;
          break;
        default:
          throw std::runtime_error("Неизвестный оператор");
      }
      stack.push_back(res);
    }
  }
  if (stack.size() != 1) throw std::runtime_error("Ошибка в выражении");
  return stack[0];
}

Polynomial parsePolyString(const std::string& s) {
  Polynomial result;
  size_t i = 0;
  size_t n = s.size();

  while (i < n) {
    while (i < n && isspace((unsigned char)s[i])) i++;
    if (i >= n) break;

    int sign = 1;
    if (s[i] == '+') {
      sign = 1;
      i++;
    } else if (s[i] == '-') {
      sign = -1;
      i++;
    }

    while (i < n && isspace((unsigned char)s[i])) i++;

    double coeff = 1.0;
    bool hasCoeff = false;
    if (i < n && (isdigit((unsigned char)s[i]) || s[i] == '.')) {
      std::string ns;
      while (i < n && (isdigit((unsigned char)s[i]) || s[i] == '.'))
        ns += s[i++];
      coeff = std::stod(ns);
      hasCoeff = true;
      while (i < n && isspace((unsigned char)s[i])) i++;
      if (i < n && s[i] == '*') {
        i++;
        while (i < n && isspace((unsigned char)s[i])) i++;
      }
    }

    int xd = 0, yd = 0, zd = 0;
    while (i < n && (s[i] == 'x' || s[i] == 'y' || s[i] == 'z')) {
      char var = s[i++];
      int deg = 1;
      if (i < n && s[i] == '^') {
        i++;
        std::string ds;
        while (i < n && isdigit((unsigned char)s[i])) ds += s[i++];
        if (!ds.empty()) deg = std::stoi(ds);
      }
      if (var == 'x')
        xd += deg;
      else if (var == 'y')
        yd += deg;
      else if (var == 'z')
        zd += deg;
      while (i < n && isspace((unsigned char)s[i])) i++;
    }

    if (!hasCoeff && xd == 0 && yd == 0 && zd == 0) break;

    try {
      result.insert(sign * coeff, xd, yd, zd);
    } catch (const std::exception& e) {
      throw std::runtime_error(std::string("Ошибка в мономе: ") + e.what());
    }
  }
  return result;
}
