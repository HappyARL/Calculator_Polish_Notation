#pragma once
#include <stack>

#include "ExprlnPolishNotation.hpp"

template <typename T>
class Calculator {
 private:
 public:
  static T CalculateExpr(const std::string& expr) {
    enum Index { Operand = 1, Bracket, BINOperator, UNAROperator };
    ExprInPolishNotation<T> exprln_polish = ExprInPolishNotation<T>(expr);
    std::vector<std::pair<AbstractToken*, int> > polish_expr =
        exprln_polish.GetTokens();
    std::stack<OperandToken<T>*> stack;
    for (size_t i = 0; i < polish_expr.size(); ++i) {
      int index = polish_expr[i].second;
      if (index == Operand) {
        OperandToken<T>* cur_operand =
            static_cast<OperandToken<T>*>(polish_expr[i].first);
        stack.push(cur_operand);
        continue;
      }
      if (index == BINOperator) {
        OperatorToken<T, true>* curr_operator =
            static_cast<OperatorToken<T, true>*>(polish_expr[i].first);
        if (static_cast<bool>(stack.empty())) {
          return -1;
        }
        OperandToken<T>* left = stack.top();
        stack.pop();
        if (static_cast<bool>(stack.empty())) {
          return -1;
        }
        OperandToken<T>* right = stack.top();
        stack.pop();
        OperandToken<T>* result = curr_operator->Calculate(left, right);
        delete left;
        delete right;
        stack.push(result);
        delete curr_operator;
        continue;
      }
      if (index == UNAROperator) {
        OperatorToken<T, false>* curr_operator =
            static_cast<OperatorToken<T, false>*>(polish_expr[i].first);
        if (static_cast<bool>(stack.empty())) {
          return -1;
        }
        OperandToken<T>* value = stack.top();
        stack.pop();
        OperandToken<T>* result = curr_operator->Calculate(value);
        delete value;
        stack.push(result);
        delete curr_operator;
        continue;
      }
    }
    if (stack.size() > 1) {
      while (!stack.empty()) {
        delete stack.top();
        stack.pop();
      }
      throw InvalidExpr();
    }
    OperandToken<T>* num = static_cast<OperandToken<T>*>(stack.top());
    T result = num->GetValue();
    delete num;
    return result;
  }
};
