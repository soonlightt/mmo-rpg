#pragma once

#include <string>
#include <utility>

#include "Commands/ICommand.h"
#include "Mediator/GameMediator.h"

class ResearchCommand : public ICommand {
 private:
  GameMediator& mediator;
  std::string techId;
  int cost;
  bool executed = false;

 public:
  ResearchCommand(GameMediator& med, std::string tech, int researchCost = 40);

  bool execute() override;
  bool undo() override;
  std::string getName() const override;
};