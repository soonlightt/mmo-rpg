#include "Factories/MidderFactory.h"

#include "Units/Midder/ArcWarden.h"
#include "Units/Midder/Invoker.h"

std::unique_ptr<Unit> MidderFactory::createUnit(int subType, int x, int y) const {
  if (subType == 1) {
    return std::make_unique<ArcWarden>(x, y);
  }
  if (subType == 2) {
    return std::make_unique<Invoker>(x, y);
  }
  return nullptr;
}