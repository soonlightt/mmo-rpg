#include "Commands/HealCommand.h"

HealCommand::HealCommand(GameMediator& med, int healer, int target)
    : mediator(med), healerId(healer), targetId(target) {}

bool HealCommand::execute() {
  Unit* target = nullptr;
  for (auto& u : mediator.getFeild()) {
    if (u.getId() == targetId) {
      target = &u;
      break;
    }
  }
  if (target) {
    prevHp = target->getHealth().getValue();
  }
  executed = mediator.healUnit(healerId, targetId);
  return executed;
}

bool HealCommand::undo() {
  if (!executed || prevHp <= 0) {
    return false;
  }
  for (auto& u : mediator.getFeild()) {
    if (u.getId() == targetId && u.isAlive()) {
      int diff = u.getHealth().getValue() - prevHp;
      if (diff > 0) {
        u.takeDamage(diff + u.getArmor().getValue());
      }
      executed = false;
      return true;
    }
  }
  return false;
}

std::string HealCommand::getName() const {
  return "heal unit " + std::to_string(targetId) + " by " + std::to_string(healerId);
}