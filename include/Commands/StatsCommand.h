#pragma once

#include <iostream>
#include <string>
#include <utility>

#include "Commands/ICommand.h"
#include "Mediator/GameMediator.h"

class StatsCommand : public ICommand {
 private:
  GameMediator& mediator;
  std::string filter;

 public:
  explicit StatsCommand(GameMediator& med, std::string statsFilter = "");

  bool execute() override;
  bool undo() override;
  std::string getName() const override;
};