#include "Controller/GameController.h"
#include "Factories/CarryFactory.h"
#include "Factories/MidderFactory.h"
#include "Factories/SupportFactory.h"
#include "NeutralObjects/HealingFountain.h"
#include "NeutralObjects/PowerRune.h"
#include "NeutralObjects/Teleport.h"
#include "NeutralObjects/Trap.h"

int main() {
  std::cout << "=========================================================\n";
  std::cout << "  MMO RPG: FULL SYSTEM DEMO (OBSERVER, PROXY, STRATEGY)\n";
  std::cout << "=========================================================\n\n";

  GameController game(15, 15, 20, 0, 0, 500, 10, 200);
  auto base = game.getBase();
  auto& terrainMap = game.getTerrainMap();
  auto& field = game.getField();

  CarryFactory carryFactory;
  MidderFactory midderFactory;
  SupportFactory supportFactory;

  std::cout << "--- SCENARIO 1: Base & Spawning Units via Factories ---\n";
  std::cout << "Base initial resources: " << base->getResources()
            << ", max units: " << base->getMaxUnits() << "\n";

  game.spawnUnit(carryFactory, 2, 1, 0, 50);
  game.spawnUnit(midderFactory, 2, 3, 1, 50);
  game.spawnUnit(supportFactory, 1, 4, 1, 50);

  terrainMap.addNeutralObject(std::make_unique<PowerRune>(2, 0));
  terrainMap.addNeutralObject(std::make_unique<Trap>(3, 0));
  terrainMap.addNeutralObject(std::make_unique<HealingFountain>(3, 2));
  terrainMap.addNeutralObject(
      std::make_unique<Teleport>(4, 0, 15, 15, &terrainMap, base.get(), &field));

  game.printWorld();

  std::cout << "\n--- SCENARIO 2: Terrain Effects & TerrainProxy Blocking ---\n";

  Unit* pa = field.getUnit(1, 0);
  if (pa) {
    std::cout << "PhantomAssassin on Plains (1, 0) -> Attack: " << pa->getAttack().getValue()
              << " (includes +10 Plains Carry bonus)\n";
  }

  std::cout << "Attempting to move PhantomAssassin onto Mountains at (1, 6)...\n";
  game.moveUnit(1, 0, 1, 6);

  std::cout << "\n--- SCENARIO 3: Neutral Objects & Overloaded Operators (+, !) ---\n";

  game.moveUnit(1, 0, 2, 0);
  NeutralObject* rune = terrainMap.getNeutralObjectAt(2, 0);
  std::cout << "Is PowerRune depleted (operator!)? "
            << (rune == nullptr ? "Yes (deactivated)" : "No") << "\n";

  game.moveUnit(2, 0, 3, 0);

  Unit* invoker = field.getUnit(3, 1);
  if (invoker) {
    invoker->takeDamage(60);
  }
  game.moveUnit(3, 1, 3, 2);

  game.moveUnit(3, 0, 4, 0);

  game.printWorld();

  std::cout << "\n--- SCENARIO 4: Combat & Observer Notification on Unit Death ---\n";

  game.moveUnit(3, 2, 3, 1);

  while (field.getUnit(4, 1) != nullptr && field.getUnit(4, 1)->isAlive()) {
    game.attackUnit(3, 1, 4, 1);
  }

  std::cout << "\nFinal alive units on field: " << field.size() << "\n";
  std::cout << "=== All scenarios completed successfully! ===\n";

  return 0;
}