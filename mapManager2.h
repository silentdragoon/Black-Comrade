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
#include "mapPieceChoices.h"
#include "lightManager.h"

#include <boost/math/distributions/uniform.hpp>
#include <boost/random.hpp>

//#define MAPROOT "/home/user-pool/users/jw7082/mapparts/"

using namespace std;
using namespace Ogre;

class MapManager {

private:

    boost::mt19937 rng;

    LightManager *lightMgr;

    int objx,objy; // Location of the objective tile
    int chosenIndex;
    string MAPROOT;
	SceneManager *sceneManager;
    std::vector<Entity*> mapEntities;
    std::vector<Waypoint*> waypoints;
    std::vector<int> chosenPieces;
    MapPieceChoices *pieceChoices;
    bool objective;

    void loadMap(char *file);
    void createTile(string dir, std::vector<int> connections, int x, int y, int pieceToChoose = -1);
    void setSpawnPoints();
    void attachLight( Real x, Real z);
    int getNextChosenPiece();

public:
    int startx,starty; // Index location of the start square of the map
    MapTile* mts[Const::MAPSIZE][Const::MAPSIZE]; // Maptile storage

    MapManager(char* file, SceneManager *sceneManager, LightManager *lightMgr);
    MapManager(char* file, MapPieceChoices *pieceChoices, SceneManager *sceneManager, LightManager *lightMgr);

    Entity* getEntity(Vector3 *locn);
    //void getMapEntities(Vector3 *locn, Entity** mps );
    MapTile* getMapTile(Vector3 *locn);
    Vector3 getActualPosition(MapTile* tile);
    std::vector<string*> getWaypoints(Vector3 *locn);
    std::vector<Vector3*> getSpawnPoints(Vector3 *locn); // Takes a location and returns the ring spawn places
    std::vector<Vector3*> getInitialSpawnPoints(); // Returns location inside the correct tile piece
    void makeConPieces();
    Vector3 getDynamicSpawnPoint(Vector3 *locn);
    Vector3 getObjectivePosition();
    std::vector<Entity*> getMapEntitiesForCollision();
    //mps has 5 space for 5 entity pointers
    void getEntitiesForCollisionFromAPosition(Vector3 *locn, Entity** mps);
    MapPieceChoices *getChosenPieces();
};

#endif
