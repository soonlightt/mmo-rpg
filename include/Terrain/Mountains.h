#pragma once

#include <string>

#include "Terrain/Terrain.h"

class Mountains : public Terrain {
 public:
  Mountains(int x, int y);
  ~Mountains() override = default;

  std::string getType() const override;
  char getSymbol() const override;
  bool isPassable() const override;
  std::string getEffectDescription() const override;

  void onEnter(Unit& unit) override;
  void onLeave(Unit& unit) override;
};