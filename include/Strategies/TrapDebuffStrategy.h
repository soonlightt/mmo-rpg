#pragma once

#include <algorithm>
#include <iostream>
#include <string>

#include "Strategies/IInteractionStrategy.h"
#include "Units/Unit.h"

class TrapDebuffStrategy : public IInteractionStrategy {
 public:
  void apply(Unit& unit) override;
  std::string getName() const override;
};