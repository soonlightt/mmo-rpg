#pragma once
#include <string>

#include "NeutralObjects/NeutralObject.h"

class PowerRune : public NeutralObject {
 public:
  PowerRune(int x, int y);
  ~PowerRune() override = default;

  void interact(Unit& unit) override;
  std::string getType() const override;
  char getSymbol() const override;
  std::string getColoredSymbol() const override;
};