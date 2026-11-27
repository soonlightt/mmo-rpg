#pragma once

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "Base/Base.h"
#include "Factories/CarryFactory.h"
#include "Factories/MidderFactory.h"
#include "Factories/SupportFactory.h"
#include "Field/GameField.h"
#include "Terrain/TerrainMap.h"

class GameMediator {
 private:
  std::shared_ptr<Base> base;
  GameField field;
  TerrainMap terrainMap;

  CarryFactory carryFactory;
  MidderFactory midderFactory;
  SupportFactory supportFactory;

  bool resolveUnitType(const std::string& unitType, const UnitFactory*& outFactory,
                       int& outSubType) const;
  Unit* findUnitById(int unitId) const;
  bool isBuildingAt(int x, int y) const;

 public:
  GameMediator(int width = 15, int height = 15, size_t maxUnits = 20, int baseX = 0, int baseY = 0,
               int baseHealth = 500, size_t baseMaxUnits = 10, int baseResources = 200);
  ~GameMediator() { field.clear(); }

  GameField& getField();
  const GameField& getField() const;
  TerrainMap& getTerrainMap();
  const TerrainMap& getTerrainMap() const;
  std::shared_ptr<Base> getBase() const;

  bool createUnit(const std::string& unitType, int x, int y, int cost = 50);
  bool moveUnit(int unitId, int toX, int toY);
  bool moveUnitByCoords(int fromX, int fromY, int toX, int toY);
  bool attackUnit(int attackerId, int targetId);
  bool collectResource(int unitId, int resourceId);
  bool healUnit(int healerId, int targetId);

  bool buildBuilding(const std::string& buildingType, int x, int y, int cost = 50);
  bool research(const std::string& techId, int cost = 40);
  bool distributeResources(const std::string& resourceType, const std::string& targetName,
                           int amount);
  bool produceUnit(const std::string& unitType, int buildingId, int cost = 50);

  std::string getStats(const std::string& filter = "") const;
  void printWorld() const;
};