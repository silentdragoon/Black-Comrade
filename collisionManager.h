#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

// Includes all Ogre classes
#include <OGRE/Ogre.h>
#include <Newton.h>

#include <stdint.h>
#include <vector>
#include <dirent.h>
#include <errno.h>
#include <iostream>

#include "collisionDetection.h"
#include "mapManager2.h"
#include "collision.h"
#include "loadingScreen.h"
#include "objective.h"

using namespace Ogre;
using namespace std;

class CollisionManager{
private:
	std::vector<Entity*> movableObj;
	CollisionDetection *cd;
	MapManager *mp;
	Objective *objective;

	bool enemyAdded;

	SceneManager* sceneMgr;
	LoadingScreen *loadingScreen;

	SceneNodeManager *sceneNodeMgr;

	//NewtonCollision *objCollision;

	void deleteAllColMeshes();

public:

	//made for the doors. Gets added to the map of the ray casting as well
	//ie. can be checked with getRCMapAndMovObjsDist and rayCollideWithTransform
	void addColidableMovableObject( Entity *e );

	CollisionManager( SceneManager* sceneMgr, MapManager* mp, Objective *objective, LoadingScreen *loadingScreen, bool rebuildCollisionMeshes, SceneNodeManager *sceneNodeMgr  );
	//Collision isCollided(Vector3 *shipPos);

	//does was it sais ( returns results similar to if the obj isnt present)
	void removeObjective();

	//stanard way to check map dist. Checks all surrounding pieces.
	dFloat getRCMapDist( Vector3 *pos, Vector3 *direction );

	//same as above but includes movable objects
	dFloat getRCMapAndMovObjsDist( Vector3 *pos, Vector3 *direction );

	//checks a/one map piece up to a specified dist. If there hsnt been a intersection 1.2*dist is returned.
	dFloat getRCDirDist(Vector3 *pos, Vector3 *direction, dFloat dist, Entity* e);

	//will add a convex hull for that entity. Will be about the origin, so any ray Cast option to it will need to use the transformed version.
	void addMesh( Entity* e);

	//adds an enemy mesh. Will only be checked by getRCEnemyDist(..)
	void addEnemyMesh( Entity *e );

	//will add ship mesh to map (removed: and keep a special poitner to it)
	void addShipMesh( Entity* e );

	//addds the an objective collision primitive that is only tested with
	//getRCObjDist and obj collision
	void addObjMesh( Real x, Real y, Real z, Real radius);

	//method for getting the dist to objective
	dFloat getRCObjDist( Vector3 *pos, Vector3 *direction );

	//raycasting against enemies
	dFloat getRCEnemyDist( Vector3 *start, Vector3 *direction, Entity *e );

	//needs to be used for dynamic pieces
	dFloat rayCollideWithTransform( Vector3 *start, Vector3 *direction, Entity* entity);

	//function for checking if a dynamic entitiy has collided with the map piece its in (and all objs)
	Collision collideWithMapPieceAndMovableObjects( Entity *e);

	dFloat getRCMovableObjectsDist( Vector3 *pos, Vector3 *direction );

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
