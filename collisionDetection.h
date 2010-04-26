
#ifndef COLLISION_DETECTION_H
#define COLLISION_DETECTION_H

// Includes all Ogre classes
#include <OGRE/Ogre.h>
#include <Newton.h>

#include <stdint.h>
#include <vector>
#include "collision.h"
#include "constManager.h"

using namespace Ogre;
using namespace std;

class CollisionDetection{
private:

   	NewtonWorld *newtonWorld;

    bool gernerateMeshes;

    int shapeID;

	dFloat idmatrix[16];

    std::map<Entity *,NewtonCollision *> collisionsMap;
    std::map<Entity *,NewtonBody *> bodysMap;
    Entity *enemy;

    NewtonCollision *enemyEnt;
    NewtonCollision *objCollision;

    //dFloat t[9];
    //dFloat shipPos[16];
    //dFloat idmatrix[16];

    void GetMeshInformation(const Ogre::MeshPtr mesh,
                                size_t &vertex_count,
                                Ogre::Vector3* &vertices,
                                size_t &index_count,
                                unsigned long* &indices,
                                const Ogre::Vector3 &position,
                                const Ogre::Quaternion &orient,
                                const Ogre::Vector3 &scale);

	void getMatrix(Entity *entity, dFloat *matrix, bool);

    static void mySerializeCollisionCallbackFunction(void* serializeHandle, const void* buffer, int size);
    static void myDeserializeCollisionCallbackFunction(void* serializeHandle, void* buffer, int size);


public:

    CollisionDetection( bool genMeshes);

    void addStaticTreeCollisionMesh(Entity *entity);

    Collision staicAndDynamicCollision(Entity *e1, Entity *e2, bool dynamic);

    Collision nonStaticCollision(Entity *e1, Entity *e2);

    dFloat rayCollideDist( Vector3 *start, Vector3 *end, Entity* collideAgainst );
    dFloat rayCollideWithTransform( Vector3 *start, Vector3 *end, Entity* collideAgainst );

    //
    bool collideEntityWithObj(Entity *e);
    void createEnemyMesh( Entity* e );
    dFloat rayCollideWithEnemy( Vector3 *start, Vector3 *end, Entity* collideAgainst );
    //depreciated. Here for refrence
    //void createShipMesh( Entity* e );

    void createConvexHull( Entity *entity, bool enemy );
    void createObjPrimitive( Real x, Real y, Real z, Real radius);
    dFloat objRayCollision(  Vector3 *start, Vector3 *end );

};

//static unsigned RayCastPrefilter (const NewtonBody* body,  const NewtonCollision* collision, void* userData);

#endif
