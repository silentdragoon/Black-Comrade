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

MapTile* PathFinder::pickNextTile(MapTile *currentMap, MapTile *lastMap,
                                  std::vector<MapTile*> prohibitedTiles) {

    PathTile *current = new PathTile(currentMap);

    std::vector<PathTile*> options = findNeighbours(current);
    std::vector<PathTile*> validOptions;

    for (int i=0; i < options.size() ; i++) {
        PathTile *optionTile = options.at(i);
        bool valid = true;
        for (int j=0; j < prohibitedTiles.size(); j++) {
            MapTile *prohibitedTile = prohibitedTiles.at(j);
            if (optionTile->getMapTile() == prohibitedTile ||
                optionTile->getMapTile() == lastMap) {
                valid = false;
                break;
            }
        }
        if (valid) validOptions.push_back(optionTile);
    }

    if (validOptions.size() == 0) {
        if (options.size() == 1)
            // Forced to go back the way we came
            return lastMap;
        else
            // Unlikely case - stay where we are
            return currentMap;
    } else {
        // We have some options
        rng.seed(static_cast<unsigned int>(std::time(0)));

        boost::uniform_int<> six(0,validOptions.size()-1);

        boost::variate_generator<boost::mt19937&, boost::uniform_int<> > die(rng, six);
        int option = die();
        PathTile *chosenTile = validOptions.at(option);
        //std::cout << "Chose option " << option+1 << " of " << validOptions.size() << "\n";

        return chosenTile->getMapTile();
    }
}
