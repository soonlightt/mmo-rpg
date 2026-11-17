#pragma once

#include <string>

#include "Terrain/Terrain.h"

class Plains : public Terrain {
 public:
  Plains(int x, int y);
  ~Plains() override = default;

  std::string getType() const override;
  char getSymbol() const override;
  bool isPassable() const override;
  std::string getEffectDescription() const override;

  void onEnter(Unit& unit) override;
  void onLeave(Unit& unit) override;
};