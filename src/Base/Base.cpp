#include "Base/Base.h"

#include <algorithm>
#include <iostream>

Base::Base(int startX, int startY, int initialHealth, size_t maxUnitsLimit, int defense,
           int initialResources)
    : x(startX),
      y(startY),
      health(std::max(0, initialHealth)),
      maxHealth(std::max(0, initialHealth)),
      maxUnits(maxUnitsLimit),
      defenseLevel(std::max(0, defense)),
      resources(std::max(0, initialResources)) {}

int Base::getX() const { return x; }

int Base::getY() const { return y; }

int Base::getHealth() const { return health; }

int Base::getMaxHealth() const { return maxHealth; }

size_t Base::getMaxUnits() const { return maxUnits; }

size_t Base::getActiveUnitsCount() const { return units.size(); }

int Base::getDefenseLevel() const { return defenseLevel; }

int Base::getResources() const { return resources; }

bool Base::isDestroyed() const { return health == 0; }

void Base::addResources(int amount) {
  if (amount > 0) {
    resources += amount;
  }
}

bool Base::spendResources(int amount) {
  if (amount < 0 || resources < amount) {
    return false;
  }
  resources -= amount;
  return true;
}

void Base::takeDamage(int inComingDamage) {
  if (inComingDamage <= 0 || isDestroyed()) {
    return;
  }
  int effectiveDamage = std::max(0, inComingDamage - defenseLevel);
  health = std::max(0, health - effectiveDamage);
}

std::shared_ptr<Unit> Base::createUnit(const UnitFactory& factory, int subType, int unitX,
                                       int unitY) {
  if (isDestroyed() || units.size() >= maxUnits) {
    return nullptr;
  }

  std::unique_ptr<Unit> rawUnit = factory.createUnit(subType, unitX, unitY);
  if (!rawUnit) {
    return nullptr;
  }

  std::shared_ptr<Unit> unit = std::move(rawUnit);
  unit->addObserver(this);
  units.push_back(unit);

  std::cout << "[Base] Created unit:" << unit->getType() << " at (" << unitX << ", " << unitY
            << "). Active units: " << units.size() << "/" << maxUnits << std::endl;

  return unit;
}

bool Base::removeUnit(const Unit* targetUnit) {
  if (!targetUnit) {
    return false;
  }

  auto it = std::find_if(units.begin(), units.end(), [targetUnit](const std::shared_ptr<Unit>& u) {
    return u.get() == targetUnit;
  });

  if (it != units.end()) {
    units.erase(it);
    return true;
  }

  return false;
}

const std::vector<std::shared_ptr<Unit>>& Base::getUnits() const { return units; }

void Base::onUnitEvent(const UnitEvent& event) {
  switch (event.type) {
    case UnitEventType::Created:
      std::cout << "[Observer -> Base] Unit created: " << event.unitName << " (ID: " << event.unitID
                << ")" << std::endl;
      break;

    case UnitEventType::Damaged:
      std::cout << "[Observer -> Base] Unit " << event.unitName << " (ID: " << event.unitID
                << ") took " << event.damage << " damage." << std::endl;
      break;

    case UnitEventType::Destroyed:
      std::cout << "[Observer -> Base] Unit " << event.unitName << " (ID: " << event.unitID
                << ") was destroyed! Freeing unit slot..." << std::endl;
      units.erase(
          std::remove_if(units.begin(), units.end(),
                         [](const std::shared_ptr<Unit>& u) { return !u || !u->isAlive(); }),
          units.end());
      std::cout << "[Base] Active units remaining: " << units.size() << "/" << maxUnits
                << std::endl;
      break;
  }
}