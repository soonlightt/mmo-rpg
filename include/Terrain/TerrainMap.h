#pragma once

#include <memory>
#include <vector>

#include "NeutralObjects/NeutralObject.h"
#include "Terrain/TerrainProxy.h"

class TerrainMap {
 private:
  int width;
  int height;
  std::vector<std::vector<std::shared_ptr<TerrainProxy>>> grid;
  std::vector<std::unique_ptr<NeutralObject>> neutralObjects;

 public:
  TerrainMap(int w = 15, int h = 15);
  ~TerrainMap() = default;

  int getWidth() const;
  int getHeight() const;
  bool isValidCoord(int x, int y) const;

  void setTerrain(int x, int y, std::shared_ptr<Terrain> terrain);
  std::shared_ptr<TerrainProxy> getTerrain(int x, int y) const;

  bool addNeutralObject(std::unique_ptr<NeutralObject> obj);
  NeutralObject* getNeutralObjectAt(int x, int y) const;
  const std::vector<std::unique_ptr<NeutralObject>>& getNeutralObjects() const;

  void generationDefaultMap();
  void printMap() const;
};