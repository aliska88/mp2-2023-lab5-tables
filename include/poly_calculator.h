#pragma once
#include <map>
#include <string>
#include "polynomial.h"

Polynomial evalPolyExpr(const std::string& expr,
                        const std::map<std::string, Polynomial>& vars);

Polynomial parsePolyString(const std::string& s);
