#include "Handlers/CreateHandler.h"

#include "Facade/GameFacade.h"
#include "Handlers/ParseUtils.h"

bool CreateHandler::handle(const std::string& input, GameFacade& facade) {
  std::istringstream iss(input);
  std::string action;
  if (!(iss >> action) || action != "create") {
    return Handler::handle(input, facade);
  }

  std::string unitType, kwAt, coordToken;
  int x = 0, y = 0;
  if (iss >> unitType >> kwAt >> coordToken && parseCoords(coordToken, x, y)) {
    return facade.createUnit(unitType, x, y);
  }
  std::cout << "[CreateHandler] Usage: create <unitType> at <x>,<y>\n";
  return false;
}