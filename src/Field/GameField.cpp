#include "Field/GameField.h"

#include <algorithm>

#include "Factories/UnitFactory.h"

GameField::GameField(int w, int h, size_t maxU)
    : width(w > 0 ? w : 15), height(h > 0 ? h : 15), maxUnits(maxU) {}

int GameField::getWidth() const { return width; }

int GameField::getHeight() const { return height; }

size_t GameField::getUnitsCount() const { return units.size(); }

size_t GameField::getMaxUnits() const { return maxUnits; }

bool GameField::addUnit(std::unique_ptr<Unit> unit) {
  if (!unit) return false;
  if (maxUnits <= units.size()) return false;
  if (unit->getX() < 0 || unit->getX() >= width || unit->getY() < 0 || unit->getY() >= height)
    return false;
  if (getUnitAt(unit->getX(), unit->getY()) != nullptr) {
    return false;
  }

  units.push_back(std::move(unit));
  return true;
}

bool GameField::spawnUnit(const UnitFactory& factory, int subType, int x, int y) {
  auto unit = factory.createUnit(subType, x, y);
  return addUnit(std::move(unit));
}

bool GameField::removeUnit(int x, int y) {
  auto it = std::find_if(units.begin(), units.end(), [x, y](const std::unique_ptr<Unit>& u) {
    return u->getX() == x && u->getY() == y;
  });

  if (it != units.end()) {
    units.erase(it);
    return true;
  }

  return false;
}

Unit* GameField::getUnitAt(int x, int y) const {
  for (const auto& u : units) {
    if (u->getX() == x && u->getY() == y) {
      return u.get();
    }
  }
  return nullptr;
}

void GameField::clear() { units.clear(); }