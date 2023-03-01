#pragma once
#include "AbstractToken.hpp"
#include "OperandToken.hpp"
#include "OperatorToken.hpp"

template <typename T>
class OperatorUMINUS : public OperatorToken<T> {
 private:
 public:
  OperatorUMINUS(const std::string& str, bool binary)
      : OperatorToken<T>(str, binary) {}

  OperandToken<T>* Calculate(OperandToken<T>* lhs,
                             OperandToken<T>* rhs) override {
    T left_value = lhs->GetValue();
    T right_value = rhs->GetValue();
    OperandToken<T>* result = new OperandToken<T>(left_value - right_value);
    return result;
  }

  OperandToken<T>* Calculate(OperandToken<T>* operand) override {
    T niga_operand = operand->GetValue();
    niga_operand = -niga_operand;
    OperandToken<T>* result = new OperandToken<T>(niga_operand);
    return result;
  }
};
