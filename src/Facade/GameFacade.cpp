#include "Facade/GameFacade.h"

namespace {
bool parseCoords(const std::string& token, int& outX, int& outY) {
  std::string cleaned = token;
  std::replace(cleaned.begin(), cleaned.end(), ',', ' ');
  std::istringstream iss(cleaned);
  return static_cast<bool>(iss >> outX >> outY);
}
}  // namespace

GameFacade::GameFacade(int width, int height, size_t maxUnits, int baseX, int baseY, int baseHealth,
                       size_t baseMaxUnits, int baseResources)
    : mediator(width, height, maxUnits, baseX, baseY, baseHealth, baseMaxUnits, baseResources) {}

GameMediator& GameFacade::getMediator() { return mediator; }
const GameMediator& GameFacade::getMediator() const { return mediator; }

bool GameFacade::runCommand(std::unique_ptr<ICommand> cmd, bool recordInHistory) {
  if (!cmd) {
    return false;
  }
  if (cmd->execute()) {
    if (recordInHistory) {
      history.push_back(std::move(cmd));
    }
    return true;
  }
  return false;
}

bool GameFacade::createUnit(const std::string& unitType, int x, int y, int cost) {
  return runCommand(std::make_unique<CreateUnitCommand>(mediator, unitType, x, y, cost));
}

bool GameFacade::moveUnit(int unitId, int toX, int toY) {
  return runCommand(std::make_unique<MoveUnitCommand>(mediator, unitId, toX, toY));
}

bool GameFacade::attackUnit(int attackerId, int targetId) {
  return runCommand(std::make_unique<AttackUnitCommand>(mediator, attackerId, targetId));
}

bool GameFacade::collectResource(int unitId, int resourceId) {
  return runCommand(std::make_unique<CollectResourceCommand>(mediator, unitId, resourceId));
}

bool GameFacade::healUnit(int healerId, int targetId) {
  return runCommand(std::make_unique<HealCommand>(mediator, healerId, targetId));
}

bool GameFacade::buildBuilding(const std::string& buildingType, int x, int y, int cost) {
  return runCommand(std::make_unique<BuildCommand>(mediator, buildingType, x, y, cost));
}

bool GameFacade::research(const std::string& techId, int cost) {
  return runCommand(std::make_unique<ResearchCommand>(mediator, techId, cost));
}

bool GameFacade::distributeResources(const std::string& resourceType, const std::string& targetName,
                                     int amount) {
  return runCommand(
      std::make_unique<DistributeResourcesCommand>(mediator, resourceType, targetName, amount));
}

bool GameFacade::produceUnit(const std::string& unitType, int buildingId, int cost) {
  return runCommand(std::make_unique<ProduceUnitCommand>(mediator, unitType, buildingId, cost));
}

void GameFacade::showStats(const std::string& filter) const {
  std::cout << mediator.getStats(filter);
}

bool GameFacade::undoLastCommand() {
  if (history.empty()) {
    std::cout << "[Facade] Undo failed: command history is empty.\n";
    return false;
  }
  std::string cmdName = history.back()->getName();
  if (history.back()->undo()) {
    std::cout << "[Facade] Undid command: " << cmdName << "\n";
    history.pop_back();
    return true;
  }
  std::cout << "[Facade] Command '" << cmdName << "' cannot be undone.\n";
  return false;
}

void GameFacade::printHistory() const {
  std::cout << "=== Command History (" << history.size() << ") ===\n";
  for (size_t i = 0; i < history.size(); ++i) {
    std::cout << "  " << (i + 1) << ". " << history[i]->getName() << "\n";
  }
}

void GameFacade::printWorld() const { mediator.printWorld(); }

void GameFacade::printHelp() const {
  std::cout << "Available commands:\n"
            << "  create <unitType> at <x>,<y>\n"
            << "  move unit <id> to <x>,<y>\n"
            << "  attack unit <attackerId> -> <targetId>\n"
            << "  collect resource <resId> by unit <unitId>\n"
            << "  heal unit <targetId> by <healerId>\n"
            << "  build <buildingType> at <x>,<y>\n"
            << "  research <techId>\n"
            << "  distribute <amount> <resType> to <targetBuilding>\n"
            << "  produce <unitType> in <buildingId>\n"
            << "  stats [units|buildings|resources]\n"
            << "  undo | history | map | help | exit\n";
}

bool GameFacade::handleCreate(std::istringstream& iss) {
  std::string unitType, kwAt, coordToken;
  int x = 0, y = 0;
  if (iss >> unitType >> kwAt >> coordToken && parseCoords(coordToken, x, y)) {
    return createUnit(unitType, x, y);
  }
  std::cout << "[Facade] Usage: create <unitType> at <x>,<y>\n";
  return false;
}

bool GameFacade::handleMove(std::istringstream& iss) {
  std::string kwUnit, kwTo, coordToken;
  int id = 0, x = 0, y = 0;
  if (iss >> kwUnit >> id >> kwTo >> coordToken && parseCoords(coordToken, x, y)) {
    return moveUnit(id, x, y);
  }
  std::cout << "[Facade] Usage: move unit <id> to <x>,<y>\n";
  return false;
}

bool GameFacade::handleAttack(std::istringstream& iss) {
  std::string kwUnit, arrow;
  int attackerId = 0, targetId = 0;
  if (iss >> kwUnit >> attackerId >> arrow >> targetId) {
    return attackUnit(attackerId, targetId);
  }
  std::cout << "[Facade] Usage: attack unit <attackerId> -> <targetId>\n";
  return false;
}

bool GameFacade::handleCollect(std::istringstream& iss) {
  std::string kwRes, kwBy, kwUnit;
  int resId = 0, unitId = 0;
  if (iss >> kwRes >> resId >> kwBy >> kwUnit >> unitId) {
    return collectResource(unitId, resId);
  }
  std::cout << "[Facade] Usage: collect resource <resId> by unit <unitId>\n";
  return false;
}

bool GameFacade::handleHeal(std::istringstream& iss) {
  std::string kwUnit, kwBy;
  int targetId = 0, healerId = 0;
  if (iss >> kwUnit >> targetId >> kwBy >> healerId) {
    return healUnit(healerId, targetId);
  }
  std::cout << "[Facade] Usage: heal unit <targetId> by <healerId>\n";
  return false;
}

bool GameFacade::handleBuild(std::istringstream& iss) {
  std::string buildingType, kwAt, coordToken;
  int x = 0, y = 0;
  if (iss >> buildingType >> kwAt >> coordToken && parseCoords(coordToken, x, y)) {
    return buildBuilding(buildingType, x, y);
  }
  std::cout << "[Facade] Usage: build <buildingType> at <x>,<y>\n";
  return false;
}

bool GameFacade::handleResearch(std::istringstream& iss) {
  std::string techId;
  if (iss >> techId) {
    return research(techId);
  }
  std::cout << "[Facade] Usage: research <techId>\n";
  return false;
}

bool GameFacade::handleDistribute(std::istringstream& iss) {
  int amount = 0;
  std::string resType, kwTo, targetName;
  if (iss >> amount >> resType >> kwTo >> targetName) {
    return distributeResources(resType, targetName, amount);
  }
  std::cout << "[Facade] Usage: distribute <amount> <resType> to <targetBuilding>\n";
  return false;
}

bool GameFacade::handleProduce(std::istringstream& iss) {
  std::string unitType, kwIn;
  int buildingId = 0;
  if (iss >> unitType >> kwIn >> buildingId) {
    return produceUnit(unitType, buildingId);
  }
  std::cout << "[Facade] Usage: produce <unitType> in <buildingId>\n";
  return false;
}

bool GameFacade::handleStats(std::istringstream& iss) {
  std::string filter;
  iss >> filter;
  return runCommand(std::make_unique<StatsCommand>(mediator, filter), false);
}

bool GameFacade::executeCommand(const std::string& input) {
  std::istringstream iss(input);
  std::string action;
  if (!(iss >> action)) {
    return false;
  }

  if (action == "create") return handleCreate(iss);
  if (action == "move") return handleMove(iss);
  if (action == "attack") return handleAttack(iss);
  if (action == "collect") return handleCollect(iss);
  if (action == "heal") return handleHeal(iss);
  if (action == "build") return handleBuild(iss);
  if (action == "research") return handleResearch(iss);
  if (action == "distribute") return handleDistribute(iss);
  if (action == "produce") return handleProduce(iss);
  if (action == "stats") return handleStats(iss);

  if (action == "undo") return undoLastCommand();
  if (action == "history") {
    printHistory();
    return true;
  }
  if (action == "map") {
    printWorld();
    return true;
  }
  if (action == "help") {
    printHelp();
    return true;
  }

  std::cout << "[Facade] Unknown command '" << action << "'. Type 'help' for commands list.\n";
  return false;
}