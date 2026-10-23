#include "Attributes/Armor.h"

#include <algorithm>

Armor::Armor(int initialValue) : value(std::max(0, initialValue)) {}

int Armor::getValue() const { return value; }

int Armor::reduceDamage(int damage) const {
  if (damage < 0) return 0;
  return std::max(0, damage - value);
}