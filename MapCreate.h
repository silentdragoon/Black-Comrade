#ifndef MAP_EDIT_H
#define MAP_EDIT_H

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

using namespace std;

class MapCreate {

private:
    char geo[MAPSIZE][MAPSIZE]; // Store for the geography of the map
    int startx,starty; // Index location of the start square of the map
    int endx,endy; // Index location of the end square of the map

    bool buildMap(char* file);
    void addEnemies();
    bool outputMap();
    int getMeshList(string dir, vector<string> &files);
    vector<int> getConnections(int x, int y);

public:
    MapCreate(char* file);

};

#endif