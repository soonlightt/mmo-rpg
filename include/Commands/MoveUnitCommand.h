#pragma once

#include <string>

#include "Commands/ICommand.h"
#include "Mediator/GameMediator.h"

class MoveUnitCommand : public ICommand {
 private:
  GameMediator& mediator;
  int unitId;
  int toX;
  int toY;
  int prevX = -1;
  int prevY = -1;
  bool executed = false;

 public:
  MoveUnitCommand(GameMediator& med, int id, int targetX, int targetY);

  bool execute() override;
  bool undo() override;
  std::string getName() const override;
};