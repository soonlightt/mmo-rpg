#include "NeutralObjects/Trap.h"

#include "Strategies/TrapDebuffStrategy.h"

Trap::Trap(int x, int y) : NeutralObject(x, y, std::make_unique<TrapDebuffStrategy>()) {}

void Trap::interact(Unit& unit) {
  if (!isActive || !unit.isAlive()) {
    return;
  }
  NeutralObject::interact(unit);
  isActive = false;
}

std::string Trap::getType() const { return "Trap"; }

char Trap::getSymbol() const { return 'x'; }

std::string Trap::getColoredSymbol() const { return "\033[91mx\033[0m"; }