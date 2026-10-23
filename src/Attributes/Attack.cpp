#include "Attributes/Attack.h"

Attack::Attack(int initialDamage) : damage(initialDamage > 0 ? initialDamage : 0) {}

int Attack::getAttack() const { return damage; }

int Attack::calculateDamage(const Armor& targetArmor) const {
  return targetArmor.reduceDamage(damage);
}