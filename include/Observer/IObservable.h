#pragma once

#include <vector>

#include "Observer/IObserver.h"

class IObservable {
 protected:
  std::vector<IObserver*> observers;

 public:
  virtual void addObserver(IObserver* observer);
  virtual void removeObserver(IObserver* observer);
  virtual void notify(const UnitEvent& event);

  virtual ~IObservable() = default;
};