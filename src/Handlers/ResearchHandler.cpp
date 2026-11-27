#include "Handlers/ResearchHandler.h"

#include "Facade/GameFacade.h"

bool ResearchHandler::handle(const std::string& input, GameFacade& facade) {
  std::istringstream iss(input);
  std::string action;
  if (!(iss >> action) || action != "research") {
    return Handler::handle(input, facade);
  }

  std::string techId;
  if (iss >> techId) {
    return facade.research(techId);
  }
  std::cout << "[ResearchHandler] Usage: research <techId>\n";
  return false;
}