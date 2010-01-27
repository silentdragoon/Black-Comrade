#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

// Includes all Ogre classes 
#include <OGRE/Ogre.h>
#include <Newton.h>

#include <stdint.h>
#include <vector>

#include "collisionDetection.h"
#include "mapCreate.h"
#include "collision.h"

using namespace Ogre;
using namespace std;

class CollisionManager{
private:

    CollisionDetection *cd;
    MapCreate *mp;
    SceneManager* sceneMgr;
    
public:
    CollisionManager( SceneManager* sceneMgr, MapCreate* mp );
    //Collision isCollided(Vector3 *shipPos);
    Collision shipMapCollision(Vector3 *shipPos);
    dFloat getRCDistBetweenPoints( Vector3 *start, Vector3 * end, Entity* collideAgainst );
    
    dFloat getRCDistAlongRay( Vector3 *start, Real Pitch, Real Yaw, dFloat maxDist, Entity* collideAgainst );

    dFloat getRCMapDist( Vector3 *pos, Real pitch, Real yaw  );
    
    Vector3* getRCVector( Vector3 *start, Real pitch, Real yaw, Entity* collideAgainst );
    
    dFloat getRCAgainstShip ( Vector3 *start, Real pitch, Real yaw );

};


#endif
