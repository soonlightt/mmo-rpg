#include <iostream>
#include <memory>
#include <string>

#include "Field/GameField.h"
#include "Units/Carry/Lifestealer.h"
#include "Units/Carry/PhantomAssassin.h"
#include "Units/Midder/Invoker.h"
#include "Units/Support/Dazzle.h"

void printSeparator(const std::string& title) {
  std::cout << "\n==================================================\n";
  std::cout << "  " << title << "\n";
  std::cout << "==================================================\n";
}

int main() {
  printSeparator("SCRIPT 1: Fill field and basic operation");

  GameField field(15, 15, 20);
  std::cout << "Create field: " << field.getWidth() << "x" << field.getHeight()
            << ", лимит юнитов: " << field.getMaxUnits() << "\n";

  field.addUnit(std::make_unique<PhantomAssassin>(1, 2));
  field.addUnit(std::make_unique<Invoker>(3, 4));
  field.addUnit(std::make_unique<Dazzle>(5, 6));

  std::cout << "Added units: " << field.size() << "\n";
  std::cout << "Is field empty? " << (field.empty() ? "Yes" : "No") << "\n";

  std::cout << "\nList units on the field:\n";
  for (auto& unit : field) {
    std::cout << "  - [" << unit.getSymbol() << "] " << unit.getType() << " on (" << unit.getX()
              << ", " << unit.getY() << ")"
              << " | HP: " << unit.getHealth().getValue()
              << " | Armor: " << unit.getArmor().getValue()
              << " | Attack: " << unit.getAttack().getValue() << "\n";
  }

  printSeparator("SCRIPT 2: Deep copy and independence of a copy");

  std::cout << "Creating a full copy of the field: GameField copyField = field;\n";
  GameField copyField = field;

  std::cout << "Size original: " << field.size() << ", size of a copy: " << copyField.size()
            << "\n";

  Unit* copiedInvoker = copyField.getUnit(3, 4);
  Unit* originalInvoker = field.getUnit(3, 4);

  if (copiedInvoker && originalInvoker) {
    std::cout << "\nDealing 50 dmg to Invoker on the copied field...\n";
    copiedInvoker->takeDamage(50);

    std::cout << "Original field Invoker HP: " << originalInvoker->getHealth().getValue() << "\n";
    std::cout << "Copied field Invoker HP:   " << copiedInvoker->getHealth().getValue() << "\n";

    if (originalInvoker->getHealth().getValue() != copiedInvoker->getHealth().getValue()) {
      std::cout << ">> SUCCESS: The copy is completely independent of the original!\n";
    }
  }

  printSeparator("SCRIPT 3: Move semantics and mass action");

  std::cout << "Moving field";
  GameField movedField = std::move(field);

  std::cout << "Original field size after move: " << field.size() << " (emptied)\n";
  std::cout << "Moved field size:               " << movedField.size() << "\n";

  std::cout << "\nMass action with help iterator: dealing 15 dmg to all units on movedField:\n";
  for (auto& unit : movedField) {
    unit.takeDamage(15);
    std::cout << "  - " << unit.getType()
              << " took damage, remaining HP: " << unit.getHealth().getValue()
              << " (alive: " << (unit.isAlive() ? "Yes" : "No") << ")\n";
  }

  std::cout << "\nRemoving unit from field (3, 4)...\n";
  movedField.removeUnit(3, 4);
  std::cout << "New movedField size: " << movedField.size() << "\n";

  printSeparator("All demonstration scenarios completed successfully!");
  return 0;
}