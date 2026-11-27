#include "Handlers/CollectHandler.h"

#include "Facade/GameFacade.h"

bool CollectHandler::handle(const std::string& input, GameFacade& facade) {
  std::istringstream iss(input);
  std::string action;
  if (!(iss >> action) || action != "collect") {
    return Handler::handle(input, facade);
  }

  std::string kwRes, kwBy, kwUnit;
  int resId = 0, unitId = 0;
  if (iss >> kwRes >> resId >> kwBy >> kwUnit >> unitId) {
    return facade.collectResource(unitId, resId);
  }
  std::cout << "[CollectHandler] Usage: collect resource <resId> by unit <unitId>\n";
  return false;
}