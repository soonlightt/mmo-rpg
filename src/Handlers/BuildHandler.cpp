#include "Handlers/BuildHandler.h"

#include "Facade/GameFacade.h"
#include "Handlers/ParseUtils.h"

bool BuildHandler::handle(const std::string& input, GameFacade& facade) {
  std::istringstream iss(input);
  std::string action;
  if (!(iss >> action)) {
    return false;
  }

  if (action == "build") {
    std::string buildingType, kwAt, coordToken;
    int x = 0, y = 0;
    if (iss >> buildingType >> kwAt >> coordToken && parseCoords(coordToken, x, y)) {
      return facade.buildBuilding(buildingType, x, y);
    }
    std::cout << "[BuildHandler] Usage: build <buildingType> at <x>,<y>\n";
    return false;
  } else if (action == "distribute") {
    int amount = 0;
    std::string resType, kwTo, targetName;
    if (iss >> amount >> resType >> kwTo >> targetName) {
      return facade.distributeResources(resType, targetName, amount);
    }
    std::cout << "[BuildHandler] Usage: distribute <amount> <resType> to <targetBuilding>\n";
    return false;
  }

  return Handler::handle(input, facade);
}