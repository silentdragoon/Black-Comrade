
#ifndef FRONT_GUN_STATE_H
#define FRONT_GUN_STATE_H

#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>
#include <OGRE/Ogre.h>
#include <iostream>
#include "ITickable.h"
#include "IPlayerControls.h"
#include "gameRole.h"

// include Raknet classes
#include "replicaObject.h"
#include "ReplicaManager3.h"

using namespace RakNet;

class GunState : public ITickable, public ReplicaObject
{
    private:
        int timeSinceLastFire;
        bool isFire;
        GameRole owner;
        
        IPlayerControls *playerControls;
        
    public:
        bool fire();
        
        virtual void tick();
        
        GunState();
        GunState(IPlayerControls *pilotControls, GameRole owner);
        ~GunState();

        virtual RakNet::RakString GetName(void) const;
        virtual RM3SerializationResult Serialize(SerializeParameters *serializeParameters);
        virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters);
	virtual void SerializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *destinationConnection);
        virtual bool DeserializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *sourceConnection);
};

#endif
