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
    Collision isCollided(Vector3 *shipPos);
};


#endif
