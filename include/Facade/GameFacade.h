#pragma once

#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "Commands/AttackUnitCommand.h"
#include "Commands/BuildCommand.h"
#include "Commands/CollectResourceCommand.h"
#include "Commands/CreateUnitCommand.h"
#include "Commands/DistributeResourcesCommand.h"
#include "Commands/HealCommand.h"
#include "Commands/ICommand.h"
#include "Commands/MoveUnitCommand.h"
#include "Commands/ProduceUnitCommand.h"
#include "Commands/ResearchCommand.h"
#include "Commands/StatsCommand.h"
#include "Mediator/GameMediator.h"

class GameFacade {
 private:
  GameMediator mediator;
  std::vector<std::unique_ptr<ICommand>> history;

  bool runCommand(std::unique_ptr<ICommand> cmd, bool recordInHistory = true);

  bool handleCreate(std::istringstream& iss);
  bool handleMove(std::istringstream& iss);
  bool handleAttack(std::istringstream& iss);
  bool handleCollect(std::istringstream& iss);
  bool handleHeal(std::istringstream& iss);
  bool handleBuild(std::istringstream& iss);
  bool handleResearch(std::istringstream& iss);
  bool handleDistribute(std::istringstream& iss);
  bool handleProduce(std::istringstream& iss);
  bool handleStats(std::istringstream& iss);

 public:
  GameFacade(int width = 15, int height = 15, size_t maxUnits = 20, int baseX = 0, int baseY = 0,
             int baseHealth = 500, size_t baseMaxUnits = 10, int baseResources = 300);

  GameMediator& getMediator();
  const GameMediator& getMediator() const;

  bool createUnit(const std::string& unitType, int x, int y, int cost = 50);
  bool moveUnit(int unitId, int toX, int toY);
  bool attackUnit(int attackerId, int targetId);
  bool collectResource(int unitId, int resourceId);
  bool healUnit(int healerId, int targetId);
  bool buildBuilding(const std::string& buildingType, int x, int y, int cost = 50);
  bool research(const std::string& techId, int cost = 40);
  bool distributeResources(const std::string& resourceType, const std::string& targetName,
                           int amount);
  bool produceUnit(const std::string& unitType, int buildingId, int cost = 50);
  void showStats(const std::string& filter = "") const;

  bool undoLastCommand();
  void printHistory() const;
  void printWorld() const;
  void printHelp() const;

  bool executeCommand(const std::string& input);
};