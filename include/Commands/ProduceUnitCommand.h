#pragma once

#include <string>

#include "Commands/ICommand.h"
#include "Mediator/GameMediator.h"

class ProduceUnitCommand : public ICommand {
 private:
  GameMediator& mediator;
  std::string unitType;
  int buildingId;
  int cost;
  bool executed = false;

 public:
  ProduceUnitCommand(GameMediator& med, std::string type, int bId, int unitCost = 50);

  bool execute() override;
  bool undo() override;
  std::string getName() const override;
};