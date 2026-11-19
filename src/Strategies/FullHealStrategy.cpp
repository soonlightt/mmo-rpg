#include "Strategies/FullHealStrategy.h"

void FullHealStrategy::apply(Unit& unit) {
  int beforeHp = unit.getHealth().getValue();
  int maxHp = unit.getHealth().getMaxValue();

  unit.heal(maxHp);

  int healed = unit.getHealth().getValue() - beforeHp;
  std::cout << "  -> " << unit.getType() << " fully healed (+" << healed
            << " HP)! Current HP: " << unit.getHealth().getValue() << "/" << maxHp << std::endl;
}

std::string FullHealStrategy::getName() const { return "FullHeal (Health restored to maximum)"; }