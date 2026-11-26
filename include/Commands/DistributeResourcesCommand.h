#pragma once

#include <string>
#include <utility>

#include "Commands/ICommand.h"
#include "Mediator/GameMediator.h"

class DistributeResourcesCommand : public ICommand {
 private:
  GameMediator& mediator;
  std::string resourceType;
  std::string targetName;
  int amount;
  bool executed = false;

 public:
  DistributeResourcesCommand(GameMediator& med, std::string resType, std::string target,
                             int resAmount);

  bool execute() override;
  bool undo() override;
  std::string getName() const override;
};