#include "Handlers/HelpHandler.h"

#include "Facade/GameFacade.h"

bool HelpHandler::handle(const std::string& input, GameFacade& facade) {
  std::istringstream iss(input);
  std::string action;
  if (!(iss >> action)) {
    return false;
  }

  if (action == "help") {
    facade.printHelp();
    return true;
  } else if (action == "undo") {
    return facade.undoLastCommand();
  } else if (action == "history") {
    facade.printHistory();
    return true;
  } else if (action == "map") {
    facade.printWorld();
    return true;
  }

  return Handler::handle(input, facade);
}