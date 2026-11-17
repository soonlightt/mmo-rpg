#pragma once

#include <memory>
#include <vector>

#include "Terrain/TerrainProxy.h"

class TerrainMap {
 private:
  int width;
  int height;
  std::vector<std::vector<std::shared_ptr<TerrainProxy>>> grid;

 public:
  TerrainMap(int w = 15, int h = 15);
  ~TerrainMap() = default;

  int getWidth() const;
  int getHeight() const;
  bool isValidCoord(int x, int y) const;

  void setTerrain(int x, int y, std::shared_ptr<Terrain> terrain);
  std::shared_ptr<TerrainProxy> getTerrain(int x, int y) const;

  void generationDefaultMap();
  void printMap() const;
};