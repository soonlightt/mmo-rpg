#pragma once

#include <cstddef>
#include <memory>
#include <vector>

#include "Units/Unit.h"

class GameFieldIterator {
 private:
  const std::vector<std::unique_ptr<Unit>>* units;
  size_t index;

  void skipEmpty();

 public:
  GameFieldIterator(const std::vector<std::unique_ptr<Unit>>* u, size_t idx);

  Unit& operator*() const;

  Unit* operator->() const;

  GameFieldIterator& operator++();

  bool operator==(const GameFieldIterator& other) const;
  bool operator!=(const GameFieldIterator& other) const;
};