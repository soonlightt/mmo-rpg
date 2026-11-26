#include "Commands/StatsCommand.h"

StatsCommand::StatsCommand(GameMediator& med, std::string statsFilter)
    : mediator(med), filter(std::move(statsFilter)) {}

bool StatsCommand::execute() {
  std::cout << mediator.getStats(filter);
  return true;
}

bool StatsCommand::undo() { return false; }

std::string StatsCommand::getName() const { return filter.empty() ? "stats" : "stats " + filter; }