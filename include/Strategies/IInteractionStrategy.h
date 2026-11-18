#pragma once

#include <string>

class Unit;

class IInteractionStrategy {
 public:
  virtual ~IInteractionStrategy() = default;
  virtual void apply(Unit& unit) = 0;
  virtual std::string getName() const = 0;
};