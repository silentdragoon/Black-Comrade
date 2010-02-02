
#ifndef FRONT_GUN_STATE_H
#define FRONT_GUN_STATE_H

#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>
#include <OGRE/Ogre.h>
#include <iostream>
#include "ITickable.h"
#include "pilotControls.h"

// include Raknet classes
#include "replicaObject.h"
#include "ReplicaManager3.h"

using namespace RakNet;

class GunState : public ITickable, public ReplicaObject
{
    private:
        int timeSinceLastFire;
        bool isFire;
        
        PilotControls *pilotControls;
        
    public:
        bool fire();
        
        virtual void tick();
        
        GunState();
        GunState(PilotControls *pilotControls);
        ~GunState();

        virtual RakNet::RakString GetName(void) const;
        virtual RM3SerializationResult Serialize(SerializeParameters *serializeParameters);
        virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters);
};

#endif
