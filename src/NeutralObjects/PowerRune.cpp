#include "NeutralObjects/PowerRune.h"

#include "Strategies/PowerBoostStrategy.h"
#include "Units/Unit.h"

PowerRune::PowerRune(int x, int y) : NeutralObject(x, y, std::make_unique<PowerBoostStrategy>()) {}

void PowerRune::interact(Unit& unit) {
  if (!isActive || !unit.isAlive()) {
    return;
  }
  NeutralObject::interact(unit);
  isActive = false;
}

std::string PowerRune::getType() const { return "PowerRune"; }

char PowerRune::getSymbol() const { return '*'; }

std::string PowerRune::getColoredSymbol() const { return "\033[93m*\033[0m"; }