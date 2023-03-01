#pragma once
#include <string>

#include "AbstractToken.hpp"

template <typename T>
class OperandToken : public AbstractToken {
 private:
 protected:
  T num_;

 public:
  OperandToken(const std::string& str) : AbstractToken(str) {}

  OperandToken(const T& value) : AbstractToken(std::to_string(value)) {
    num_ = value;
  }

  ~OperandToken() = default;

  const T& GetValue() { return num_; }
};
