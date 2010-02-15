#include "pathTile.h"

PathTile::PathTile(MapTile* mapTile) :
    mapTile(mapTile)
{
    x = mapTile->getX();
    y = mapTile->getY();
    g = 1.0;
}

int PathTile::getX() { return x; }

int PathTile::getY() { return y; }

void PathTile::setParent(PathTile *newParent) {
    parent = newParent;
    std::cout << "set parent" << std::endl;
}

PathTile* PathTile::getParent() { return parent; }

std::vector<MapTile*> PathTile::getPath() {
    std::vector<MapTile*> path;
    if (parent == 0) {
        path = std::vector<MapTile*>();
    } else {
        path = parent->getPath();
        std::cout << path.size() << std::endl;
    }
    path.push_back(getMapTile());
    return path;
}

MapTile* PathTile::getMapTile() { return mapTile; }

PathTile* PathTile::getAdjacent(int port) {
    MapTile *adjacent = mapTile->getAdjacent(port);
    if (adjacent != 0)
        return new PathTile(adjacent);
    else
        return 0;
}

void PathTile::setG(double newG) {
    g = newG;
}

double PathTile::getG() { return g; }

double PathTile::getF(PathTile *dest) {
    return g + getH(dest);
}

double PathTile::getH(PathTile *dest) {
    return euclideanDistance(this,dest);
}

double PathTile::euclideanDistance(PathTile *a, PathTile *b) {
    double result = 0.0;
    result = sqrt(pow((double)a->getX()-b->getX(),2.0) + 
                  pow((double)a->getY()-b->getY(),2.0));
    std::cout << result << std::endl;
    return result;
}
