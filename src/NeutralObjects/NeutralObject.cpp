#include "NeutralObjects/NeutralObject.h"

#include <iostream>

NeutralObject::NeutralObject(int startX, int startY,
                             std::unique_ptr<IInteractionStrategy> initialStrategy)
    : x(startX), y(startY), isActive(true), strategy(std::move(initialStrategy)) {}

int NeutralObject::getX() const { return x; }

int NeutralObject::getY() const { return y; }

bool NeutralObject::getIsActive() const { return isActive; }

void NeutralObject::setActive(bool active) { isActive = active; }

void NeutralObject::setStrategy(std::unique_ptr<IInteractionStrategy> newStrategy) {
  strategy = std::move(newStrategy);
}

void NeutralObject::interact(Unit& unit) {
  if (!isActive || !unit.isAlive()) {
    return;
  }
  if (strategy) {
    std::cout << "[" << getType() << "] Applying strategy '" << strategy->getName() << "' to "
              << unit.getType() << std::endl;
    strategy->apply(unit);
  }
}

bool NeutralObject::operator!() const { return !isActive; }

Unit& operator+(Unit& unit, NeutralObject& obj) {
  obj.interact(unit);
  return unit;
}