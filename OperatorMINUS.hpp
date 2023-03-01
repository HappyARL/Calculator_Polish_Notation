#pragma once
#include "AbstractToken.hpp"
#include "OperandToken.hpp"
#include "OperatorToken.hpp"

template <typename T>
class OperatorMINUS : public OperatorToken<T> {
 private:
 public:
  OperatorMINUS(const std::string& str, bool binary)
      : OperatorToken<T>(str, binary) {}

  OperandToken<T>* Calculate(OperandToken<T>* lhs,
                             OperandToken<T>* rhs) override {
    T left_value = lhs->GetValue();
    T right_value = rhs->GetValue();
    OperandToken<T>* result = new OperandToken<T>(right_value - left_value);
    return result;
  }

  OperandToken<T>* Calculate(OperandToken<T>* operand) override {
    return operand;
  }
};
