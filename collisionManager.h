#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

// Includes all Ogre classes 
#include <OGRE/Ogre.h>
#include <Newton.h>

#include <stdint.h>
#include <vector>

#include "collisionDetection.h"
#include "mapManager2.h"
#include "collision.h"
#include "loadingScreen.h"

using namespace Ogre;
using namespace std;

class CollisionManager{
private:

    CollisionDetection *cd;
    MapManager *mp;
    SceneManager* sceneMgr;
    LoadingScreen *loadingScreen;
    
    NewtonCollision *objCollision;
    
public:

    CollisionManager( SceneManager* sceneMgr, MapManager* mp, LoadingScreen *loadingScreen, bool rebuildCollisionMeshes );
    //Collision isCollided(Vector3 *shipPos);
    
    //stanard way to check map dist. Checks all surrounding pieces.
    dFloat getRCMapDist( Vector3 *pos, Vector3 *direction );

    //checks a/one map piece up to a specified dist. If there hsnt been a intersection 1.2*dist is returned.
    dFloat getRCDirDist(Vector3 *pos, Vector3 *direction, dFloat dist, Entity* e);

    //will add a convex hull for that entity. Will be about the origin, so any ray Cast option to it will need to use the transformed version.
    void addMesh( Entity* e);
    
    //will add ship mesh to map (removed: and keep a special poitner to it)
    void addShipMesh( Entity* e );
    
    //addds the an objective collision primitive that is only tested with
    //getRCObjDist and obj collision
    void addObjMesh( Real x, Real y, Real z, Real radius);
    
    //method for getting the dist to objective
    dFloat getRCObjDist( Vector3 *pos, Vector3 *direction );
    
    //needs to be used for dynamic pieces
    dFloat rayCollideWithTransform( Vector3 *start, Vector3 *direction, Entity* entity);
    
    //function for checking if a dynamic entitiy has collided with the map piece its in
    Collision collideWithMapPiece( Entity *e);
    
    //check if the ship has collided with the obj
    bool collideEntityWithObj(Entity *e);
    
    
    
    //working for now. may need to be looked at later
    //Collision shipMapCollision(Vector3 *shipPos);
    
    
    

    //working stuff thats unneccesary for now
    //dFloat getRCDistBetweenPoints( Vector3 *start, Vector3 * end, Entity* collideAgainst );
    // dFloat getRCDistAlongRay( Vector3 *start, Real Pitch, Real Yaw, dFloat maxDist, Entity* collideAgainst );
    //dFloat getRCMapDist( Vector3 *pos, Real pitch, Real yaw  );
    
    //depreciated stuff. Here for me to refrence
    // Vector3* getRCVector( Vector3 *start, Real pitch, Real yaw, Entity* collideAgainst );
    
};


#endif
