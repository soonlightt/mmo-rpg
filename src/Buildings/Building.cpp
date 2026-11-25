#include "Buildings/Building.h"

int Building::nextId = 1;

Building::Building(const std::string& type, int x, int y)
    :

      id(nextId++),
      type(type),
      x(x),
      y(y) {}

int Building::getId() const { return id; }

std::string Building::getType() const { return type; }

int Building::getX() const { return x; }

int Building::getY() const { return y; }