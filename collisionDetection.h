
#ifndef COLLISION_DETECTION_H
#define COLLISION_DETECTION_H

// Includes all Ogre classes 
#include <OGRE/Ogre.h>
#include <Newton.h>

#include <stdint.h>
#include <vector>
#include "collision.h"


using namespace Ogre;
using namespace std;

class CollisionDetection{
private:

   	NewtonWorld *newtonWorld;
    
    int shapeID;

	dFloat idmatrix[16];

    std::map<Entity *,NewtonCollision *> collisionsMap;
    std::map<Entity *,NewtonBody *> bodysMap;
    Entity *enemy;
    
    NewtonCollision *enemyCol;

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

	void getMatrix(Entity *entity, dFloat *matrix);
    
public:
    
    CollisionDetection();
    
    void addStaticTreeCollisionMesh(Entity *entity);
    
    Collision isCollision(Entity *e1, Entity *e2);
    
    dFloat rayCollideDist( Vector3 *start, Vector3 *end, Entity* collideAgainst );
    dFloat rayCollideWithTransform( Vector3 *start, Vector3 *end, Entity* collideAgainst );
    
    //depreciated. Here for refrence
    //dFloat rayCollideWithEnemy( Vector3 *start, Vector3 *end, Entity* collideAgainst );
     void createShipMesh( Entity* e );
    // void createEnemyMesh( Entity* e );
    
    void createConvexHull( Entity *entity );
    
};

//static unsigned RayCastPrefilter (const NewtonBody* body,  const NewtonCollision* collision, void* userData);

#endif
