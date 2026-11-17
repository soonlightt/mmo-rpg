#include "Terrain/TerrainProxy.h"

#include "Terrain/Plains.h"

TerrainProxy::TerrainProxy(int x, int y, std::shared_ptr<Terrain> terrain)
    : Terrain(x, y), realTerrain(terrain ? std::move(terrain) : std::make_shared<Plains>(x, y)) {}
