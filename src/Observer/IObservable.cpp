#include "Observer/IObservable.h"

#include <algorithm>

void IObservable::addObserver(IObserver* observer) {
  if (observer && std::find(observers.begin(), observers.end(), observer) == observers.end()) {
    observers.push_back(observer);
  }
}

void IObservable::removeObserver(IObserver* observer) {
  observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void IObservable::notify(const UnitEvent& event) {
  for (IObserver* observer : observers) {
    if (observer) {
      observer->onUnitEvent(event);
    }
  }
}