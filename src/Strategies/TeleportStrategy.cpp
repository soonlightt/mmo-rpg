#include "Strategies/TeleportStrategy.h"

TeleportStrategy::TeleportStrategy(int width, int height, const TerrainMap* map,
                                   const Base* gameBase, const GameField* gameField)
    : areaWidth(width), areaHeight(height), terrainMap(map), base(gameBase), field(gameField) {}

void TeleportStrategy::apply(Unit& unit) {
  int targetX = (areaWidth - 1) - unit.getX();
  int targetY = (areaHeight - 1) - unit.getY();

  if (targetX == unit.getX() && targetY == unit.getY()) {
    std::cout << "  -> [Teleport] " << unit.getType()
              << " is at the mirror center, position unchanged." << std::endl;
    return;
  }

  auto targetTerrain = terrainMap ? terrainMap->getTerrain(targetX, targetY) : nullptr;
  if (!targetTerrain || !targetTerrain->isPassable()) {
    std::cout << "  -> [Teleport] Blocked! Target (" << targetX << ", " << targetY
              << ") is out of bounds or impassable (Mountains)." << std::endl;
    return;
  }

  if (base && targetX == base->getX() && targetY == base->getY()) {
    std::cout << "  -> [Teleport] Blocked! Target (" << targetX << ", " << targetY
              << ") is occupied by Base." << std::endl;
    return;
  }

  if (field && field->getUnit(targetX, targetY) != nullptr) {
    std::cout << "  -> [Teleport] Blocked! Target (" << targetX << ", " << targetY
              << ") is occupied by another unit." << std::endl;
    return;
  }

  unit.setPosition(targetX, targetY);
  std::cout << "  -> [Teleport] " << unit.getType() << " mirrored to (" << targetX << ", "
            << targetY << ")!" << std::endl;
}

std::string TeleportStrategy::getName() const { return "MirrorTeleport"; }