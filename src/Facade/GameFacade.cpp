#include "Facade/GameFacade.h"

GameFacade::GameFacade(int width, int height, size_t maxUnits, int baseX, int baseY, int baseHealth,
                       size_t baseMaxUnits, int baseResources)
    : mediator(width, height, maxUnits, baseX, baseY, baseHealth, baseMaxUnits, baseResources) {
  initHandlerChain();
}

void GameFacade::initHandlerChain() {
  auto create = std::make_unique<CreateHandler>();
  auto move = std::make_unique<MoveHandler>();
  auto attack = std::make_unique<AttackHandler>();
  auto collect = std::make_unique<CollectHandler>();
  auto heal = std::make_unique<HealHandler>();
  auto build = std::make_unique<BuildHandler>();
  auto research = std::make_unique<ResearchHandler>();
  auto produce = std::make_unique<ProduceHandler>();
  auto stats = std::make_unique<StatsHandler>();
  auto help = std::make_unique<HelpHandler>();

  create->setNext(std::move(move))
      ->setNext(std::move(attack))
      ->setNext(std::move(collect))
      ->setNext(std::move(heal))
      ->setNext(std::move(build))
      ->setNext(std::move(research))
      ->setNext(std::move(produce))
      ->setNext(std::move(stats))
      ->setNext(std::move(help));

  handlerChain = std::move(create);
}

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
  StatsCommand cmd(const_cast<GameMediator&>(mediator), filter);
  cmd.execute();
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

bool GameFacade::executeCommand(const std::string& input) {
  std::istringstream iss(input);
  std::string action;
  if (!(iss >> action)) {
    return false;
  }

  if (handlerChain && handlerChain->handle(input, *this)) {
    return true;
  }

  if (action != "create" && action != "move" && action != "attack" && action != "collect" &&
      action != "heal" && action != "build" && action != "distribute" && action != "research" &&
      action != "produce" && action != "stats" && action != "help" && action != "undo" &&
      action != "history" && action != "map") {
    std::cout << "[Facade] Unknown command '" << action << "'. Type 'help' for commands list.\n";
  }
  return false;
}