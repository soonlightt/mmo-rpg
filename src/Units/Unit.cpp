#include "Units/Unit.h"

Unit::Unit(int startX, int startY, const Health& h, const Armor& arm, const Attack& att)
    : x(startX), y(startY), health(h), armor(arm), attack(att) {}

int Unit::getX() const { return x; }

int Unit::getY() const { return y; }

void Unit::setPosition(int newX, int newY) {
  x = newX;
  y = newY;
}

const Health& Unit::getHealth() const { return health; }

const Armor& Unit::getArmor() const { return armor; }

const Attack& Unit::getAttack() const { return attack; }

void Unit::takeDamage(int inComingDamage) {
  int effectiveeDamage = armor.reduceDamage(inComingDamage);
  health.decrease(effectiveeDamage);
}

bool Unit::isAlive() const { return !health.isZero(); }