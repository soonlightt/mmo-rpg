#pragma once
#include "Observer/UnitEvent.h"

class IObserver {
 public:
  virtual void onUnitEvent(const UnitEvent& event) = 0;
  virtual ~IObserver() = default;
};