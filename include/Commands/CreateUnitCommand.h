#pragma once

#include <string>
#include <utility>

#include "Commands/ICommand.h"
#include "Mediator/GameMediator.h"

class CreateUnitCommand : public ICommand {
 private:
  GameMediator& mediator;
  std::string unitType;
  int x;
  int y;
  int cost;
  bool executed = false;

 public:
  CreateUnitCommand(GameMediator& med, std::string type, int posX, int posY, int unitCost = 50);

  bool execute() override;
  bool undo() override;
  std::string getName() const override;
};