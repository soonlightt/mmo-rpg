#include "Handlers/StatsHandler.h"

#include "Facade/GameFacade.h"

bool StatsHandler::handle(const std::string& input, GameFacade& facade) {
  std::istringstream iss(input);
  std::string action;
  if (!(iss >> action) || action != "stats") {
    return Handler::handle(input, facade);
  }

  std::string filter;
  iss >> filter;
  facade.showStats(filter);
  return true;
}