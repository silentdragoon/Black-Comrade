#ifndef _MAPMANAGER2_H
#define _MAPMANAGER2_H

#include <OGRE/Ogre.h>

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>
#include <sys/types.h>
#include <dirent.h>
#include <string>
#include <errno.h>
#include <algorithm>
#include <math.h>
#include "waypoint.h"
#include "mapTile.h"
#include "constManager.h"
#include "const.h"

//#define MAPROOT "/home/user-pool/users/jw7082/mapparts/"

using namespace std;
using namespace Ogre;

class MapManager {

private:
    string MAPROOT;
	SceneManager *sceneManager;
    std::vector<Entity*> mapEntities;
    std::vector<Waypoint*> waypoints;

    void createTile(string dir, std::vector<int> connections, int x, int y);
    void setSpawnPoints();
    void attachLight( Real x, Real z);

public:
    int startx,starty; // Index location of the start square of the map
    MapTile* mts[Const::MAPSIZE][Const::MAPSIZE]; // Maptile storage

    MapManager(char* file, SceneManager *sceneManager);
    std::vector<Entity*> getMapPieces();
    Entity* getEntity(Vector3 *locn);
    void getMapEntities(Vector3 *locn, Entity** mps );
    MapTile* getMapTile(Vector3 *locn);
    Vector3 getActualPosition(MapTile* tile);
    string* getWaypoint(Vector3 *locn);
    std::vector<Vector3*> getSpawnPoints(Vector3 *locn); // Takes a location and returns the ring spawn places
    std::vector<Vector3*> getInitialSpawnPoints(); // Returns location inside the correct tile piece
    void makeConPieces();
    Vector3 getDynamicSpawnPoint(Vector3 *locn);
};

#endif
