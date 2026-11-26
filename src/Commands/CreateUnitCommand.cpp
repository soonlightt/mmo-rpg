#include "Commands/CreateUnitCommand.h"

CreateUnitCommand::CreateUnitCommand(GameMediator& med, std::string type, int posX, int posY,
                                     int unitCost)
    : mediator(med), unitType(type), x(posX), y(posY), cost(unitCost) {}

bool CreateUnitCommand::execute() {
  executed = mediator.createUnit(unitType, x, y, cost);
  return executed;
}

bool CreateUnitCommand::undo() {
  if (!executed) {
    return false;
  }
  Unit* u = mediator.getFeild().getUnit(x, y);
  if (u) {
    if (mediator.getBase()) {
      mediator.getBase()->removeUnit(u);
      mediator.getBase()->addResources(cost);
    }
    mediator.getFeild().removeUnit(x, y);
    executed = false;
    return true;
  }
  return false;
}

std::string CreateUnitCommand::getName() const {
  return "craete " + unitType + " at (" + std::to_string(x) + ", " + std::to_string(y) + ")";
}