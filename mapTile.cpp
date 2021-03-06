#include "mapTile.h"

MapTile::MapTile(SceneNode *node, Entity *e, int x, int y) :
    node(node),
    e(e),
    x(x),
    y(y),
    eastConn(false),
    southConn(false),
    eastConnPieceEntity(NULL),
    southConnPieceEntity(NULL)
{
    empty = false;
    waypoint = false;
    obj = false;
    start = false;
    end = false;

    northTile = NULL;
    eastTile = NULL;
    southTile = NULL;
    westTile = NULL;
}

MapTile::MapTile() :
    eastConn(false),
    southConn(false),
    eastConnPieceEntity(NULL),
    southConnPieceEntity(NULL),
    e(NULL)
{
    empty = true;
    waypoint = false;
    obj = false;
    start = false;
    end = false;

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

std::vector<Waypoint*> MapTile::getWaypoints()
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

bool MapTile::isObj()
{
    return obj;
}

bool MapTile::isStart()
{
    return start;
}

bool MapTile::isEnd()
{
    return end;
}

void MapTile::setStart()
{
    start = true;
}

void MapTile::setEnd()
{
    end = true;
}

void MapTile::setObjective() 
{
    obj = true;
}

void MapTile::assignWaypoint(Waypoint *wa)
{
    w.push_back(wa);
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

void MapTile::setEastConnPiece( Entity *ent )
{
    eastConnPieceEntity = ent;
}

void MapTile::setSouthConnPiece( Entity *ent )
{
    southConnPieceEntity = ent;
}

Entity* MapTile::getSouthConnEntity()
{
    return southConnPieceEntity;
}
    
    
Entity* MapTile::getEastConnEntity()
{
    return eastConnPieceEntity;
}


//ents has space for 5 entitiy pointer allocated
void MapTile::getTileAndConnectionEntities( Entity **ents )
{
    ents[0] = e;
    ents[1] = eastConnPieceEntity;
    ents[2] = southConnPieceEntity;
    if( northTile != NULL) ents[3] = northTile->getSouthConnEntity();
    else ents[3] = NULL;
    if( westTile != NULL ) ents[4] = westTile->getEastConnEntity();
    else ents[4] = NULL;
}
    



