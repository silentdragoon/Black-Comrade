
#ifndef SHIP_STATE_H
#define SHIP_STATE_H

// Includes all Ogre classes
#include <OGRE/Ogre.h>

#include "IMotionState.h"
#include "ITickable.h"

// include Raknet classes
#include "ReplicaObject.h"
#include "ReplicaManager3.h"

using namespace Ogre;
using namespace RakNet;

class ShipState : public ITickable, public ReplicaObject {
private:
    Vector3 *position;
    Real roll;
    Real pitch;
    Real yaw;

    IMotionState *motionState;
public:
    ShipState(SceneNode *shipSceneNode, IMotionState *motionState);
    ShipState();
    SceneNode *shipSceneNode;

    void print();
    void tick();
    void updateOgre();

    void setX(double newX);
    void setY(double newY);
    void setZ(double newZ);

    double getX();
    double getY();
    double getZ();

    virtual RakNet::RakString GetName(void) const;
    virtual RM3SerializationResult Serialize(SerializeParameters *serializeParameters);
    virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters);
};

#endif
