#pragma once
#include <memory>

#include "Units/Unit.h"

class UnitFactory {
 public:
  virtual std::unique_ptr<Unit> createUnit(int subTyoe, int x, int y) const = 0;

  virtual ~UnitFactory() = default;
};