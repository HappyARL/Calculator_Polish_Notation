#pragma once
#include "AbstractToken.hpp"

class BracketToken : public AbstractToken {
 private:
 public:
  BracketToken(const std::string& str) : AbstractToken(str) {}

  ~BracketToken() = default;

  bool IsOpening() const {
    bool is_open = false;
    if (this->GetStringToken() == "(") {
      return !is_open;
    }
    return is_open;
  }
};
