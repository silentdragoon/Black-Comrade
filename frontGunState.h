
#ifndef FRONT_GUN_STATE_H
#define FRONT_GUN_STATE_H

#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>
#include <OGRE/Ogre.h>
#include <iostream>
#include "ITickable.h"
#include "shipControls.h"

// include Raknet classes
#include "ReplicaObject.h"
#include "ReplicaManager3.h"

#define MIN_SHOOT_PERIOD 25

using namespace RakNet;

class FrontGunState : public ITickable, public ReplicaObject
{
    private:
        int timeSinceLastFire;
        bool isFire;
        
        ShipControls *shipControls;
        
    public:
        bool fire();
        
        virtual void tick();
        
        FrontGunState();
        FrontGunState(ShipControls *shipControls);
        ~FrontGunState();

        virtual RakNet::RakString GetName(void) const;
        virtual RM3SerializationResult Serialize(SerializeParameters *serializeParameters);
        virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters);
};

#endif
