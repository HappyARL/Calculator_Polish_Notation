#pragma once
#include "AbstractToken.hpp"
#include "OperandToken.hpp"
#include "OperatorToken.hpp"

template <typename T, bool Binary>
class OperatorPLUS : public OperatorToken<T, Binary> {
 private:
 public:
  OperatorPLUS(const std::string& str) : OperatorToken<T, Binary>(str) {}

  OperandToken<T>* Calculate(OperandToken<T>* lhs,
                             OperandToken<T>* rhs) override {
    T left_value = lhs->GetValue();
    T right_value = rhs->GetValue();
    OperandToken<T>* result = new OperandToken<T>(left_value + right_value);
    return result;
  }

  OperandToken<T>* Calculate(OperandToken<T>* operand) override {
    T positive_operand = operand->GetValue();
    positive_operand =
        positive_operand * ((positive_operand > 0) - (positive_operand < 0));
    OperandToken<T>* result = new OperandToken<T>(positive_operand);
    return result;
  }
};
