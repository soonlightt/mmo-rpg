#pragma once

#include <string>
#include <utility>

#include "Commands/ICommand.h"
#include "Mediator/GameMediator.h"

class BuildCommand : public ICommand {
 private:
  GameMediator& mediator;
  std::string buildingType;
  int x;
  int y;
  int cost;
  int builtBuildingId = -1;
  bool executed = false;

 public:
  BuildCommand(GameMediator& med, std::string type, int posX, int posY, int buildSoct = 50);

  bool execute() override;
  bool undo() override;
  std::string getName() const override;
};