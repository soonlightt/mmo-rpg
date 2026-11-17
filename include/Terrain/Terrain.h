#pragma once

#include <string>

#include "Units/Unit.h"

class Terrain {
 protected:
  int x;
  int y;

 public:
  Terrain(int x, int y) : x(x), y(y) {}
  virtual ~Terrain() = default;

  int getX() const { return x; }
  int getY() const { return y; }

  virtual std::string getType() const = 0;
  virtual char getSymbol() const = 0;
  virtual bool isPassable() const = 0;
  virtual std::string getEffectDescription() const = 0;

  virtual void onEnter(Unit& unit) = 0;
  virtual void onLeave(Unit& unit) = 0;
};