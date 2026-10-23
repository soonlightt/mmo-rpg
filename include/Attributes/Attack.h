#pragma once
#include "Attributes/Armor.h"

class Attack {
 private:
  int damage;

 public:
  explicit Attack(int initialDamage = 10);

  int getAttack() const;
  int calculateDamage(const Armor& targetArmor) const;
};