#include "Field/GameFieldIterator.h"

#include "Units/Unit.h"

GameFieldIterator::GameFieldIterator(const std::vector<std::unique_ptr<Unit>>* u, size_t idx)
    : units(u), index(idx) {
  skipEmpty();
}

void GameFieldIterator::skipEmpty() {
  if (!units) return;
  while (index < units->size() && (*units)[index] == nullptr) {
    ++index;
  }
}

Unit& GameFieldIterator::operator*() const { return *((*units)[index]); }

Unit* GameFieldIterator::operator->() const { return (*units)[index].get(); }

GameFieldIterator& GameFieldIterator::operator++() {
  if (units && index < units->size()) {
    ++index;
    skipEmpty();
  }
  return *this;
}

bool GameFieldIterator::operator==(const GameFieldIterator& other) const {
  return units == other.units && index == other.index;
}

bool GameFieldIterator::operator!=(const GameFieldIterator& other) const {
  return !(*this == other);
}