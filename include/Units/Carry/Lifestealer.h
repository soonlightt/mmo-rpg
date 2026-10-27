#pragma once

#include <memory>
#include <string>

#include "Units/Carry/Carry.h"

class Lifestealer : public Carry {
 public:
  Lifestealer(int x, int y);
  ~Lifestealer() override = default;

  std::string getType() const override;
  char getSymbol() const override;
  std::unique_ptr<Unit> clone() const override;
};