#include "NeutralObjects/Teleport.h"

#include "Strategies/TeleportStrategy.h"

Teleport::Teleport(int x, int y, int areaWidth, int areaHeight, const TerrainMap* map,
                   const Base* base, const GameField* field)
    : NeutralObject(x, y,
                    std::make_unique<TeleportStrategy>(areaWidth, areaHeight, map, base, field)) {}

std::string Teleport::getType() const { return "Teleport"; }

char Teleport::getSymbol() const { return '@'; }

std::string Teleport::getColoredSymbol() const { return "\033[96m@\033[0m"; }