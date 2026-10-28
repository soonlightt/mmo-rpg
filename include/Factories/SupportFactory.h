#pragma once

#include "Factories/UnitFactory.h"

class SupportFactory : public UnitFactory {
 public:
  ~SupportFactory() override = default;

  std::unique_ptr<Unit> createUnit(int subType, int x, int y) const override;
};