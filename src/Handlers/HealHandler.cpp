#include "Handlers/HealHandler.h"

#include "Facade/GameFacade.h"

bool HealHandler::handle(const std::string& input, GameFacade& facade) {
  std::istringstream iss(input);
  std::string action;
  if (!(iss >> action) || action != "heal") {
    return Handler::handle(input, facade);
  }

  std::string kwUnit, kwBy;
  int targetId = 0, healerId = 0;
  if (iss >> kwUnit >> targetId >> kwBy >> healerId) {
    return facade.healUnit(healerId, targetId);
  }
  std::cout << "[HealHandler] Usage: heal unit <targetId> by <healerId>\n";
  return false;
}