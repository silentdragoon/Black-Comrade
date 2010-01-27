#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

// Includes all Ogre classes 
#include <OGRE/Ogre.h>
#include <Newton.h>

#include <stdint.h>
#include <vector>

#include "collisionDetection.h"
#include "mapManager.h"
#include "collision.h"

using namespace Ogre;
using namespace std;

class CollisionManager{
private:

    CollisionDetection *cd;
    MapManager *mp;
    SceneManager* sceneMgr;
    
public:

    CollisionManager( SceneManager* sceneMgr, MapManager* mp );
    //Collision isCollided(Vector3 *shipPos);
    Collision shipMapCollision(Vector3 *shipPos);
    dFloat getRCDistBetweenPoints( Vector3 *start, Vector3 * end, Entity* collideAgainst );
    
    dFloat getRCDistAlongRay( Vector3 *start, Real Pitch, Real Yaw, dFloat maxDist, Entity* collideAgainst );
    dFloat getRCDirDist(Vector3 *pos, Vector3 *direction, dFloat dist, Entity* e);


    dFloat getRCMapDist( Vector3 *pos, Real pitch, Real yaw  );
    dFloat getRCMapDist( Vector3 *pos, Vector3 *direction );
    
    Vector3* getRCVector( Vector3 *start, Real pitch, Real yaw, Entity* collideAgainst );
    
    dFloat getRCAgainstShip ( Vector3 *start, Real pitch, Real yaw );
    
    void addEnemy( Entity e);
    dFloat rayCollideWithEnemy( Vector3 *start, Vector3 *end, Entity* collideAgainst);
};


#endif
