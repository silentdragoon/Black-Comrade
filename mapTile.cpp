#include "mapTile.h"

MapTile::MapTile(SceneNode *node, Entity *e) :
    node(node),
    e(e)
{
    empty = false;
    waypoint = false;

    northTile = NULL;
    eastTile = NULL;
    southTile = NULL;
    westTile = NULL;
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

void MapTile::setAdjacent(int i, MapTile *adj)
{
    if(i==1) northTile = adj;
    if(i==2) eastTile = adj;
    if(i==3) southTile = adj;
    if(i==4) westTile = adj;
}

MapTile* MapTile::getAdjacent(int i)
{
    if(i==1) return northTile;
    if(i==2) return eastTile;
    if(i==3) return southTile;
    if(i==4) return westTile;
    return NULL;
}

void MapTile::setConnections(vector<int> c)
{
    conns = c;
}

vector<int> MapTile::getConnections()
{
    random_shuffle(conns.begin(),conns.end());
    return conns;
}
