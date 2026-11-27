#include "Handlers/MoveHandler.h"

#include "Facade/GameFacade.h"
#include "Handlers/ParseUtils.h"

bool MoveHandler::handle(const std::string& input, GameFacade& facade) {
  std::istringstream iss(input);
  std::string action;
  if (!(iss >> action) || action != "move") {
    return Handler::handle(input, facade);
  }

  std::string kwUnit, kwTo, coordToken;
  int id = 0, x = 0, y = 0;
  if (iss >> kwUnit >> id >> kwTo >> coordToken && parseCoords(coordToken, x, y)) {
    return facade.moveUnit(id, x, y);
  }
  std::cout << "[MoveHandler] Usage: move unit <id> to <x>,<y>\n";
  return false;
}