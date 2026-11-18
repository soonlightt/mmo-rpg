#include "Strategies/TrapDebuffStrategy.h"

void TrapDebuffStrategy::apply(Unit& unit) {
  int armorLoss = (unit.getArmor().getValue() > 0)
                      ? std::max(1, static_cast<int>(unit.getArmor().getValue() * 0.05))
                      : 0;
  int attackLoss = (unit.getAttack().getValue() > 0)
                       ? std::max(1, static_cast<int>(unit.getAttack().getValue() * 0.05))
                       : 0;

  unit.modifyArmor(-armorLoss);
  unit.modifyAttack(-attackLoss);

  std::cout << "  -> " << unit.getType() << " triggered a trap! -" << armorLoss << " armor, -"
            << attackLoss << " attack (-5%). New stats -> Armor: " << unit.getArmor().getValue()
            << ", Attack: " << unit.getAttack().getValue() << std::endl;
}

std::string TrapDebuffStrategy::getName() const { return "TrapDebuff (-5% Armor & -5% Attack)"; }