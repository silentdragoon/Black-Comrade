#ifndef PATH_TILE_H
#define PATH_TILE_H

#include "mapTile.h"

class PathTile {
private:
    PathTile *parent;
    MapTile *mapTile;

    int x;
    int y;
    double g;
    double h;

    double euclideanDistance(PathTile *b);

public:
    PathTile(MapTile *mapTile);

    int getX();
    int getY();

    MapTile* getMapTile();
    PathTile *getAdjacent(int port);

    PathTile *getParent();
    void setParent(PathTile *parent);

    std::vector<MapTile*> getPath();

    void setG(double g);

    double getG();
    double getF(PathTile* dest);
    double getH(PathTile* dest);

};

#endif
