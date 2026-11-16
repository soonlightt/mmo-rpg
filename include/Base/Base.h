#pragma once

#include <memory>
#include <vector>

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
  const std::vector<std::shared_ptr<Unit>>& getUnits() const;

  void onUnitEvent(const UnitEvent& event) override;
};