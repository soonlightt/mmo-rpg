#include <iostream>

#include "Field/GameField.h"

int main() {
  GameField field(15, 15, 20);
  std::cout << "Create field: " << field.getWidth() << "x" << field.getHeight() << std::endl;
  std::cout << "Max units: " << field.getMaxUnits() << std::endl;
  return 0;
}