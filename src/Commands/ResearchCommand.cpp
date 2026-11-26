#include "Commands/ResearchCommand.h"

ResearchCommand::ResearchCommand(GameMediator& med, std::string tech, int researchCost)
    : mediator(med), techId(tech), cost(researchCost) {}

bool ResearchCommand::execute() {
  executed = mediator.research(techId, cost);
  return executed;
}

bool ResearchCommand::undo() { return false; }

std::string ResearchCommand::getName() const { return "research " + techId; }