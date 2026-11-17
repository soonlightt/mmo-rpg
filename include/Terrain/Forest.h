#pragma once

#include <string>

#include "Terrain/Terrain.h"

class Forest : public Terrain {
 public:
  Forest(int x, int y);
  ~Forest() override = default;

  std::string getType() const override;
  char getSymbol() const override;
  bool isPassable() const override;
  std::string getEffectDescription() const override;

  void onEnter(Unit& unit) override;
  void onLeave(Unit& unit) override;
};