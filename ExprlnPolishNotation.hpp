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

template <typename T>
class ExprInPolishNotation {
 private:
  std::vector<std::pair<AbstractToken*, int> > abstract_;

 public:
  int Priority(std::string sign) {
    int prior;
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

  std::pair<std::pair<AbstractToken*, int>, bool> MakePLUSPair(bool is_binary) {
    enum Index { Operand = 1, Bracket, BINOperator, UNAROperator };
    if (is_binary) {
      is_binary = false;
      return std::make_pair(
          std::make_pair(new OperatorPLUS<T, true>("+"), BINOperator),
          is_binary);
    }
    return std::make_pair(
        std::make_pair(new OperatorPLUS<T, false>("+"), UNAROperator),
        is_binary);
  }

  std::pair<std::pair<AbstractToken*, int>, bool> MakeMINUSPair(
      bool is_binary) {
    enum Index { Operand = 1, Bracket, BINOperator, UNAROperator };
    if (is_binary) {
      is_binary = false;
      return std::make_pair(
          std::make_pair(new OperatorMINUS<T, true>("-"), BINOperator),
          is_binary);
    }
    return std::make_pair(
        std::make_pair(new OperatorMINUS<T, false>("~"), UNAROperator),
        is_binary);
  }

  std::pair<std::pair<AbstractToken*, int>, bool> MakeMULPair(bool is_binary) {
    enum Index { Operand = 1, Bracket, BINOperator, UNAROperator };
    if (is_binary) {
      is_binary = false;
      return std::make_pair(
          std::make_pair(new OperatorMUL<T, true>("*"), BINOperator),
          is_binary);
    }
    return std::make_pair(
        std::make_pair(new OperatorMUL<T, false>("*"), UNAROperator),
        is_binary);
  }

  std::pair<std::pair<AbstractToken*, int>, bool> MakeSUBPair(bool is_binary) {
    enum Index { Operand = 1, Bracket, BINOperator, UNAROperator };
    if (is_binary) {
      is_binary = false;
      return std::make_pair(
          std::make_pair(new OperatorSUB<T, true>("/"), BINOperator),
          is_binary);
    }
    return std::make_pair(
        std::make_pair(new OperatorSUB<T, false>("/"), UNAROperator),
        is_binary);
  }

  std::vector<std::pair<AbstractToken*, int> > NewVector(
      const std::string& str) {
    enum Index { Operand = 1, Bracket, BINOperator, UNAROperator };
    bool is_binary = false;
    std::vector<std::pair<AbstractToken*, int> > tmp;
    for (size_t i = 0; i < str.length(); ++i) {
      if (str[i] == '+') {
        std::pair<std::pair<AbstractToken*, int>, bool> plus_pair =
            MakePLUSPair(is_binary);
        tmp.push_back(plus_pair.first);
        is_binary = plus_pair.second;
        continue;
      }
      if (str[i] == '-') {
        std::pair<std::pair<AbstractToken*, int>, bool> minus_pair =
            MakeMINUSPair(is_binary);
        tmp.push_back(minus_pair.first);
        is_binary = minus_pair.second;
        continue;
      }
      if (str[i] == '*') {
        std::pair<std::pair<AbstractToken*, int>, bool> mul_pair =
            MakeMULPair(is_binary);
        tmp.push_back(mul_pair.first);
        is_binary = mul_pair.second;
        continue;
      }
      if (str[i] == '/') {
        std::pair<std::pair<AbstractToken*, int>, bool> sub_pair =
            MakeSUBPair(is_binary);
        tmp.push_back(sub_pair.first);
        is_binary = sub_pair.second;
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
    enum Index { Operand = 1, Bracket, BINOperator, UNAROperator };
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
      std::string operator_token = tmp[i].first->GetStringToken();
      while (!stack.empty()) {
        if (stack.top().second == Bracket) {
          break;
        }
        std::string operator_stack = stack.top().first->GetStringToken();
        if (Priority(operator_stack) > Priority(operator_token)) {
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
