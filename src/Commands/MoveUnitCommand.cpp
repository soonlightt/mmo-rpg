#include "Commands/MoveUnitCommand.h"

MoveUnitCommand::MoveUnitCommand(GameMediator& med, int id, int targetX, int targetY)
    : mediator(med), unitId(id), toX(targetX), toY(targetY) {}

bool MoveUnitCommand::execute() {
  for (auto& u : mediator.getField()) {
    if (u.getId() == unitId) {
      prevX = u.getX();
      prevY = u.getY();
      break;
    }
  }
  executed = mediator.moveUnit(unitId, toX, toY);
  return executed;
}

bool MoveUnitCommand::undo() {
  if (!executed || prevX <= 0 || prevY <= 0) {
    return false;
  }
  if (mediator.moveUnit(unitId, prevX, prevY)) {
    executed = false;
    return true;
  }
  return false;
}

std::string MoveUnitCommand::getName() const {
  return "move unit " + std::to_string(unitId) + " to (" + std::to_string(toX) + ", " +
         std::to_string(toY) + ")";
}