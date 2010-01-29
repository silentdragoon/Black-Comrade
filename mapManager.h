#ifndef _MAPMANAGER_H
#define _MAPMANAGER_H

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
#include "const.h"

#define MAPROOT "./mapparts/"

using namespace std;
using namespace Ogre;

class MapManager {

private:
    char geo[Const::MAPSIZE][Const::MAPSIZE]; // Store for the geography of the map
    MapTile* mts[Const::MAPSIZE][Const::MAPSIZE]; // Maptile storage
    int endx,endy; // Index location of the end square of the map

	SceneManager *sceneManager;
    vector<Entity*> mapEntities;
    vector<Waypoint*> waypoints;

    bool buildMap(char* file);
    int getMeshList(string dir, vector<string> &files, int x, int y);
    vector<int> getConnections(int x, int y);
    int cavernChecker(int x, int y, char type);
    void attachTile(SceneNode *sceneNode, string *file, int x, int y);
    void fetchTile(string dir, vector<int> connections, int x, int y,SceneNode *sceneNode);
    void setSpawnPoints();

public:
    int startx,starty; // Index location of the start square of the map

    MapManager(char* file, SceneManager *sceneManager);
    bool outputMap(SceneNode *sceneNode);
    
    vector<Entity*> getMapPieces();
    
    Entity* getEntity(Vector3 *locn);
    void getMapEntities(Vector3 *locn, Entity** mps );
    
    
    string* getWaypoint(Vector3 *locn);

    vector<Vector3*> getSpawnPoints(Vector3 *locn); // Takes a location and returns the ring spawn places
    vector<Vector3*> getInitialSpawnPoints(); // Returns location inside the correct tile piece

    Vector3 getDynamicSpawnPoint(Vector3 *locn);
};

#endif
