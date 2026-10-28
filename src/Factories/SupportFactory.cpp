#include "Factories/SupportFactory.h"

#include "Units/Support/Dazzle.h"
#include "Units/Support/WitchDoctor.h"

std::unique_ptr<Unit> SupportFactory::createUnit(int subType, int x, int y) const {
  if (subType == 1) {
    return std::make_unique<Dazzle>(x, y);
  }
  if (subType == 2) {
    return std::make_unique<WitchDoctor>(x, y);
  }
  return nullptr;
}