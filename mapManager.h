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

#define MAPSIZE 30
#define MAPROOT "./mapparts/"

#define TILE_SIZE 400

using namespace std;
using namespace Ogre;

class MapManager {

private:
    char geo[MAPSIZE][MAPSIZE]; // Store for the geography of the map
    int endx,endy; // Index location of the end square of the map

	SceneManager *sceneManager;
    vector<Entity*> mapEntities;
    vector<Waypoint*> waypoints;

    bool buildMap(char* file);
    void addEnemies();
    int getMeshList(string dir, vector<string> &files, int x, int y);
    vector<int> getConnections(int x, int y);
    int cavernChecker(int x, int y, char type);
    void attachTile(SceneNode *sceneNode, string *file, int x, int y);
    void fetchTile(string dir, vector<int> connections, int x, int y,SceneNode *sceneNode);

public:
    int startx,starty; // Index location of the start square of the map

    MapManager(char* file, SceneManager *sceneManager);
    bool outputMap(SceneNode *sceneNode);
    
    vector<Entity*> getMapPieces();
    
    Entity* getEntity(Vector3 *locn);
    string* getWaypoint(Vector3 *locn);
};

#endif
