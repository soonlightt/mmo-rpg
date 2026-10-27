#pragma once

#include "Units/Unit.h"

class Support : public Unit {
 public:
  Support(int x, int y, const Health& h, const Armor& arm, const Attack& att)
      : Unit(x, y, h, arm, att) {}
  virtual ~Support() = default;
};