#pragma once

#include <vector>

#include "Observer/IObserver.h"
#include "Observer/UnitEvent.h"

class IObservable {
 protected:
  std::vector<IObserver*> observers;

 public:
  virtual ~IObservable() = default;

  void addObserver(IObserver* observer);
  void removeObserver(IObserver* observer);
  void notify(const UnitEvent& event);
};