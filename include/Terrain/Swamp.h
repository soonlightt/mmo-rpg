#pragma once

#include <string>

#include "Terrain/Terrain.h"

class Swamp : public Terrain {
 public:
  Swamp(int x, int y);
  ~Swamp() override = default;

  std::string getType() const override;
  char getSymbol() const override;
  bool isPassable() const override;
  std::string getEffectDescription() const override;

  void onEnter(Unit& unit) override;
  void onLeave(Unit& unit) override;
};