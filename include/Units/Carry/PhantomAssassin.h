#pragma once

#include <memory>
#include <string>

#include "Units/Carry/Carry.h"

class PhantomAssassin : public Carry {
 public:
  PhantomAssassin(int x, int y);
  ~PhantomAssassin() override = default;

  std::string getType() const override;
  char getSymbol() const override;
  std::unique_ptr<Unit> clone() const override;
};