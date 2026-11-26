#include "Commands/BuildCommand.h"

BuildCommand::BuildCommand(GameMediator& med, std::string type, int posX, int posY, int buildCost)
    : mediator(med), buildingType(std::move(type)), x(posX), y(posY), cost(buildCost) {}

bool BuildCommand::execute() {
  executed = mediator.buildBuilding(buildingType, x, y, cost);
  if (executed && mediator.getBase() && !mediator.getBase()->getBuildings().empty()) {
    builtBuildingId = mediator.getBase()->getBuildings().back()->getId();
  }
  return executed;
}

bool BuildCommand::undo() {
  if (!executed || builtBuildingId < 0 || !mediator.getBase()) {
    return false;
  }
  if (mediator.getBase()->removeBuilding(builtBuildingId)) {
    mediator.getBase()->addResources(cost);
    executed = false;
    return true;
  }
  return false;
}

std::string BuildCommand::getName() const {
  return "build " + buildingType + " at (" + std::to_string(x) + ", " + std::to_string(y) + ")";
}