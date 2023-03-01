#pragma once
#include <iostream>

class AbstractToken {
 private:
 protected:
  std::string token_;

 public:
  AbstractToken(const std::string& str) { token_ = str; }

  virtual ~AbstractToken() = default;

  const std::string& GetStringToken() const { return token_; }
};
