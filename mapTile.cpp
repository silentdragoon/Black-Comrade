#include "mapTile.h"

MapTile::MapTile(SceneNode *node, Entity *e, int x, int y) :
    node(node),
    e(e),
    x(x),
    y(y),
    eastConn(false),
    southConn(false)
{
    empty = false;
    waypoint = false;

    northTile = NULL;
    eastTile = NULL;
    southTile = NULL;
    westTile = NULL;
}

MapTile::MapTile() :
    eastConn(false),
    southConn(false)
{
    empty = true;
    waypoint = false;
    northTile = NULL;
    eastTile = NULL;
    southTile = NULL;
    westTile = NULL;
    x = 0;
    y = 0;
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

void MapTile::assignSpawnPoints(std::vector<Vector3*> sp)
{
    spawnPoints = sp;
}

std::vector<Vector3*> MapTile::getSpawnPoints()
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
    if(i==2){eastTile = adj; eastConn = true;}
    if(i==3){southTile = adj; southConn = true;}
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

int MapTile::getX() { return x; }

int MapTile::getY() { return y; }

void MapTile::setConnections(std::vector<int> c)
{
    conns = c;
}

std::vector<int> MapTile::getConnections()
{
    random_shuffle(conns.begin(),conns.end());
    return conns;
}

bool MapTile::eastConnected()
{
    return eastConn;
}
bool MapTile::southConnected()
{
    return southConn;
}

