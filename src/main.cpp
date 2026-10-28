#include <iostream>

#include "Factories/CarryFactory.h"
#include "Factories/MidderFactory.h"
#include "Factories/SupportFactory.h"
#include "Field/GameField.h"

int main() {
  std::cout << "=== DEMO: Pattern Factory Method (Stage 3) ===" << std::endl;

  GameField field(15, 15, 20);
  std::cout << "Field created: " << field.getWidth() << 'x' << field.getHeight()
            << ", Max Units: " << field.getMaxUnits() << std::endl;

  CarryFactory carryFactory;
  MidderFactory midderFactory;
  SupportFactory supportFactory;

  std::cout << "--- Spawning Units using factories ---" << std::endl;

  field.spawnUnit(carryFactory, 1, 1, 1);
  field.spawnUnit(carryFactory, 2, 2, 2);

  field.spawnUnit(midderFactory, 1, 3, 3);
  field.spawnUnit(midderFactory, 2, 4, 4);

  field.spawnUnit(supportFactory, 1, 5, 5);
  field.spawnUnit(supportFactory, 2, 6, 6);

  std::cout << "Total units on field: " << field.getUnitsCount() << std::endl;

  std::cout << "--- Units Information ---" << std::endl;

  int coords[6][2] = {{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}};

  for (const auto& pos : coords) {
    Unit* u = field.getUnitAt(pos[0], pos[1]);
    if (u) {
      std::cout << "[" << u->getSymbol() << "] " << u->getType() << " at (" << u->getX() << ", "
                << u->getY() << ") | HP: " << u->getHealth().getValue()
                << ", Armor: " << u->getArmor().getValue()
                << ", Attack: " << u->getAttack().getValue() << std::endl;
    }
  }

  std::cout << "\n--- Edge Cases Validation ---" << std::endl;

  bool duplicateSpawn = field.spawnUnit(carryFactory, 1, 1, 1);
  std::cout << "Spawn on occupied cell (1, 1): "
            << (duplicateSpawn ? "SUCCESS" : "REJECTED (Correct)") << std::endl;

  bool invalidTypeSpawn = field.spawnUnit(midderFactory, 99, 7, 7);
  std::cout << "Spawn invalid subType (99): "
            << (invalidTypeSpawn ? "SUCCESS" : "REJECTED (Correct)") << std::endl;

  std::cout << "\nFinal units count: " << field.getUnitsCount() << std::endl;
  std::cout << "=== Stage 3 Completed Successfully ===" << std::endl;

  return 0;
}