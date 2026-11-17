#include "Terrain/Swamp.h"

Swamp::Swamp(int x, int y) : Terrain(x, y) {}

std::string Swamp::getType() const { return "Swamp"; }

char Swamp::getSymbol() const { return '~'; }

bool Swamp::isPassable() const { return true; }

std::string Swamp::getEffectDescription() const {
  return "Swamp: Carry and Midder -5 Armor, Support +8 Armor and +3 Attack";
}

void Swamp::onEnter(Unit& unit) {
  if (unit.getRole() == "Carry" || unit.getRole() == "Midder") {
    unit.modifyArmor(-5);
  } else if (unit.getRole() == "Support") {
    unit.modifyArmor(8);
    unit.modifyAttack(3);
  }
}

void Swamp::onLeave(Unit& unit) {
  if (unit.getRole() == "Carry" || unit.getRole() == "Midder") {
    unit.modifyArmor(5);
  } else if (unit.getRole() == "Support") {
    unit.modifyArmor(-8);
    unit.modifyAttack(-3);
  }
}