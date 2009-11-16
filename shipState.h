
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

    SceneNode *shipSceneNode;
    IMotionState *motionState;
public:
    ShipState(SceneNode *shipSceneNode, IMotionState *motionState);
    ShipState();

    void print();
    void tick();
    void updateOgre();

    virtual RakNet::RakString GetName(void) const;
    virtual void WriteAllocationID(RakNet::BitStream *allocationIdBitstream) const;
    virtual RM3SerializationResult Serialize(SerializeParameters *serializeParameters);
    virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters);
};

#endif
