#include "Terrain/TerrainMap.h"

#include <iostream>
#include <utility>

#include "Terrain/Forest.h"
#include "Terrain/Mountains.h"
#include "Terrain/Plains.h"
#include "Terrain/Swamp.h"

TerrainMap::TerrainMap(int w, int h)
    : width(w > 0 ? w : 15),
      height(h > 0 ? h : 15),
      grid(height, std::vector<std::shared_ptr<TerrainProxy>>(width)) {
  generationDefaultMap();
}

int TerrainMap::getWidth() const { return width; }

int TerrainMap::getHeight() const { return height; }

bool TerrainMap::isValidCoord(int x, int y) const {
  return x >= 0 && x < width && y >= 0 && y < height;
}

void TerrainMap::setTerrain(int x, int y, std::shared_ptr<Terrain> terrain) {
  if (!isValidCoord(x, y)) {
    return;
  }
  grid[y][x] = std::make_shared<TerrainProxy>(x, y, std::move(terrain));
}

std::shared_ptr<TerrainProxy> TerrainMap::getTerrain(int x, int y) const {
  if (!isValidCoord(x, y)) {
    return nullptr;
  }
  return grid[y][x];
}

void TerrainMap::generationDefaultMap() {
  const std::vector<std::string> layout = {"...............", ".~~~~....^...^.", ".~~~~..^...^.^.",
                                           "...~^...~~.....", "........~~..TTT", "........~~..TTT",
                                           ".^...T..~...TTT", "..^TTTT.......T", ".^.TTTT.......T",
                                           "...TTT....~....", ".^.TT.....~~...", "..........~~...",
                                           ".......T..~~~^.", ".^.TTTTT..~~~..", "...TTTTTT^....."};

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      char cell = '.';
      if (y < static_cast<int>(layout.size()) && x < static_cast<int>(layout[y].size())) {
        cell = layout[y][x];
      }

      switch (cell) {
        case 'T':
          setTerrain(x, y, std::make_shared<Forest>(x, y));
          break;
        case '~':
          setTerrain(x, y, std::make_shared<Swamp>(x, y));
          break;
        case '^':
          setTerrain(x, y, std::make_shared<Mountains>(x, y));
          break;
        default:
          setTerrain(x, y, std::make_shared<Plains>(x, y));
          break;
      }
    }
  }
}

void TerrainMap::printMap() const {
  const std::string RESET = "\033[0m";
  const std::string GREEN = "\033[92m";
  const std::string DARK_GREEN = "\033[32m";
  const std::string YELLOW = "\033[33m";
  const std::string WHITE = "\033[97m";

  std::cout << "___ Terrain Map (" << width << "x" << height << ") ___" << std::endl;

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      NeutralObject* neutralObj = getNeutralObjectAt(x, y);
      if (neutralObj) {
        std::cout << neutralObj->getColoredSymbol() << ' ';
        continue;
      }

      if (!grid[y][x]) {
        std::cout << "? ";
        continue;
      }

      char sym = grid[y][x]->getSymbol();
      switch (sym) {
        case 'T':
          std::cout << GREEN << sym << RESET << ' ';
          break;
        case '~':
          std::cout << YELLOW << sym << RESET << ' ';
          break;
        case '^':
          std::cout << WHITE << sym << RESET << ' ';
          break;
        default:
          std::cout << DARK_GREEN << sym << RESET << ' ';
          break;
      }
    }
    std::cout << std::endl;
  }

  std::cout << "Legend: " << DARK_GREEN << "." << RESET << " Plains | " << GREEN << "T" << RESET
            << " Forest | " << YELLOW << "~" << RESET << " Swamp | " << WHITE << "^" << RESET
            << " Mountains | \033[93m*\033[0m Rune | \033[91mx\033[0m Trap | "
            << "\033[92m+\033[0m Fountain | \033[96m@\033[0m Teleport\n";
}

bool TerrainMap::addNeutralObject(std::unique_ptr<NeutralObject> obj) {
  if (!obj) {
    return false;
  }
  int x = obj->getX();
  int y = obj->getY();

  if (!isValidCoord(x, y) || !grid[y][x] || !grid[y][x]->isPassable()) {
    return false;
  }
  if (getNeutralObjectAt(x, y) != nullptr) {
    return false;
  }
  neutralObjects.push_back(std::move(obj));
  return true;
}

NeutralObject* TerrainMap::getNeutralObjectAt(int x, int y) const {
  for (const auto& obj : neutralObjects) {
    if (obj && obj->getIsActive() && obj->getX() == x && obj->getY() == y) {
      return obj.get();
    }
  }
  return nullptr;
}

const std::vector<std::unique_ptr<NeutralObject>>& TerrainMap::getNeutralObjects() const {
  return neutralObjects;
}