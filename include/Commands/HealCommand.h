#pragma once

#include "Commands/ICommand.h"
#include "Mediator/GameMediator.h"

class HealCommand : public ICommand {
 private:
  GameMediator& mediator;
  int healerId;
  int targetId;
  int prevHp = -1;
  bool executed = false;

 public:
  HealCommand(GameMediator& med, int healer, int target);

  bool execute() override;
  bool undo() override;
  std::string getName() const override;
};