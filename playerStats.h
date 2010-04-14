#ifndef PLAYER_STATS_H
#define PLAYER_STATS_H

#include "gameRole.h"
#include "replicaObject.h"
#include <iostream>

class PlayerStats : public ReplicaObject {
private:
    GameRole owner;

public:
    PlayerStats();
    PlayerStats(GameRole owner);

    int shotsFired;
    int shotsHit;
    int enemiesDestroyed;
    int friendlyFire;
    int repairsMade;

    double averageSpeed;
    int numCollisions;


    virtual RakNet::RakString GetName(void) const;
    virtual RM3SerializationResult Serialize(SerializeParameters *serializeParameters);
    virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters);
    virtual void SerializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *destinationConnection);
    virtual bool DeserializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *sourceConnection);

    void print();
};

#endif
