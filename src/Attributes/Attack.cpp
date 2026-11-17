#include "Attributes/Attack.h"

#include <algorithm>

Attack::Attack(int initialDamage) : damage(initialDamage > 0 ? initialDamage : 0) {}

int Attack::getValue() const { return damage; }

int Attack::calculateDamage(const Armor& targetArmor) const {
  return targetArmor.reduceDamage(damage);
}

void Attack::modifyDamage(int delta) { damage = std::max(0, damage + delta); }