#include "Commands/DistributeResourcesCommand.h"

DistributeResourcesCommand::DistributeResourcesCommand(GameMediator& med, std::string resType,
                                                       std::string target, int resAmount)
    : mediator(med),
      resourceType(std::move(resType)),
      targetName(std::move(target)),
      amount(resAmount) {}

bool DistributeResourcesCommand::execute() {
  executed = mediator.distributeResources(resourceType, targetName, amount);
  return executed;
}

bool DistributeResourcesCommand::undo() {
  if (!executed || !mediator.getBase()) {
    return false;
  }
  mediator.getBase()->addResources(amount);
  executed = false;
  return true;
}

std::string DistributeResourcesCommand::getName() const {
  return "distribute " + std::to_string(amount) + " " + resourceType + " to " + targetName;
}