#ifndef PATH_FINDER_H
#define PATH_FINDER_H

// Includes all Ogre classes
#include <OGRE/Ogre.h>
#include "mapManager.h"
#include "mapTile.h"
#include "pathTile.h"
#include <vector>

class PathFinder {
private:
    MapManager *mapMgr;

    PathTile* start;
    PathTile* end;

    std::vector<PathTile*> findNeighbours(PathTile* tile);
    PathTile* chooseNextTile(std::vector<PathTile*> &tiles, PathTile* dest);
    bool contains(std::vector<PathTile*> tiles, PathTile* tile);

public:
    PathFinder(MapManager *mapMgr);
    std::vector<MapTile*> findPath(MapTile* start, MapTile *end);
};

#endif
