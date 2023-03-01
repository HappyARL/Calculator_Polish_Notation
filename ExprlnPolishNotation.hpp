#pragma once
#include <stack>
#include <vector>

#include "AbstractToken.hpp"
#include "BracketToken.hpp"
#include "Exception.hpp"
#include "OperandToken.hpp"
#include "OperatorMINUS.hpp"
#include "OperatorMUL.hpp"
#include "OperatorPLUS.hpp"
#include "OperatorSUB.hpp"
#include "OperatorToken.hpp"
#include "OperatorUMINUS.hpp"
#include "OperatorUPLUS.hpp"

template <typename T>
class ExprInPolishNotation {
 private:
  std::vector<std::pair<AbstractToken*, int> > abstract_;

 public:
  int Priority(OperatorToken<T>* oper) {
    int prior;
    std::string sign = oper->GetStringToken();
    if (sign == "+") {
      prior = 1;
    }
    if (sign == "-") {
      prior = 1;
    }
    if (sign == "*") {
      prior = 2;
    }
    if (sign == "/") {
      prior = 2;
    }
    if (sign == "~") {
      prior = 3;
    }
    return prior;
  }

  std::pair<T, size_t> StrToNumber(const std::string& str, size_t index) {
    std::string number;
    number.push_back(str[index]);
    if (index != str.length() - 1) {
      while (index != str.length() - 1 && str[index + 1] != '+' &&
             str[index + 1] != '-' && str[index + 1] != '*' &&
             str[index + 1] != '/' && str[index + 1] != '(' &&
             str[index + 1] != ')' && str[index + 1] != ' ') {
        number.push_back(str[index + 1]);
        ++index;
      }
    }
    T num = (T)std::stod(number);
    return std::make_pair(num, index);
  }

  std::vector<std::pair<AbstractToken*, int> > NewVector(
      const std::string& str) {
    enum Index { Operand = 1, Bracket, Operator };
    bool is_binary = false;
    std::vector<std::pair<AbstractToken*, int> > tmp;
    for (size_t i = 0; i < str.length(); ++i) {
      if (str[i] == '+') {
        if (is_binary) {
          tmp.push_back(
              std::make_pair(new OperatorPLUS<T>("+", is_binary), Operator));
          is_binary = false;
        } else {
          tmp.push_back(
              std::make_pair(new OperatorUPLUS<T>("+", is_binary), Operator));
        }
        continue;
      }
      if (str[i] == '-') {
        if (is_binary) {
          tmp.push_back(
              std::make_pair(new OperatorMINUS<T>("-", is_binary), Operator));
          is_binary = false;
        } else {
          tmp.push_back(
              std::make_pair(new OperatorUMINUS<T>("~", is_binary), Operator));
        }
        continue;
      }
      if (str[i] == '*') {
        tmp.push_back(
            std::make_pair(new OperatorMUL<T>("*", is_binary), Operator));
        is_binary = false;
        continue;
      }
      if (str[i] == '/') {
        tmp.push_back(
            std::make_pair(new OperatorSUB<T>("/", is_binary), Operator));
        is_binary = false;
        continue;
      }
      if (str[i] == '(') {
        tmp.push_back(std::make_pair(new BracketToken("("), Bracket));
        is_binary = false;
        continue;
      }
      if (str[i] == ')') {
        tmp.push_back(std::make_pair(new BracketToken(")"), Bracket));
        is_binary = true;
        continue;
      }
      if (str[i] != ' ') {
        std::pair<T, size_t> pair = StrToNumber(str, i);
        T num = pair.first;
        i = pair.second;
        tmp.push_back(std::make_pair(new OperandToken<T>(num), Operand));
        is_binary = true;
      }
    }
    return tmp;
  }

  void FreeStack(std::stack<std::pair<AbstractToken*, int> > stack) {
    while (!stack.empty()) {
      delete stack.top().first;
      stack.pop();
    }
    delete abstract_[0].first;
    throw InvalidExpr();
  }

  ExprInPolishNotation(const std::string& str) {
    enum Index { Operand = 1, Bracket, Operator };
    std::vector<std::pair<AbstractToken*, int> > tmp = NewVector(str);
    std::stack<std::pair<AbstractToken*, int> > stack;
    for (size_t i = 0; i < tmp.size(); ++i) {
      if (tmp[i].second == Operand) {
        abstract_.push_back(tmp[i]);
        continue;
      }
      if (tmp[i].second == Bracket) {
        BracketToken* bracket_token = static_cast<BracketToken*>(tmp[i].first);
        if (bracket_token->IsOpening()) {
          stack.push(tmp[i]);
        }
        if (!bracket_token->IsOpening()) {
          bool meet_open = false;
          while (!stack.empty()) {
            if (stack.top().second == Bracket) {
              meet_open = true;
              delete stack.top().first;
              stack.pop();
              break;
            }
            abstract_.push_back(stack.top());
            stack.pop();
          }
          if (!meet_open) {
            throw InvalidExpr();
          }
          delete tmp[i].first;
        }
        continue;
      }
      OperatorToken<T>* operator_token =
          static_cast<OperatorToken<T>*>(tmp[i].first);
      while (!stack.empty()) {
        if (stack.top().second == Bracket) {
          break;
        }
        OperatorToken<T>* oper_stack =
            static_cast<OperatorToken<T>*>(stack.top().first);
        if (Priority(oper_stack) > Priority(operator_token)) {
          abstract_.push_back(stack.top());
          stack.pop();
        }
        break;
      }
      stack.push(tmp[i]);
    }
    while (!stack.empty()) {
      if (stack.top().second == Bracket) {
        FreeStack(stack);
      }
      abstract_.push_back(stack.top());
      stack.pop();
    }
  }

  const std::vector<std::pair<AbstractToken*, int> >& GetTokens() {
    return abstract_;
  }
};
