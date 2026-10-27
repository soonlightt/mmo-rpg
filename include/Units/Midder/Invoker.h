#pragma once

#include <memory>
#include <string>

#include "Units/Midder/Midder.h"

class Invoker : public Midder {
 public:
  Invoker(int x, int y);
  ~Invoker() override = default;

  std::string getType() const override;
  char getSymbol() const override;
  std::unique_ptr<Unit> clone() const override;
};