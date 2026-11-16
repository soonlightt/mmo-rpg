#include "Units/Unit.h"

int Unit::id = 1;

Unit::Unit(int startX, int startY, const Health& h, const Armor& arm, const Attack& att)
    : x(startX), y(startY), health(h), armor(arm), attack(att) {}

int Unit::getId() const { return id; }

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
  int effectiveDamage = armor.reduceDamage(inComingDamage);
  health.decrease(effectiveDamage);

  notify(
      {UnitEventType::Damaged, getType(), id, effectiveDamage, std::chrono::system_clock::now()});

  if (!isAlive()) {
    notify({UnitEventType::Destroyed, getType(), id, 0, std::chrono::system_clock::now()});
  }
}

void Unit::heal(int amount) {
  if (isAlive() && amount > 0) {
    health.increase(amount);
  }
}

bool Unit::isAlive() const { return !health.isZero(); }