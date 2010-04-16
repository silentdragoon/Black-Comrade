#ifndef COLLABORATIONINFO_H
#define COLLABORATIONINFO_H

#include "gameRole.h"
#include "networkRole.h"
#include "replicaObject.h"
#include "shipSystem.h"
#include "playerStats.h"

#include <string>

using namespace std;

class CollaborationInfo : public ReplicaObject
{
    private:
    string nick;
    GameRole gameRole;
    NetworkRole networkRole;
    PlayerStats *stats;

    int killCount;

    public:
    CollaborationInfo(string nick, NetworkRole networkRole, GameRole gameRole);
    CollaborationInfo();

    void setPlayerStats(PlayerStats* stats);
    PlayerStats * getPlayerStats();

    ShipSystem toRepair;
    int repairAmount;
    bool hasQuit;
    bool hasCompletedTutorial;
    bool repairing;

    string getNick();
    GameRole getGameRole();
    string getGameRoleString();
    NetworkRole getNetworkRole();
    string getNetworkRoleString();

    virtual RakNet::RakString GetName(void) const;
    virtual RM3SerializationResult Serialize(SerializeParameters *serializeParameters);
    virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters);
    virtual void SerializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *destinationConnection);
    virtual bool DeserializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *sourceConnection);
};

#endif

