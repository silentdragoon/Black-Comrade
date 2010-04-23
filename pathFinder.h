#ifndef PATH_FINDER_H
#define PATH_FINDER_H

// Includes all Ogre classes
#include <OGRE/Ogre.h>
#include "mapManager2.h"
#include "mapTile.h"
#include "pathTile.h"
#include <vector>
#include <math.h>
#include <limits>

#include <boost/math/distributions/uniform.hpp>
#include <boost/random.hpp>

class PathFinder {
private:
    MapManager *mapMgr;

    PathTile* start;
    PathTile* end;

    boost::mt19937 rng;

    std::vector<PathTile*> findNeighbours(PathTile* tile);
    PathTile* chooseNextTile(std::vector<PathTile*> &tiles, PathTile* dest);
    bool contains(std::vector<PathTile*> tiles, PathTile* tile);

public:
    PathFinder(MapManager *mapMgr);
    std::vector<MapTile*> findPath(MapTile* start, MapTile *end);
    MapTile *pickNextTile(MapTile* current, MapTile *previous);
};

#endif
