#pragma once
#include <exception>

struct InvalidExpr : std::exception {
  const char* what() const noexcept override { return "Invalid expression!"; }
};
