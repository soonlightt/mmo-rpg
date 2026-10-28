#pragma once

#include "Factories/UnitFactory.h"

class MidderFactory : public UnitFactory {
 public:
  ~MidderFactory() override = default;

  std::unique_ptr<Unit> createUnit(int subType, int x, int y) const override;
};