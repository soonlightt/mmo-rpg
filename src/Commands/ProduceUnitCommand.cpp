#include "Commands/ProduceUnitCommand.h"

ProduceUnitCommand::ProduceUnitCommand(GameMediator& med, std::string type, int bId, int unitCost)
    : mediator(med), unitType(std::move(type)), buildingId(bId), cost(unitCost) {}

bool ProduceUnitCommand::execute() {
  executed = mediator.produceUnit(unitType, buildingId, cost);
  return executed;
}

bool ProduceUnitCommand::undo() { return false; }

std::string ProduceUnitCommand::getName() const {
  return "produce " + unitType + " in building #" + std::to_string(buildingId);
}