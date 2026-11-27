#pragma once

#include <string>

#include "Commands/ICommand.h"
#include "Mediator/GameMediator.h"

class CollectResourceCommand : public ICommand {
 private:
  GameMediator& mediator;
  int unitId;
  int resourceId;
  bool executed = false;

 public:
  CollectResourceCommand(GameMediator& med, int unit, int resource);

  bool execute() override;
  bool undo() override;
  std::string getName() const override;
};