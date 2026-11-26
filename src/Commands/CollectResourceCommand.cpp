#include "Commands/CollectResourceCommand.h"

CollectResourceCommand::CollectResourceCommand(GameMediator& med, int unit, int resource)
    : mediator(med), unitId(unit), resourceId(resource) {}

bool CollectResourceCommand::execute() {
  executed = mediator.collectResource(unitId, resourceId);
  return executed;
}

bool CollectResourceCommand::undo() {
  if (!executed || !mediator.getBase()) {
    return false;
  }
  if (mediator.getBase()->spendResources(30)) {
    executed = false;
    return true;
  }
  return false;
}

std::string CollectResourceCommand::getName() const {
  return "collect resource " + std::to_string(resourceId) + " by unit " + std::to_string(unitId);
}