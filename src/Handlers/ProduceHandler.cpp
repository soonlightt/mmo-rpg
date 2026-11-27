#include "Handlers/ProduceHandler.h"

#include "Facade/GameFacade.h"

bool ProduceHandler::handle(const std::string& input, GameFacade& facade) {
  std::istringstream iss(input);
  std::string action;
  if (!(iss >> action) || action != "produce") {
    return Handler::handle(input, facade);
  }

  std::string unitType, kwIn;
  int buildingId = 0;
  if (iss >> unitType >> kwIn >> buildingId) {
    return facade.produceUnit(unitType, buildingId);
  }
  std::cout << "[ProduceHandler] Usage: produce <unitType> in <buildingId>\n";
  return false;
}