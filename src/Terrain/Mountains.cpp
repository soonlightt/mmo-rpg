#include "Terrain/Mountains.h"

Mountains::Mountains(int x, int y) : Terrain(x, y) {}

std::string Mountains::getType() const { return "Mountains"; }

char Mountains::getSymbol() const { return '^'; }

bool Mountains::isPassable() const { return false; }

std::string Mountains::getEffectDescription() const { return "Mountains: Impassable"; }

void Mountains::onEnter(Unit&) {}
void Mountains::onLeave(Unit&) {}