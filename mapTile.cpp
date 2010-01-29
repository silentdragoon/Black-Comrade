#include "mapTile.h"

MapTile::MapTile(SceneNode *node, Entity *e) :
    node(node),
    e(e)
{
    empty = false;
    waypoint = false;
}

MapTile::MapTile()
{
    empty = true;
    waypoint = false;
}

MapTile::~MapTile()
{}

Entity* MapTile::getEntity()
{
    return e;
}

SceneNode* MapTile::getSceneNode()
{
    return node;    
}

Waypoint * MapTile::getWaypoint()
{
    return w;
}

bool MapTile::hasWaypoint()
{
    return waypoint;
}

bool MapTile::isEmpty()
{
    return empty;
}

void MapTile::assignWaypoint(Waypoint *wa)
{
    w = wa;
    waypoint = true;
}

void MapTile::assignSpawnPoints(vector<Vector3*> sp)
{
    spawnPoints = sp;
}

vector<Vector3*> MapTile::getSpawnPoints()
{
    return spawnPoints;
}

void MapTile::setSpawn(int i, Vector3 *locn)
{
    if(i==1) northSpawn = locn;
    if(i==2) eastSpawn = locn;
    if(i==3) southSpawn = locn;
    if(i==4) westSpawn = locn;
}

Vector3* MapTile::getSpawn(int i)
{
    if(i==1) return northSpawn;
    if(i==2) return eastSpawn;
    if(i==3) return southSpawn;
    if(i==4) return westSpawn;
    return NULL;
}
