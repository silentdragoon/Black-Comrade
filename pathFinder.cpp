#include "pathFinder.h"
#include <math.h>
#include <limits>

PathFinder::PathFinder(MapManager *mapMgr)
    : mapMgr(mapMgr)
{
    open = std::vector<PathTile*>();
    closed = std::vector<PathTile*>();
    initialize();

    std::vector<MapTile*> neighbours = findPath(mapMgr->mts[3][0], mapMgr->mts[7][2]);
    for(std::vector<MapTile*>::const_iterator ite=neighbours.begin();ite!=neighbours.end();++ite) {
        MapTile * neighbour = *ite;
        std::cout << "maptile: " << neighbour->getX() << " " << neighbour->getY() << std::endl;
    }
    
}

void PathFinder::initialize() {
    for(int xpos=0;xpos<Const::MAPSIZE;xpos++) {
        for(int ypos=0;ypos<Const::MAPSIZE;ypos++) {
            if(!mapMgr->mts[xpos][ypos]->isEmpty()) {
                //open.push_back(new PathTile(mapMgr->mts[xpos][ypos]));
            }
        }
    }
}

PathTile* PathFinder::chooseNextTile(std::vector<PathTile*> tiles, PathTile* dest) {
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
        std::cout << tile->getX() << " " << tile->getY() << " compared to " << temp->getX() << " " << temp->getY() << std::endl;
        if (temp->getX() == tile->getX() && temp->getY() == tile->getY()) return true;
    }
    return false;
}

double PathFinder::euclideanDistance(PathTile *a, PathTile *b) {
    double result = 0.0;
    result = sqrt(pow((double)a->getX()-b->getX(),2.0) + 
                  pow((double)a->getY()-b->getY(),2.0));
    return result;
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

std::vector<MapTile*> PathFinder::findPath(MapTile* start, MapTile* mapEnd) {
    std::vector<PathTile*> path = std::vector<PathTile*>();
    bool found = false;

    PathTile *end = new PathTile(mapEnd);
    PathTile *currentBest = 0;
    PathTile *lastBest = 0;

    open.push_back(new PathTile(start));
    //open is [3,0]
    //closed is 

    while (open.size() != 0 && found == false) {
        //std::cout << "loop" << std::endl;
        currentBest = chooseNextTile(open,end);
        //open is
        //closed is

        //std::cout << "Current Best: " << currentBest->getX() << " " << currentBest->getY() << std::endl;

        if (currentBest != 0 && currentBest->getX() == end->getX() && currentBest->getY() == end->getY()) {
            currentBest->setParent(lastBest);
            found = true;
            std::cout << "FOUND" << std::endl;
            break;
        }

        closed.push_back(currentBest);
        //open is
        //closed is [3,0]
        std::cout << "Num Closed: " << closed.size() << std::endl;
        
        std::vector<PathTile*> neighbours = findNeighbours(currentBest);
        //neighbours is [3,1]
        //closed is [3,0]

        //std::cout << "Num Neighbours: " << neighbours.size() << std::endl;
        for(std::vector<PathTile*>::const_iterator ite=neighbours.begin();ite!=neighbours.end();++ite) {
            PathTile * neighbour = *ite;
            //std::cout << "Neighbour: " << neighbour->getX() << " " << neighbour->getY() << std::endl;

            if (contains(closed,neighbour)) {
                // Closed contains neighbour
                std::cout << "contained in closed" << std::endl;
                continue;
            }
            double tentativeG = currentBest->getG() + 1.0;
            //2
            //std::cout << "Tentative G: " << tentativeG << std::endl;

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
        std::cout << "Impossible path" << std::endl;
        return std::vector<MapTile*>();
    } else {
        return currentBest->getPath();
    }
    
    return std::vector<MapTile*>();
}
