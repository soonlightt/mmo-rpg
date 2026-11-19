#pragma once

#include <iostream>
#include <string>

#include "Base/Base.h"
#include "Field/GameField.h"
#include "Strategies/IInteractionStrategy.h"
#include "Terrain/TerrainMap.h"
#include "Units/Unit.h"

class TeleportStrategy : public IInteractionStrategy {
 private:
  int areaWidth;
  int areaHeight;
  const TerrainMap* terrainMap;
  const Base* base;
  const GameField* field;

 public:
  TeleportStrategy(int width = 15, int height = 15, const TerrainMap* map = nullptr,
                   const Base* gameBase = nullptr, const GameField* gameField = nullptr);

  void apply(Unit& unit) override;
  std::string getName() const override;
};