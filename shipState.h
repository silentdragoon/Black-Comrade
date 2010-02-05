
#ifndef SHIP_STATE_H
#define SHIP_STATE_H

// Includes all Ogre classes
#include <OGRE/Ogre.h>

// #include "flying.h"
#include "ITickable.h"
#include "IDrawable.h"

// include Raknet classes
#include "replicaObject.h"
#include "ReplicaManager3.h"
#include <math.h>
#include <string>

using namespace Ogre;
using namespace RakNet;

class ShipState : public ITickable, public IDrawable, public ReplicaObject {
private:
        Vector3 *position;
public:
   

    Real roll;
    Real pitch;
    Real yaw;

    ShipState(SceneNode *shipSceneNode);

    ShipState();
    SceneNode *shipSceneNode;

    void print();
    void tick();
    void updateOgre();

    void setX(double newX);
    void setY(double newY);
    void setZ(double newZ);
    
    Vector3 *getPosition();
    Vector3 *getOrientation();
    IDrawable *getParentObject();
    std::string getMeshName();

    double getX();
    double getY();
    double getZ();

    virtual RakNet::RakString GetName(void) const;
    virtual RM3SerializationResult Serialize(SerializeParameters *serializeParameters);
    virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters);
};

#endif
