#include "Factories/CarryFactory.h"

#include "Units/Carry/Lifestealer.h"
#include "Units/Carry/PhantomAssassin.h"

std::unique_ptr<Unit> CarryFactory::createUnit(int subType, int x, int y) const {
  if (subType == 1) {
    return std::make_unique<Lifestealer>(x, y);
  }
  if (subType == 2) {
    return std::make_unique<PhantomAssassin>(x, y);
  }
  return nullptr;
}