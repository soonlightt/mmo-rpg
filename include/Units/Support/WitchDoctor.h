#pragma once

#include <memory>
#include <string>

#include "Units/Support/Support.h"

class WitchDoctor : public Support {
 public:
  WitchDoctor(int x, int y);
  ~WitchDoctor() override = default;

  std::string getType() const override;
  char getSymbol() const override;
  std::unique_ptr<Unit> clone() const override;
};