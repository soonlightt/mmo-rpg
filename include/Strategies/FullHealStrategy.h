#pragma once

#include <iostream>
#include <string>

#include "Strategies/IInteractionStrategy.h"
#include "Units/Unit.h"

class FullHealStrategy : public IInteractionStrategy {
 public:
  void apply(Unit& unit) override;
  std::string getName() const override;
};