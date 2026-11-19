#pragma once

#include <string>

#include "Base/Base.h"
#include "Field/GameField.h"
#include "NeutralObjects/NeutralObject.h"
#include "Terrain/TerrainMap.h"

class Teleport : public NeutralObject {
 public:
  Teleport(int x, int y, int areaWidth = 15, int areaHeight = 15, const TerrainMap* map = nullptr,
           const Base* base = nullptr, const GameField* field = nullptr);
  ~Teleport() override = default;

  std::string getType() const override;
  char getSymbol() const override;
  std::string getColoredSymbol() const override;
};