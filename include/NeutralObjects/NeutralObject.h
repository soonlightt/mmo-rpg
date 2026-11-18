#pragma once

#include <memory>
#include <string>

#include "Strategies/IInteractionStrategy.h"
#include "Units/Unit.h"

class NeutralObject {
 protected:
  int x;
  int y;
  bool isActive;
  std::unique_ptr<IInteractionStrategy> strategy;

 public:
  NeutralObject(int startX, int startY,
                std::unique_ptr<IInteractionStrategy> initialStrategy = nullptr);
  virtual ~NeutralObject() = default;

  int getX() const;
  int getY() const;
  bool getIsActive() const;
  void setActive(bool active);

  void setStrategy(std::unique_ptr<IInteractionStrategy> newStrategy);
  virtual void interact(Unit& unit);

  virtual std::string getType() const = 0;
  virtual char getSymbol() const = 0;

  bool operator!() const;
  friend Unit& operator+(Unit& unit, NeutralObject& obj);

  virtual std::string getColoredSymbol() const { return std::string(1, getSymbol()); }
};