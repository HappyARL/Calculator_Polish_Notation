#pragma once
#include "AbstractToken.hpp"
#include "OperandToken.hpp"
#include "OperatorToken.hpp"

template <typename T, bool Binary>
class OperatorMINUS : public OperatorToken<T, Binary> {
 private:
 public:
  OperatorMINUS(const std::string& str) : OperatorToken<T, Binary>(str) {}

  OperandToken<T>* Calculate(OperandToken<T>* lhs,
                             OperandToken<T>* rhs) override {
    T left_value = lhs->GetValue();
    T right_value = rhs->GetValue();
    OperandToken<T>* result = new OperandToken<T>(right_value - left_value);
    return result;
  }

  OperandToken<T>* Calculate(OperandToken<T>* operand) override {
    T niga_operand = operand->GetValue();
    niga_operand = -niga_operand;
    OperandToken<T>* result = new OperandToken<T>(niga_operand);
    return result;
  }
};
