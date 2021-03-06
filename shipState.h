#ifndef SHIP_STATE_H
#define SHIP_STATE_H

// Includes all Ogre classes
#include <OGRE/Ogre.h>

#include "ITickable.h"
#include "IDrawable.h"
#include "const.h"
#include "damageState.h"

// include Raknet classes
#include "replicaObject.h"
#include "ReplicaManager3.h"
#include <math.h>
#include <string>

#include "IBulletTarget.h"

using namespace Ogre;
using namespace RakNet;

class ShipState : public ITickable, public IDrawable, public ReplicaObject,
                  public IBulletTarget {
private:
        Vector3 *position;
public:
   

    Real roll;
    Real pitch;
    Real yaw;
    double speed;

    ShipState();
    DamageState *damageState;

    void print();
    void tick();

    void setX(double newX);
    void setY(double newY);
    void setZ(double newZ);
    void setSpeed(double s);
    
    Vector3 *getPosition();
    Vector3 *getOrientation();
    Vector3 *getMeshOrientation();
    
    IDrawable *getParentObject();
    std::string getMeshName();

    void setDamageState(DamageState *damageState);
    void damage(float amount);

    EntityType getEntityType();
    EffectType getHitEffectType();

    double getX();
    double getY();
    double getZ();
    double getSpeed();

    virtual RakNet::RakString GetName(void) const;
    virtual RM3SerializationResult Serialize(SerializeParameters *serializeParameters);
    virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters);
};

#endif
