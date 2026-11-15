#pragma once
#include <memory>
#include <utility>
#include <vector>

#include "Factories/UnitFactory.h"
#include "Field/GameFieldIterator.h"
#include "Units/Unit.h"

class GameField {
 private:
  int width;
  int height;
  size_t maxUnits;
  std::vector<std::unique_ptr<Unit>> units;

 public:
  explicit GameField(int w = 15, int h = 15, size_t maxU = 20);
  ~GameField() = default;

  GameFieldIterator begin() const;
  GameFieldIterator end() const;

  GameField(const GameField& other);
  GameField& operator=(GameField other);
  friend void swap(GameField& first, GameField& second) noexcept;

  GameField(GameField&&) noexcept = default;

  int getWidth() const;
  int getHeight() const;
  size_t size() const;
  size_t getMaxUnits() const;

  bool spawnUnit(const UnitFactory& factory, int subType, int x, int y);
  bool addUnit(std::unique_ptr<Unit> unit);
  bool removeUnit(int x, int y);
  bool empty() const;
  Unit* getUnit(int x, int y) const;
  void clear();
};