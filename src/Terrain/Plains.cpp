#include "Terrain/Plains.h"

Plains::Plains(int x, int y) : Terrain(x, y) {}

std::string Plains::getType() const { return "Plains"; }

char Plains::getSymbol() const { return '.'; }

bool Plains::isPassable() const { return true; }

std::string Plains::getEffectDescription() const { return "Plains: Carry +10 Attack"; }

void Plains::onEnter(Unit& unit) {
  if (unit.getRole() == "Carry") {
    unit.modifyAttack(10);
  }
}

void Plains::onLeave(Unit& unit) {
  if (unit.getRole() == "Carry") {
    unit.modifyAttack(-10);
  }
}