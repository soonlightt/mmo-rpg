#pragma once
#include <memory>
#include <vector>

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

  GameField(const GameField&) = delete;
  GameField& operator=(const GameField&) = delete;

  GameField(GameField&&) noexcept = default;
  GameField& operator=(GameField&&) noexcept = default;

  int getWidth() const;
  int getHeight() const;
  size_t getUnitsCount() const;
  size_t getMaxUnits() const;

  bool addUnit(std::unique_ptr<Unit> unit);
  bool removeUnit(int x, int y);
  Unit* getUnitAt(int x, int y) const;
  void clear();
};