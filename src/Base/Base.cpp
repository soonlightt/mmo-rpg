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

  if (hasUpgrade("attack_upgrade")) {
    rawUnit->modifyAttack(5);
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

bool Base::registerUnit(const std::shared_ptr<Unit>& unit) {
  if (!unit || isDestroyed() || units.size() >= maxUnits) {
    return false;
  }
  auto it = std::find(units.begin(), units.end(), unit);
  if (it != units.end()) {
    return false;
  }
  if (hasUpgrade("attack_upgrade")) {
    unit->modifyAttack(5);
  }
  unit->addObserver(this);
  units.push_back(unit);
  return true;
}

std::shared_ptr<Building> Base::buildBuilding(const std::string& type, int buildX, int buildY,
                                              int cost) {
  if (isDestroyed() || type.empty() || !spendResources(cost)) {
    return nullptr;
  }

  auto building = std::make_shared<Building>(type, buildX, buildY);
  buildings.push_back(building);

  std::cout << "[Base] Built " << type << " (ID: " << building->getId() << ") at (" << buildX
            << " ," << buildY << "). Remaining resources: " << resources << std::endl;

  return building;
}

std::shared_ptr<Building> Base::findBuildingById(int buildingId) const {
  for (const auto& b : buildings) {
    if (b && b->getId() == buildingId) {
      return b;
    }
  }
  return nullptr;
}

bool Base::removeBuilding(int buildingId) {
  auto it = std::find_if(
      buildings.begin(), buildings.end(),
      [buildingId](const std::shared_ptr<Building>& b) { return b && b->getId() == buildingId; });
  if (it != buildings.end()) {
    buildings.erase(it);
    return true;
  }
  return false;
}

const std::vector<std::shared_ptr<Building>>& Base::getBuildings() const { return buildings; }

bool Base::hasUpgrade(const std::string& techId) const {
  return researchedUpgrades.find(techId) != researchedUpgrades.end();
}

bool Base::research(const std::string& techId, int cost) {
  if (isDestroyed() || techId.empty() || hasUpgrade(techId) || !spendResources(cost)) {
    return false;
  }

  researchedUpgrades.insert(techId);

  if (techId == "attack_upgrade") {
    for (auto& u : units) {
      if (u && u->isAlive()) {
        u->modifyAttack(5);
      }
    }
  }

  std::cout << "[Base] Researched technology '" << techId << "'. Remaining resources: " << resources
            << std::endl;
  return true;
}

const std::set<std::string>& Base::getResearchedUpgrades() const { return researchedUpgrades; }

bool Base::distributeResources(const std::string& resourceType, const std::string& targetName,
                               int amount) {
  if (isDestroyed() || amount <= 0 || resourceType.empty() || targetName.empty()) {
    return false;
  }

  bool targetExists = std::any_of(buildings.begin(), buildings.end(),
                                  [&targetName](const std::shared_ptr<Building>& b) {
                                    return b && b->getType() == targetName;
                                  });

  if (!targetExists || !spendResources(amount)) {
    return false;
  }

  buildingResources[targetName + ";" + resourceType] += amount;
  std::cout << "[Base] Distributed " << amount << " " << resourceType << " to " << targetName
            << ". remaining base resources: " << resources << std::endl;
  return true;
}

const std::map<std::string, int>& Base::getAllocatedResources() const { return buildingResources; }