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
    
    //stanard way to check map dist. Checks all surrounding pieces.
    dFloat getRCMapDist( Vector3 *pos, Vector3 *direction );

    //checks a map piece up to a specified dist. If there hsnt been a intersection 1.2*dist is returned.
    dFloat getRCDirDist(Vector3 *pos, Vector3 *direction, dFloat dist, Entity* e);

    //will add a convex hull for that entity. Will be about the origin, so any ray Cast option to it will need to use the transformed version.
    void addMesh( Entity* e);
    
    //needs to be used for dynamic pieces
    dFloat rayCollideWithTransform( Vector3 *start, Vector3 *direction, Entity* entity);
    
    //working for now. may need to be looked at later
    Collision shipMapCollision(Vector3 *shipPos);

    //working stuff thats unneccesary for now
    //dFloat getRCDistBetweenPoints( Vector3 *start, Vector3 * end, Entity* collideAgainst );
    // dFloat getRCDistAlongRay( Vector3 *start, Real Pitch, Real Yaw, dFloat maxDist, Entity* collideAgainst );
    //dFloat getRCMapDist( Vector3 *pos, Real pitch, Real yaw  );
    
    //depreciated stuff. Here for me to refrence
    // Vector3* getRCVector( Vector3 *start, Real pitch, Real yaw, Entity* collideAgainst );
    
};


#endif
