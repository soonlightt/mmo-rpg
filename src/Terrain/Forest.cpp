#include "Terrain/Forest.h"

Forest::Forest(int x, int y) : Terrain(x, y) {}

std::string Forest::getType() const { return "Forest"; }

char Forest::getSymbol() const { return 'T'; }

bool Forest::isPassable() const { return true; }

std::string Forest::getEffectDescription() const {
  return "Forest: Midder +10 Attack, Support -5 Armor";
}

void Forest::onEnter(Unit& unit) {
  if (unit.getRole() == "Midder") {
    unit.modifyAttack(10);
  } else if (unit.getRole() == "Support") {
    unit.modifyArmor(-5);
  }
}

void Forest::onLeave(Unit& unit) {
  if (unit.getRole() == "Midder") {
    unit.modifyAttack(-10);
  } else if (unit.getRole() == "Support") {
    unit.modifyArmor(5);
  }
}