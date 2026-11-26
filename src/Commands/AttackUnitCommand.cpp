#include "Commands/AttackUnitCommand.h"

AttackUnitCommand::AttackUnitCommand(GameMediator& med, int attacker, int target)
    : mediator(med), attackerId(attacker), targetId(target) {}

bool AttackUnitCommand::execute() { return mediator.attackUnit(attackerId, targetId); }

bool AttackUnitCommand::undo() { return false; }

std::string AttackUnitCommand::getName() const {
  return "attack unit " + std::to_string(attackerId) + " -> " + std::to_string(targetId);
}