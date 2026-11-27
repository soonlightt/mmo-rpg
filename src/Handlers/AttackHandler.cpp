#include "Handlers/AttackHandler.h"

#include "Facade/GameFacade.h"

bool AttackHandler::handle(const std::string& input, GameFacade& facade) {
  std::istringstream iss(input);
  std::string action;
  if (!(iss >> action) || action != "attack") {
    return Handler::handle(input, facade);
  }

  std::string kwUnit, arrow;
  int attackerId = 0, targetId = 0;
  if (iss >> kwUnit >> attackerId >> arrow >> targetId) {
    return facade.attackUnit(attackerId, targetId);
  }
  std::cout << "[AttackHandler] Usage: attack unit <attackerId> -> <targetId>\n";
  return false;
}