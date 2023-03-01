#pragma once
#include <stack>

#include "ExprlnPolishNotation.hpp"

template <typename T>
class Calculator {
 private:
 public:
  static T CalculateExpr(const std::string& expr) {
    enum Index { Operand = 1, Bracket, Operator };
    ExprInPolishNotation<T> exprln_polish = ExprInPolishNotation<T>(expr);
    std::vector<std::pair<AbstractToken*, int> > polish_expr =
        exprln_polish.GetTokens();
    size_t length = polish_expr.size();
    std::stack<OperandToken<T>*> stack;
    for (size_t i = 0; i < length; ++i) {
      int index = polish_expr[i].second;
      if (index == Operand) {
        OperandToken<T>* cur_operand =
            static_cast<OperandToken<T>*>(polish_expr[i].first);
        stack.push(cur_operand);
        continue;
      }
      if (index == Operator) {
        OperatorToken<T>* cur_oper =
            static_cast<OperatorToken<T>*>(polish_expr[i].first);
        if (static_cast<bool>(stack.empty())) {
          return -1;
        }
        if (cur_oper->IsBinary()) {
          OperandToken<T>* left = stack.top();
          stack.pop();
          if (static_cast<bool>(stack.empty())) {
            return -1;
          }
          OperandToken<T>* right = stack.top();
          stack.pop();
          OperandToken<T>* result = cur_oper->Calculate(left, right);
          delete left;
          delete right;
          stack.push(result);
        } else {
          OperandToken<T>* value = stack.top();
          stack.pop();
          OperandToken<T>* result = cur_oper->Calculate(value);
          delete value;
          stack.push(result);
        }
        delete cur_oper;
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
