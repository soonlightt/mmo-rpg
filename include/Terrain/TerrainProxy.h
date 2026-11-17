#pragma once

#include <iostream>
#include <memory>
#include <string>

#include "Terrain/Terrain.h"
class TerrainProxy : public Terrain {
 private:
  std::shared_ptr<Terrain> realTerrain;

 public:
  TerrainProxy(int x, int y, std::shared_ptr<Terrain> realTerrain);

  std::string getType() const override { return realTerrain->getType(); }

  char getSymbol() const override { return realTerrain->getSymbol(); }

  bool isPassable() const override { return realTerrain->isPassable(); }

  std::string getEffectDescription() const override { return realTerrain->getEffectDescription(); }

  void onEnter(Unit& unit) override {
    if (!unit.isAlive()) {
      return;
    }

    if (!realTerrain->isPassable()) {
      std::cout << "[TerrainProxy] Access denied to " << realTerrain->getType() << std::endl;
      return;
    }

    std::cout << "[TerrainProxy] " << unit.getType() << " entered " << realTerrain->getType()
              << std::endl;
    realTerrain->onEnter(unit);
  }

  void onLeave(Unit& unit) override {
    if (!realTerrain->isPassable()) {
      return;
    }

    std::cout << "[TerrainProxy] " << unit.getType() << " left " << realTerrain->getType()
              << std::endl;
    realTerrain->onLeave(unit);
  }
};