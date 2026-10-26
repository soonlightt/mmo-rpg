#pragma once

#include "Units/Unit.h"

class Carry : public Unit {
 public:
  Carry(int x, int y, const Health& h, const Armor& arm, const Attack& att)
      : Unit(x, y, h, arm, att) {}

  virtual ~Carry() = default;
};