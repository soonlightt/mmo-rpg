#pragma once

#include "Units/Unit.h"

class Midder : public Unit {
 public:
  Midder(int x, int y, const Health& h, const Armor& arm, const Attack& att)
      : Unit(x, y, h, arm, att) {}

  virtual ~Midder() = default;
};