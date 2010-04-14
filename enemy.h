#ifndef _ENEMY_H 
#define _ENEMY_H

#include <OGRE/Ogre.h>
#include <string>
#include <iostream>
#include <math.h>

#include "IDrawable.h"
#include "replicaObject.h"

#include "sceneNodeManager.h"
#include "const.h"

#include "IBulletOwner.h"
#include "IBulletTarget.h"

using namespace Ogre;
using namespace std;

class Enemy : public IDrawable, public ReplicaObject,
              public IBulletOwner, public IBulletTarget
{
    private:
        Vector3 *position;
        
        SceneManager *sceneMgr;
        int id;
        
    public:

        EntityType getEntityType();

    	float health;
    	bool fire;
    	float yawScatter;
    	float pitchScatter;
        bool isReplicated;
        bool isDead;        // Used so enemy deaths can be synched properly
        bool hasExploded;
        int ticksSinceDeath;
    	int fireDelay;
    	
    	Real roll;
        Real pitch;
        Real yaw;
    
        SceneNode *node;
        Enemy(int health, int id);
        Enemy();

        Vector3 getDirection();

        Vector3 *getPosition();
        Vector3 *getOrientation();
        Vector3 *getMeshOrientation();

        std::string getMeshName();
        IDrawable *getParentObject();

        void setPosition(Vector3 v);
        void setOrientation(Real roll, Real pitch, Real yaw);

        Vector3 getBulletOrigin();
        Vector3 getBulletDirection();
        ColourValue getBulletColour();

        float getHealth();
        void damage(float amount);

        ~Enemy();

        virtual RakNet::RakString GetName(void) const;
        virtual RM3SerializationResult Serialize(SerializeParameters *serializeParameters);
        virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters);
};

#endif
