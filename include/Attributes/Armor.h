#pragma once

class Armor {
 private:
  int value;

 public:
  explicit Armor(int initialArmor = 0);

  int getValue() const;
  int reduceDamage(int damage) const;
};