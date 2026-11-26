#pragma once

#include <string>

#include "Commands/ICommand.h"
#include "Mediator/GameMediator.h"

class AttackUnitCommand : public ICommand {
 private:
  GameMediator& mediator;
  int attackerId;
  int targetId;

 public:
  AttackUnitCommand(GameMediator& med, int attacker, int target);

  bool execute() override;
  bool undo() override;
  std::string getName() const override;
};