#include <iostream>

#include "Field/GameField.h"
#include "Units/Carry/PhantomAssassin.h"
#include "Units/Midder/Invoker.h"

int main() {
  GameField field(10, 10, 5);
  field.addUnit(std::make_unique<PhantomAssassin>(1, 1));
  field.addUnit(std::make_unique<Invoker>(2, 3));

  for (auto& unit : field) {
    std::cout << "Unit: " << unit.getType() << " [" << unit.getSymbol() << "] at (" << unit.getX()
              << ", " << unit.getY() << ")\n";
  }

  GameField copyField = field;
  std::cout << "Copy size: " << copyField.size() << std::endl;

  return 0;
}