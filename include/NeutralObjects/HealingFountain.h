#pragma once

#include <string>

#include "NeutralObjects/NeutralObject.h"

class HealingFountain : public NeutralObject {
 public:
  HealingFountain(int x, int y);
  ~HealingFountain() override = default;

  std::string getType() const override;
  char getSymbol() const override;
  std::string getColoredSymbol() const override;
};