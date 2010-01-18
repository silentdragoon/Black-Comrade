
#ifndef COLLISION_DETECTION_H
#define COLLISION_DETECTION_H

// Includes all Ogre classes 
#include <OGRE/Ogre.h>
#include <Newton.h>

#include <stdint.h>

using namespace Ogre;
using namespace std;

class CollisionDetection{
private:

   	NewtonWorld *newtonWorld;

	dFloat idmatrix[16];

	map<Entity *,NewtonCollision *> collisionsMap;
	map<Entity *,NewtonBody *> bodysMap;

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

    //static dFloat RayCastFilter(const NewtonBody* body, const dFloat* normal, int collisionID, void* userData, dFloat intersetParam);
    
public:
    
    CollisionDetection();
    
    void addTreeCollisionMesh(Entity *entity);
    
    bool isCollision(Entity *e1, Entity *e2);
};

//static unsigned RayCastPrefilter (const NewtonBody* body,  const NewtonCollision* collision, void* userData);

#endif
