#pragma once
#include <stack>

#include "AbstractToken.hpp"
#include "OperandToken.hpp"

template <typename T>
class OperatorToken : public AbstractToken {
 private:
  bool binary_;

 public:
  OperatorToken(const std::string& str, bool binary) : AbstractToken(str) {
    binary_ = binary;
  }

  ~OperatorToken() = default;

  bool IsBinary() { return binary_; }

  virtual OperandToken<T>* Calculate(OperandToken<T>* lhs,
                                     OperandToken<T>* rhs) = 0;

  virtual OperandToken<T>* Calculate(OperandToken<T>* operand) = 0;
};
