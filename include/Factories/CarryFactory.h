#pragma once

#include "Factories/UnitFactory.h"

class CarryFactory : public UnitFactory {
 public:
  ~CarryFactory() override = default;

  std::unique_ptr<Unit> createUnit(int subType, int x, int y) const override;
};