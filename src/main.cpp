#include <iostream>
#include <memory>
#include <string>

#include "Facade/GameFacade.h"
#include "NeutralObjects/HealingFountain.h"
#include "NeutralObjects/PowerRune.h"
#include "NeutralObjects/Teleport.h"
#include "NeutralObjects/Trap.h"

int main() {
  std::cout << "=========================================================\n";
  std::cout << "  MMO RPG: LAB 3 DEMO (MEDIATOR, COMMAND, FACADE)\n";
  std::cout << "=========================================================\n\n";

  GameFacade game(15, 15, 20, 0, 0, 500, 10, 400);
  auto& mediator = game.getMediator();
  auto& terrainMap = mediator.getTerrainMap();
  auto& field = mediator.getField();
  auto base = mediator.getBase();

  terrainMap.addNeutralObject(std::make_unique<PowerRune>(2, 0));
  terrainMap.addNeutralObject(std::make_unique<Trap>(3, 0));
  terrainMap.addNeutralObject(std::make_unique<HealingFountain>(3, 2));
  terrainMap.addNeutralObject(
      std::make_unique<Teleport>(4, 0, 15, 15, &terrainMap, base.get(), &field));

  std::cout << "--- PART 1: Automated Demonstration of All 10 Commands ---\n";

  game.executeCommand("create PhantomAssassin at 1,0");
  game.executeCommand("create Invoker at 3,1");
  game.executeCommand("create Dazzle at 4,1");

  game.executeCommand("build barracks at 1,1");
  game.executeCommand("produce WitchDoctor in 1");
  game.executeCommand("research attack_upgrade");
  game.executeCommand("distribute 30 gold to barracks");

  game.executeCommand("move unit 1 to 2,0");
  game.executeCommand("collect resource 1 by unit 1");
  game.executeCommand("heal unit 1 by 3");
  game.executeCommand("attack unit 2 -> 3");

  game.executeCommand("stats");
  game.executeCommand("map");

  std::cout << "\n--- PART 2: Command History & Undo Demonstration ---\n";
  game.executeCommand("history");
  game.executeCommand("undo");
  game.executeCommand("undo");
  game.executeCommand("stats resources");

  std::cout << "\n--- PART 3: Interactive Console Mode (type 'help' or 'exit') ---\n";
  game.executeCommand("help");

  std::string line;
  while (true) {
    std::cout << "\n> ";
    if (!std::getline(std::cin, line)) {
      break;
    }
    if (line == "exit" || line == "quit") {
      std::cout << "Exiting game.\n";
      break;
    }
    if (line.empty()) {
      continue;
    }
    game.executeCommand(line);
  }

  return 0;
}