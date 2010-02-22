#include "pathFinder.h"

PathFinder::PathFinder(MapManager *mapMgr)
    : mapMgr(mapMgr)
{
    // Example usage:
    //std::vector<MapTile*> path = findPath(mapMgr->mts[3][0], mapMgr->mts[8][9]);
    //for(std::vector<MapTile*>::const_iterator ite=path.begin();ite!=path.end();++ite) {
    //    MapTile* tile = *ite;
    //    std::cout << "maptile: " << tile->getX() << " " << tile->getY() << std::endl;
    //}   
}

PathTile* PathFinder::chooseNextTile(std::vector<PathTile*> &tiles, PathTile* dest) {
    PathTile *currentBest;
    double bestF = numeric_limits<double>::max();
    int bestI = 0;
    int i = 0;

    for(std::vector<PathTile*>::const_iterator ite=tiles.begin();ite!=tiles.end();++ite) {
        PathTile * tile = *ite;
        double temp = tile->getF(dest);
        if (temp < bestF) {
            currentBest = tile;
            bestF = temp;
            bestI = i;
        }
        i++;
    }
    if (tiles.size() > 0) tiles.erase(tiles.begin()+bestI);

    return currentBest;
}

bool PathFinder::contains(std::vector<PathTile*> tiles, PathTile* tile) {
    for(std::vector<PathTile*>::const_iterator ite=tiles.begin();ite!=tiles.end();++ite) {
        PathTile* temp = *ite;
        if (temp->getX() == tile->getX() && temp->getY() == tile->getY()) return true;
    }
    return false;
}

std::vector<PathTile*> PathFinder::findNeighbours(PathTile* tile) {
    std::vector<PathTile*> neighbours = std::vector<PathTile*>();
    if(tile->getAdjacent(1)!=0) {
        neighbours.push_back(tile->getAdjacent(1));
    }
    if(tile->getAdjacent(2)!=0) {
        neighbours.push_back(tile->getAdjacent(2));
    }
    if(tile->getAdjacent(3)!=0) {
        neighbours.push_back(tile->getAdjacent(3));
    }
    if(tile->getAdjacent(4)!=0) {
        neighbours.push_back(tile->getAdjacent(4));
    }
    return neighbours;
}

std::vector<MapTile*> PathFinder::findPath(MapTile* mapStart, MapTile* mapEnd) {
    std::vector<PathTile*> open = std::vector<PathTile*>();
    std::vector<PathTile*> closed = std::vector<PathTile*>();
    std::vector<PathTile*> path = std::vector<PathTile*>();
    PathTile *end = new PathTile(mapEnd);
    PathTile *currentBest = 0;
    PathTile *lastBest = 0;
    bool found = false;

    open.push_back(new PathTile(mapStart));

    while (open.size() != 0 && found == false) {
        currentBest = chooseNextTile(open,end);

        if (currentBest != 0 && currentBest->getX() == end->getX() && currentBest->getY() == end->getY()) {
            currentBest->setParent(lastBest);
            found = true;
            break;
        }

        closed.push_back(currentBest);  
        std::vector<PathTile*> neighbours = findNeighbours(currentBest);

        for(std::vector<PathTile*>::const_iterator ite=neighbours.begin();ite!=neighbours.end();++ite) {
            PathTile * neighbour = *ite;
            if (contains(closed,neighbour)) continue;

            double tentativeG = currentBest->getG() + 1.0;

            if (!contains(open,neighbour)) {
                open.push_back(neighbour);
                neighbour->setParent(currentBest);
                neighbour->setG(tentativeG);
            } else if (tentativeG < neighbour->getG()) {
                neighbour->setParent(currentBest);
                neighbour->setG(tentativeG);
            }
        }
        lastBest = currentBest;
    }

    if (!found) {
        std::cerr << "Impossible path" << std::endl;
        return std::vector<MapTile*>();
    } else {
        return currentBest->getPath();
    }
}

MapTile* PathFinder::pickNextTile(MapTile *currentMap, MapTile *lastMap) {
    PathTile *current = new PathTile(currentMap);
    std::vector<PathTile*> options = findNeighbours(current);
    if (options.size() == 0) {
        // Unlikely case - stay where you are
        return currentMap;
    } else if (options.size() == 1) {
        // Dead end or possible initial game state
        if (lastMap == 0) return options.at(0)->getMapTile();
        return lastMap;
    } else {
        // We have some options
        if (options.at(0)->getMapTile() != lastMap) return options.at(0)->getMapTile();
        if (options.at(1)->getMapTile() != lastMap) return options.at(1)->getMapTile();
        if (options.at(2)->getMapTile() != lastMap) return options.at(2)->getMapTile();
        return options.at(3)->getMapTile();
    }
}
