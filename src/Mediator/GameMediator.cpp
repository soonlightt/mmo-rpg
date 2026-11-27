#include "Mediator/GameMediator.h"

GameMediator::GameMediator(int width, int height, size_t maxUnits, int baseX, int baseY,
                           int baseHealth, size_t baseMaxUnits, int baseResources)
    : base(std::make_shared<Base>(baseX, baseY, baseHealth, baseMaxUnits, 5, baseResources)),
      field(width, height, maxUnits),
      terrainMap(width, height) {}

GameField& GameMediator::getField() { return field; }
const GameField& GameMediator::getField() const { return field; }
TerrainMap& GameMediator::getTerrainMap() { return terrainMap; }
const TerrainMap& GameMediator::getTerrainMap() const { return terrainMap; }
std::shared_ptr<Base> GameMediator::getBase() const { return base; }

bool GameMediator::resolveUnitType(const std::string& unitType, const UnitFactory*& outFactory,
                                   int& outSubType) const {
  if (unitType == "Lifestealer" || unitType == "lifestealer") {
    outFactory = &carryFactory;
    outSubType = 1;
    return true;
  }
  if (unitType == "carry" || unitType == "Carry" || unitType == "PhantomAssassin" ||
      unitType == "phantom_assassin") {
    outFactory = &carryFactory;
    outSubType = 2;
    return true;
  }
  if (unitType == "midder" || unitType == "Midder" || unitType == "ArcWarden" ||
      unitType == "arc_warden") {
    outFactory = &midderFactory;
    outSubType = 1;
    return true;
  }
  if (unitType == "Invoker" || unitType == "invoker") {
    outFactory = &midderFactory;
    outSubType = 2;
    return true;
  }
  if (unitType == "Dazzle" || unitType == "dazzle") {
    outFactory = &supportFactory;
    outSubType = 1;
    return true;
  }
  if (unitType == "support" || unitType == "Support" || unitType == "WitchDoctor" ||
      unitType == "witch_doctor") {
    outFactory = &supportFactory;
    outSubType = 2;
    return true;
  }
  return false;
}

Unit* GameMediator::findUnitById(int unitId) const {
  for (auto& u : field) {
    if (u.getId() == unitId) {
      return &u;
    }
  }
  return nullptr;
}

bool GameMediator::isBuildingAt(int x, int y) const {
  if (!base) {
    return false;
  }
  for (const auto& b : base->getBuildings()) {
    if (b && b->getX() == x && b->getY() == y) {
      return true;
    }
  }
  return false;
}

bool GameMediator::createUnit(const std::string& unitType, int x, int y, int cost) {
  const UnitFactory* factory = nullptr;
  int subType = 0;
  if (!resolveUnitType(unitType, factory, subType)) {
    std::cout << "[Mediator] Create failed: unknown unit type '" << unitType << "'.\n";
    return false;
  }

  if (!terrainMap.isValidCoord(x, y)) {
    std::cout << "[Mediator] Create failed: invalid coordinates (" << x << ", " << y << ").\n";
    return false;
  }

  if ((base && x == base->getX() && y == base->getY()) || isBuildingAt(x, y)) {
    std::cout << "[Mediator] Create failed: cell (" << x << ", " << y
              << ") is occupied by a building.\n";
    return false;
  }

  auto terrain = terrainMap.getTerrain(x, y);
  if (!terrain || !terrain->isPassable()) {
    std::cout << "[Mediator] Create failed: terrain at (" << x << ", " << y << ") is impassable.\n";
    return false;
  }

  if (field.getUnit(x, y) != nullptr) {
    std::cout << "[Mediator] Create failed: cell (" << x << ", " << y << ") is already occupied.\n";
    return false;
  }

  if (base && (base->getActiveUnitsCount() >= base->getMaxUnits() || base->getResources() < cost)) {
    std::cout << "[Mediator] Create failed: base unit limit reached or insufficient resources.\n";
    return false;
  }

  auto rawUnit = factory->createUnit(subType, x, y);
  if (!rawUnit) {
    return false;
  }

  if (base && base->hasUpgrade("attack_upgrade")) {
    rawUnit->modifyAttack(5);
  }

  if (base && !base->spendResources(cost)) {
    return false;
  }

  if (!field.addUnit(std::move(rawUnit))) {
    if (base) {
      base->addResources(cost);
    }
    return false;
  }

  Unit* placed = field.getUnit(x, y);
  if (placed) {
    if (base) {
      placed->addObserver(base.get());
    }
    std::cout << "[Mediator] Created " << placed->getType() << " (ID: " << placed->getId()
              << ") at (" << x << ", " << y << ").\n";
    terrain->onEnter(*placed);

    NeutralObject* neutralObj = terrainMap.getNeutralObjectAt(x, y);
    if (neutralObj && neutralObj->getIsActive()) {
      *placed + *neutralObj;
    }
  }
  return true;
}

bool GameMediator::moveUnit(int unitId, int toX, int toY) {
  Unit* unit = findUnitById(unitId);
  if (!unit) {
    std::cout << "[Mediator] Move failed: unit ID " << unitId << " not found.\n";
    return false;
  }
  return moveUnitByCoords(unit->getX(), unit->getY(), toX, toY);
}

bool GameMediator::moveUnitByCoords(int fromX, int fromY, int toX, int toY) {
  Unit* unit = field.getUnit(fromX, fromY);
  if (!unit || !unit->isAlive()) {
    std::cout << "[Mediator] Move failed: no alive unit at (" << fromX << ", " << fromY << ").\n";
    return false;
  }

  if (!terrainMap.isValidCoord(toX, toY)) {
    std::cout << "[Mediator] Move failed: target (" << toX << ", " << toY << ") out of bounds.\n";
    return false;
  }

  if ((base && toX == base->getX() && toY == base->getY()) || isBuildingAt(toX, toY)) {
    std::cout << "[Mediator] Move failed: target (" << toX << ", " << toY
              << ") is occupied by a building.\n";
    return false;
  }

  if (field.getUnit(toX, toY) != nullptr) {
    std::cout << "[Mediator] Move failed: target (" << toX << ", " << toY << ") is occupied.\n";
    return false;
  }

  auto targetTerrain = terrainMap.getTerrain(toX, toY);
  if (!targetTerrain || !targetTerrain->isPassable()) {
    std::cout << "[Mediator] Move blocked by TerrainProxy at (" << toX << ", " << toY << ")!\n";
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
  std::cout << "[Mediator] " << unit->getType() << " (ID: " << unit->getId() << ") moved from ("
            << fromX << ", " << fromY << ") to (" << toX << ", " << toY << ").\n";

  targetTerrain->onEnter(*unit);

  NeutralObject* neutralObj = terrainMap.getNeutralObjectAt(toX, toY);
  if (neutralObj && neutralObj->getIsActive()) {
    *unit + *neutralObj;
  }
  return true;
}

bool GameMediator::attackUnit(int attackerId, int targetId) {
  Unit* attacker = findUnitById(attackerId);
  Unit* target = findUnitById(targetId);

  if (!attacker || !target || !attacker->isAlive() || !target->isAlive() ||
      attackerId == targetId) {
    std::cout << "[Modiator] Attack failed: invalid attacker or target ID.\n";
    return false;
  }

  int targetX = target->getX();
  int targetY = target->getY();
  int rawDamage = attacker->getAttack().getValue();

  std::cout << "[Mediator] " << attacker->getType() << " (ID: " << attackerId << ") attacks "
            << target->getType() << " (ID: " << targetId << ") for " << rawDamage
            << " raw damage.\n";
  target->takeDamage(rawDamage);

  if (target->isAlive()) {
    std::cout << "[Mediator] " << target->getType() << " (ID: " << target->getId()
              << ") remaining HP: " << target->getHealth().getValue() << "/"
              << target->getHealth().getMaxValue() << "\n";
  } else {
    std::cout << "[Mediator] " << target->getType() << " (ID: " << target->getId()
              << ") was destroyed and removed from field.\n";
    field.removeUnit(target->getX(), target->getY());
  }
  return true;
}

bool GameMediator::collectResource(int unitId, int resourceId) {
  Unit* unit = findUnitById(unitId);
  if (!unit || !unit->isAlive() || resourceId <= 0) {
    std::cout << "[Mediator] Collect failed: invalid unit ID or resource ID.\n";
    return false;
  }

  int gathered = 30;
  if (base) {
    base->addResources(gathered);
  }
  std::cout << "[Mediator] Unit " << unit->getType() << " (ID: " << unitId
            << ") collected resource #" << resourceId << " (+" << gathered
            << " resources to Base).\n";
  return true;
}

bool GameMediator::healUnit(int healerId, int targetId) {
  Unit* healer = findUnitById(healerId);
  Unit* target = findUnitById(targetId);

  if (!healer || !target || !healer->isAlive() || !target->isAlive()) {
    std::cout << "[Mediator] Heal failed: invalid healer or target ID.\n";
    return false;
  }

  int healAmount = 25;
  target->heal(healAmount);
  std::cout << "[Mediator] " << healer->getType() << " (ID: " << healerId << ") healed "
            << target->getType() << " (ID: " << targetId << ") for " << healAmount
            << " HP. Current HP: " << target->getHealth().getValue() << "/"
            << target->getHealth().getMaxValue() << ".\n";
  return true;
}

bool GameMediator::buildBuilding(const std::string& buildingType, int x, int y, int cost) {
  if (!base || !terrainMap.isValidCoord(x, y)) {
    std::cout << "[Mediator] Build failed: invalid coordinates or missing base.\n";
    return false;
  }

  if ((x == base->getX() && y == base->getY()) || isBuildingAt(x, y) ||
      field.getUnit(x, y) != nullptr) {
    std::cout << "[Mediator] Build failed: cell (" << x << ", " << y << ") is already occupied.\n";
    return false;
  }

  auto terrain = terrainMap.getTerrain(x, y);
  if (!terrain || !terrain->isPassable()) {
    std::cout << "[Mediator] Build failed: terrain at (" << x << ", " << y << ") is impassable.\n";
    return false;
  }

  if (!base->buildBuilding(buildingType, x, y, cost)) {
    std::cout << "[Mediator] Build failed: insufficient resources or invalid building.\n";
    return false;
  }

  int newId = base->getBuildings().back()->getId();
  std::cout << "[Mediator] Built " << buildingType << " (ID: " << newId << ") at (" << x << ", "
            << y << "). Remaining resources: " << base->getResources() << ".\n";
  return true;
}

bool GameMediator::research(const std::string& techId, int cost) {
  if (!base) {
    return false;
  }
  if (base->hasUpgrade(techId)) {
    std::cout << "[Mediator] Research skipped: '" << techId << "' is already researched.\n";
    return false;
  }
  if (!base->research(techId, cost)) {
    std::cout << "[Mediator] Research failed: insufficient resources or invalid tech.\n";
    return false;
  }
  if (techId == "attack_upgrade") {
    for (auto& u : field) {
      if (u.isAlive()) {
        u.modifyAttack(5);
      }
    }
  }
  return true;
}

bool GameMediator::distributeResources(const std::string& resourceType,
                                       const std::string& targetName, int amount) {
  if (!base || !base->distributeResources(resourceType, targetName, amount)) {
    std::cout << "[Mediator] Distribute failed: invalid target, amount, or insufficient "
                 "resources.\n";
    return false;
  }
  return true;
}

bool GameMediator::produceUnit(const std::string& unitType, int buildingId, int cost) {
  if (!base) {
    return false;
  }
  auto building = base->findBuildingById(buildingId);
  if (!building) {
    std::cout << "[Mediator] Produce failed: building ID " << buildingId << " not found.\n";
    return false;
  }

  const int dx[4] = {1, 0, -1, 0};
  const int dy[4] = {0, 1, 0, -1};
  for (int i = 0; i < 4; ++i) {
    int sx = building->getX() + dx[i];
    int sy = building->getY() + dy[i];
    if (terrainMap.isValidCoord(sx, sy) && !isBuildingAt(sx, sy) &&
        !(sx == base->getX() && sy == base->getY()) && field.getUnit(sx, sy) == nullptr) {
      auto t = terrainMap.getTerrain(sx, sy);
      if (t && t->isPassable()) {
        return createUnit(unitType, sx, sy, cost);
      }
    }
  }

  std::cout << "[Mediator] Produce failed: no free passable cell around building #" << buildingId
            << ".\n";
  return false;
}

std::string GameMediator::getStats(const std::string& filter) const {
  std::ostringstream out;
  bool showAll = filter.empty() || filter == "all";

  if (showAll || filter == "resources") {
    out << "=== Base & Resources ===\n";
    if (base) {
      out << "Base at (" << base->getX() << ", " << base->getY() << ") | HP: " << base->getHealth()
          << "/" << base->getMaxHealth() << " | Resources: " << base->getResources() << "\n";
      for (const auto& kv : base->getAllocatedResources()) {
        out << "  Allocated [" << kv.first << "]: " << kv.second << "\n";
      }
      out << "Researched upgrades: ";
      if (base->getResearchedUpgrades().empty()) {
        out << "none\n";
      } else {
        for (const auto& upg : base->getResearchedUpgrades()) {
          out << upg << " ";
        }
        out << "\n";
      }
    }
  }

  if (showAll || filter == "units") {
    out << "=== Alive Units (" << field.size() << ") ===\n";
    for (const auto& u : field) {
      out << "  ID " << u.getId() << ": " << u.getType() << " at (" << u.getX() << ", " << u.getY()
          << ") | HP: " << u.getHealth().getValue() << "/" << u.getHealth().getMaxValue()
          << " | ARM: " << u.getArmor().getValue() << " | ATK: " << u.getAttack().getValue()
          << "\n";
    }
  }

  if (showAll || filter == "buildings") {
    out << "=== Buildings (" << (base ? base->getBuildings().size() : 0) << ") ===\n";
    if (base) {
      for (const auto& b : base->getBuildings()) {
        out << "  ID " << b->getId() << ": " << b->getType() << " at (" << b->getX() << ", "
            << b->getY() << ")\n";
      }
    }
  }

  return out.str();
}

void GameMediator::printWorld() const {
  int w = terrainMap.getWidth();
  int h = terrainMap.getHeight();

  std::cout << "\n=== World Map (" << w << "x" << h << ") ===\n";
  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      Unit* unit = field.getUnit(x, y);
      if (unit && unit->isAlive()) {
        std::cout << "\033[95m" << unit->getSymbol() << "\033[0m ";
        continue;
      }
      if (base && x == base->getX() && y == base->getY()) {
        std::cout << "\033[94mB\033[0m ";
        continue;
      }
      if (isBuildingAt(x, y)) {
        std::cout << "\033[93mH\033[0m ";
        continue;
      }
      NeutralObject* neutralObj = terrainMap.getNeutralObjectAt(x, y);
      if (neutralObj) {
        std::cout << neutralObj->getColoredSymbol() << ' ';
        continue;
      }
      auto cell = terrainMap.getTerrain(x, y);
      std::cout << (cell ? cell->getSymbol() : '?') << ' ';
    }
    std::cout << '\n';
  }
}