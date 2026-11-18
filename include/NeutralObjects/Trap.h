#pragma once

#include <string>

#include "NeutralObjects/NeutralObject.h"
#include "Units/Unit.h"

class Trap : public NeutralObject {
 public:
  Trap(int x, int y);
  ~Trap() override = default;

  void interact(Unit& unit) override;
  std::string getType() const override;
  char getSymbol() const override;
  std::string getColoredSymbol() const override;
};