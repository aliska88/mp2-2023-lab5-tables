#pragma once
#include <string>
#include <vector>
#include "polynomial.h"

class ITable {
 public:
  virtual ~ITable() = default;
  virtual void insert(const std::string& key, const Polynomial& val,
                      int& ops) = 0;
  virtual bool find(const std::string& key, Polynomial& val, int& ops) = 0;
  virtual bool remove(const std::string& key, int& ops) = 0;
  virtual std::vector<std::string> get_keys() const = 0;
  virtual std::string name() const = 0;
  virtual int size() const = 0;
};
