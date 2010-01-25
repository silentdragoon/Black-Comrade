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
    Collision shipMapCollsision(Vector3 *shipPos);
};


#endif
