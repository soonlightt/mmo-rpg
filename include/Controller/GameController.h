#pragma once

#include <iostream>
#include <memory>
#include <string>

#include "Base/Base.h"
#include "Factories/UnitFactory.h"
#include "Field/GameField.h"
#include "Terrain/TerrainMap.h"

class GameController {
 private:
  std::shared_ptr<Base> base;
  GameField field;
  TerrainMap terrainMap;

 public:
  GameController(int width = 15, int height = 15, size_t maxUnits = 20, int baseX = 0,
                 int baseY = 0, int baseHealth = 500, size_t baseMaxUnits = 10,
                 int baseResources = 200);
  ~GameController() {
    for (auto& unit : field) {
      if (base) {
        unit.removeObserver(base.get());
      }
    }
    field.clear();
  }

  GameField& getField();
  const GameField& getField() const;

  TerrainMap& getTerrainMap();
  const TerrainMap& getTerrainMap() const;

  std::shared_ptr<Base> getBase() const;

  bool spawnUnit(const UnitFactory& factory, int subType, int x, int y, int cost = 50);

  bool moveUnit(int fromX, int fromY, int toX, int toY);

  bool attackUnit(int attackerX, int attackerY, int targetX, int targetY);

  void printWorld() const;
};