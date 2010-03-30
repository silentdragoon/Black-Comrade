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
        std::vector<Waypoint*> w;
        bool waypoint;
        bool empty;
        bool obj;
        
        bool southConn;
        bool eastConn;
        
        Vector3 *northSpawn;
        Vector3 *eastSpawn;
        Vector3 *southSpawn;
        Vector3 *westSpawn;

        int x;
        int y;

        std::vector<int> conns;

        MapTile *northTile;
        MapTile *eastTile;
        MapTile *southTile;
        MapTile *westTile;
        
        Entity *eastConnPieceEntity;
        Entity *southConnPieceEntity;

    public:
        bool hasWaypoint();
        bool isEmpty();
        bool isObj();

        void setObjective();

        void assignWaypoint(Waypoint *wa);
        void assignSpawnPoints(std::vector<Vector3*> sp);
        void setSpawn(int i, Vector3 *locn);
        void setAdjacent(int i, MapTile *adj);
        void setConnections(std::vector<int> c);
        
        void setEastConnPiece( Entity *e );
        void setSouthConnPiece( Entity *e );

        Vector3* getSpawn(int i);
        MapTile* getAdjacent(int i);
        std::vector<int> getConnections();
        std::vector<Vector3*> getSpawnPoints();

        std::vector<Waypoint*> getWaypoints();
        Entity* getEntity();
        SceneNode* getSceneNode();
        
        bool eastConnected();
        bool southConnected();
        
        //ents has space allocated for 5 ents. Return tile and 4 con piece pointers
        // if there arnt 4 conn Pieces then pad with NULL
        void getTileAndConnectionEntities(Entity **ents);
        
        Entity* getSouthConnEntity();
        Entity* getEastConnEntity();

        int getX();
        int getY();

        MapTile(SceneNode *node, Entity *e, int x, int y);
        MapTile();
        ~MapTile();
};

#endif

