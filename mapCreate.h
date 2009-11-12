#ifndef MAP_EDIT_H
#define MAP_EDIT_H

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

#define MAPSIZE 30
#define MAPROOT "./mapparts/"

#define TILE_SIZE 200

using namespace std;
using namespace Ogre;

class MapCreate {

private:
    char geo[MAPSIZE][MAPSIZE]; // Store for the geography of the map
    int startx,starty; // Index location of the start square of the map
    int endx,endy; // Index location of the end square of the map

	SceneManager *sceneManager;

    bool buildMap(char* file);
    void addEnemies();
    int getMeshList(string dir, vector<string> &files, int x, int y);
    vector<int> getConnections(int x, int y);
    int cavernChecker(int x, int y, char type);
    void attachTile(SceneNode *sceneNode, string *file, int x, int y);

public:
    MapCreate(char* file, SceneManager *sceneManager);
	bool outputMap(SceneNode *sceneNode);
};

#endif
