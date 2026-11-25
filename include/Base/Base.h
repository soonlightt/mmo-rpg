#pragma once

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "Buildings/Building.h"
#include "Factories/UnitFactory.h"
#include "Observer/IObserver.h"
#include "Units/Unit.h"

class Base : public IObserver {
 private:
  int x;
  int y;
  int health;
  int maxHealth;
  size_t maxUnits;
  int defenseLevel;
  int resources;
  std::vector<std::shared_ptr<Unit>> units;
  std::vector<std::shared_ptr<Building>> buildings;
  std::set<std::string> researchedUpgrades;
  std::map<std::string, int> buildingResources;

 public:
  Base(int startX, int startY, int initialHealth = 500, size_t maxUnitsLimit = 10, int defense = 5,
       int initialResources = 100);

  ~Base() override = default;

  int getX() const;
  int getY() const;
  int getHealth() const;
  int getMaxHealth() const;
  size_t getMaxUnits() const;
  size_t getActiveUnitsCount() const;
  int getDefenseLevel() const;
  int getResources() const;
  bool isDestroyed() const;

  void addResources(int amount);
  bool spendResources(int amount);
  void takeDamage(int inComingDamage);

  std::shared_ptr<Unit> createUnit(const UnitFactory& factory, int subType, int unitX, int unitY);
  bool removeUnit(const Unit* targetUnit);
  bool registerUnit(const std::shared_ptr<Unit>& unit);
  const std::vector<std::shared_ptr<Unit>>& getUnits() const;

  void onUnitEvent(const UnitEvent& event) override;

  std::shared_ptr<Building> buildBuilding(const std::string& type, int buildX, int buildY,
                                          int cost = 50);

  std::shared_ptr<Building> findBuildingById(int buildingId) const;
  const std::vector<std::shared_ptr<Building>>& getBuildings() const;

  bool hasUpgrade(const std::string& techId) const;
  bool research(const std::string& techId, int cost = 40);
  const std::set<std::string>& getResearchedUpgrades() const;

  bool distributeResources(const std::string& resourceType, const std::string& targetName,
                           int amount);
  const std::map<std::string, int>& getAllocatedResources() const;
};