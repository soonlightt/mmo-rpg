#include "Attributes/Health.h"

#include <algorithm>

Health::Health(int initialValue)
    : value(std::max(0, initialValue)), maxValue(std::max(0, initialValue)) {}

int Health::getValue() const { return value; }

int Health::getMaxValue() const { return maxValue; }

void Health::decrease(int amount) {
  if (amount <= 0) return;
  value = std::max(0, value - amount);
}

void Health::increase(int amount) {
  if (amount <= 0) return;
  value = std::min(maxValue, value + amount);
}

bool Health::isZero() const { return value == 0; }