#include "Strategies/PowerBoostStrategy.h"

void PowerBoostStrategy::apply(Unit& unit) {
  int bonus = std::max(1, static_cast<int>(unit.getAttack().getValue() * 0.15));
  unit.modifyAttack(bonus);
  std::cout << "  -> " << unit.getType() << " gained +" << bonus
            << " attack (+15%)! New attack: " << unit.getAttack().getValue() << std::endl;
}

std::string PowerBoostStrategy::getName() const { return "PowerBoost (+15% Attack)"; }