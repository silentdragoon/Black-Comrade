
#ifndef _GUN_STATE_H
#define _GUN_STATE_H

#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>
#include <OGRE/Ogre.h>
#include <iostream>
#include "ITickable.h"
#include "pilotControls.h"
#include "navigatorControls.h"
#include "engineerControls.h"

// include Raknet classes
#include "replicaObject.h"
#include "ReplicaManager3.h"

using namespace RakNet;

class GunState : public ITickable, public ReplicaObject
{
    private:
        int pilotTimeSinceLastFire;
        int navTimeSinceLastFire;
        int engTimeSinceLastFire;
        bool isPilotFire;
        bool isNavFire;
        bool isEngFire;
        
        PilotControls *pilotControls;
        NavigatorControls *navControls;
        EngineerControls *engControls;
        
    public:
        bool pilotFire();
        bool navFire();
        bool engFire();
        
        virtual void tick();
        
        GunState();
        GunState(PilotControls *pilotControls);
        GunState(NavigatorControls *navControls);
        GunState(EngineerControls *engControls);
        ~GunState();

        virtual RakNet::RakString GetName(void) const;
        virtual RM3SerializationResult Serialize(SerializeParameters *serializeParameters);
        virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters);
};

#endif
