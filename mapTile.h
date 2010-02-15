#ifndef _MAPTILE_H 
#define _MAPTILE_H

#include <OGRE/Ogre.h>
#include <string>
#include <vector>
#include <iostream>
#include "waypoint.h"

using namespace Ogre;
using namespace std;

class MapTile 
{
    private:
        std::vector<Vector3*> spawnPoints;
        SceneNode *node;
        Entity *e;
        Waypoint *w;
        bool waypoint;
        bool empty;
        
        bool southConn;
        bool eastConn;
        
        Vector3 *northSpawn;
        Vector3 *eastSpawn;
        Vector3 *southSpawn;
        Vector3 *westSpawn;

        std::vector<int> conns;

        MapTile *northTile;
        MapTile *eastTile;
        MapTile *southTile;
        MapTile *westTile;

    public:
        bool hasWaypoint();
        bool isEmpty();

        void assignWaypoint(Waypoint *wa);
        void assignSpawnPoints(std::vector<Vector3*> sp);
        void setSpawn(int i, Vector3 *locn);
        void setAdjacent(int i, MapTile *adj);
        void setConnections(std::vector<int> c);

        Vector3* getSpawn(int i);
        MapTile* getAdjacent(int i);
        std::vector<int> getConnections();
        std::vector<Vector3*> getSpawnPoints();

        Waypoint * getWaypoint();
        Entity* getEntity();
        SceneNode* getSceneNode();
        
        bool eastConnected();
        bool southConnected();

        MapTile(SceneNode *node, Entity *e);
        MapTile();
        ~MapTile();
};

#endif

