#include "Controller/GameController.h"

GameController::GameController(int width, int height, size_t maxUnits, int baseX, int baseY,
                               int baseHealth, size_t baseMaxUnits, int baseResources)
    : base(std::make_shared<Base>(baseX, baseY, baseHealth, baseMaxUnits, 5, baseResources)),
      field(width, height, maxUnits),
      terrainMap(width, height) {}

GameField& GameController::getField() { return field; }
const GameField& GameController::getField() const { return field; }

TerrainMap& GameController::getTerrainMap() { return terrainMap; }
const TerrainMap& GameController::getTerrainMap() const { return terrainMap; }

std::shared_ptr<Base> GameController::getBase() const { return base; }

bool GameController::spawnUnit(const UnitFactory& factory, int subType, int x, int y, int cost) {
  if (!terrainMap.isValidCoord(x, y)) {
    std::cout << "[GameController] Spawn failed: invalid coordinates (" << x << ", " << y << ").\n";
    return false;
  }

  if (base && x == base->getX() && y == base->getY()) {
    std::cout << "[GameController] Spawn failed: cell (" << x << ", " << y
              << ") is occupied by Base.\n";
    return false;
  }

  auto terrain = terrainMap.getTerrain(x, y);
  if (!terrain || !terrain->isPassable()) {
    std::cout << "[GameController] Spawn failed: terrain at (" << x << ", " << y
              << ") is impassable.\n";
    return false;
  }

  if (field.getUnit(x, y) != nullptr) {
    std::cout << "[GameController] Spawn failed: cell (" << x << ", " << y
              << ") is already occupied.\n";
    return false;
  }

  if (base && !base->spendResources(cost)) {
    std::cout << "[GameController] Spawn failed: not enough resources in Base (need " << cost
              << ", have " << base->getResources() << ").\n";
    return false;
  }

  auto unit = factory.createUnit(subType, x, y);
  if (!unit) {
    if (base) {
      base->addResources(cost);
    }
    std::cout << "[GameController] Spawn failed: unknown unit subtype " << subType << ".\n";
    return false;
  }

  if (!field.addUnit(std::move(unit))) {
    if (base) {
      base->addResources(cost);
    }
    return false;
  }

  Unit* placedUnit = field.getUnit(x, y);
  if (placedUnit) {
    if (base) {
      placedUnit->addObserver(base.get());
    }
    std::cout << "[GameController] Spawned " << placedUnit->getType() << " at (" << x << ", " << y
              << "). Base resources left: " << (base ? base->getResources() : 0) << "\n";

    terrain->onEnter(*placedUnit);

    NeutralObject* neutralObj = terrainMap.getNeutralObjectAt(x, y);
    if (neutralObj && neutralObj->getIsActive()) {
      *placedUnit + *neutralObj;
    }
  }

  return true;
}

bool GameController::moveUnit(int fromX, int fromY, int toX, int toY) {
  Unit* unit = field.getUnit(fromX, fromY);
  if (!unit) {
    std::cout << "[GameController] Move failed: no unit at (" << fromX << ", " << fromY << ").\n";
    return false;
  }

  if (!unit->isAlive()) {
    std::cout << "[GameController] Move failed: unit is dead.\n";
    return false;
  }

  if (!terrainMap.isValidCoord(toX, toY)) {
    std::cout << "[GameController] Move failed: target (" << toX << ", " << toY
              << ") out of bounds.\n";
    return false;
  }

  if (base && toX == base->getX() && toY == base->getY()) {
    std::cout << "[GameController] Move failed: target (" << toX << ", " << toY << ") is Base.\n";
    return false;
  }

  if (field.getUnit(toX, toY) != nullptr) {
    std::cout << "[GameController] Move failed: target (" << toX << ", " << toY
              << ") is occupied.\n";
    return false;
  }

  auto targetTerrain = terrainMap.getTerrain(toX, toY);
  if (!targetTerrain || !targetTerrain->isPassable()) {
    std::cout << "[GameController] Move blocked by TerrainProxy at (" << toX << ", " << toY
              << ")!\n";
    if (targetTerrain) {
      targetTerrain->onEnter(*unit);
    }
    return false;
  }

  auto oldTerrain = terrainMap.getTerrain(fromX, fromY);
  if (oldTerrain) {
    oldTerrain->onLeave(*unit);
  }

  unit->setPosition(toX, toY);
  std::cout << "[GameController] " << unit->getType() << " moved from (" << fromX << ", " << fromY
            << ") to (" << toX << ", " << toY << ").\n";

  targetTerrain->onEnter(*unit);

  NeutralObject* neutralObj = terrainMap.getNeutralObjectAt(toX, toY);
  if (neutralObj && neutralObj->getIsActive()) {
    *unit + *neutralObj;
  }

  return true;
}

bool GameController::attackUnit(int attackerX, int attackerY, int targetX, int targetY) {
  Unit* attacker = field.getUnit(attackerX, attackerY);
  Unit* target = field.getUnit(targetX, targetY);

  if (!attacker || !target || !attacker->isAlive() || !target->isAlive()) {
    std::cout << "[GameController] Attack failed: invalid attacker or target.\n";
    return false;
  }

  int rawDamage = attacker->getAttack().getValue();
  std::cout << "[GameController] " << attacker->getType() << " attacks " << target->getType()
            << " for " << rawDamage << " raw damage!\n";

  target->takeDamage(rawDamage);

  if (!target->isAlive()) {
    std::cout << "[GameController] " << target->getType() << " died at (" << targetX << ", "
              << targetY << ") and is removed from the field.\n";
    field.removeUnit(targetX, targetY);
  }

  return true;
}

void GameController::printWorld() const {
  const std::string RESET = "\033[0m";
  const std::string GREEN = "\033[92m";
  const std::string DARK_GREEN = "\033[32m";
  const std::string YELLOW = "\033[33m";
  const std::string WHITE = "\033[97m";
  const std::string BLUE = "\033[94m";
  const std::string MAGENTA = "\033[95m";

  int w = terrainMap.getWidth();
  int h = terrainMap.getHeight();

  std::cout << "\n---- World Map (" << w << "x" << h << ") ----\n";
  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      Unit* unit = field.getUnit(x, y);
      if (unit && unit->isAlive()) {
        std::cout << MAGENTA << unit->getSymbol() << RESET << ' ';
        continue;
      }

      if (base && x == base->getX() && y == base->getY()) {
        std::cout << BLUE << 'B' << RESET << ' ';
        continue;
      }

      NeutralObject* neutralObj = terrainMap.getNeutralObjectAt(x, y);
      if (neutralObj) {
        std::cout << neutralObj->getColoredSymbol() << ' ';
        continue;
      }

      auto cell = terrainMap.getTerrain(x, y);
      if (!cell) {
        std::cout << "? ";
        continue;
      }
      char sym = cell->getSymbol();
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
    std::cout << '\n';
  }
  std::cout << "Legend: " << BLUE << "B" << RESET << " Base | " << MAGENTA << "Unit" << RESET
            << " | \033[93m*\033[0m Rune | \033[91mx\033[0m Trap | \033[92m+\033[0m Fountain | "
            << "\033[96m@\033[0m Teleport | " << DARK_GREEN << "." << RESET << " Plains | " << GREEN
            << "T" << RESET << " Forest | " << YELLOW << "~" << RESET << " Swamp | " << WHITE << "^"
            << RESET << " Mountains\n";
}