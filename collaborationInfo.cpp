#include "collaborationInfo.h"
#include <iostream>

using namespace std;

CollaborationInfo::CollaborationInfo(string nick, NetworkRole networkRole, GameRole gameRole) :
    nick(nick),
    networkRole(networkRole),
    gameRole(gameRole),
    toRepair(SS_NONE),
    isReady(false),
    repairAmount(0),
    killCount(0),
    stats(0),
    hasQuit(false),
    repairing(false),
    hasCompletedTutorial(false)
{
}

CollaborationInfo::CollaborationInfo()
    : networkRole(NO_NETWORK_ROLE)
    , gameRole(NO_GAME_ROLE)
    , nick("")
    , toRepair(SS_NONE)
    , repairAmount(0)
    , killCount(0)
    , stats(0)
    , isReady(false)
    , hasQuit(false)
    , repairing(false)
    , hasCompletedTutorial(false)
{}

string CollaborationInfo::getNick() { return nick; }

NetworkRole CollaborationInfo::getNetworkRole() { return networkRole; }

GameRole CollaborationInfo::getGameRole() { return gameRole; }

string CollaborationInfo::getGameRoleString() {
    if (gameRole == PILOT) return "Pilot";
    else if (gameRole == NAVIGATOR) return "Navigator";
    else if (gameRole == ENGINEER) return "Engineer";
    else return "";
}

string CollaborationInfo::getNetworkRoleString() {
    if (networkRole == SERVER) return "Server";
    else if (networkRole == CLIENT) return "Client";
    else return "";
}

PlayerStats * CollaborationInfo::getPlayerStats() { return stats; }

void CollaborationInfo::setPlayerStats(PlayerStats *mStats) {
    stats = mStats;
}

void CollaborationInfo::SerializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *destinationConnection) {
    constructionBitstream->Write(gameRole);
    constructionBitstream->Write(RakNet::RakString(nick.c_str()));
}

bool CollaborationInfo::DeserializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *sourceConnection) {
    constructionBitstream->Read(gameRole);
    RakNet::RakString raknick;
    constructionBitstream->Read(raknick);
    nick = raknick.C_String();
    return true;
}

RakNet::RakString CollaborationInfo::GetName(void) const {
    if (gameRole == PILOT) return RakNet::RakString("PilotInfo");
    if (gameRole == NAVIGATOR) return RakNet::RakString("NavigatorInfo");
    if (gameRole == ENGINEER) return RakNet::RakString("EngineerInfo");

    return RakNet::RakString("");
}

RM3SerializationResult CollaborationInfo::Serialize(SerializeParameters *serializeParameters) {
    serializeParameters->outputBitstream[0].Write(isReady);
    serializeParameters->outputBitstream[0].Write(hasCompletedTutorial);
    serializeParameters->outputBitstream[0].Write(toRepair);
    serializeParameters->outputBitstream[0].Write(repairAmount);
    serializeParameters->outputBitstream[0].Write(hasQuit);
    serializeParameters->outputBitstream[0].Write(repairing);

    if (repairAmount !=0) {
        toRepair = SS_NONE;
        repairAmount = 0;
        return RM3SR_SERIALIZED_ALWAYS_IDENTICALLY;
    } else {
        return RM3SR_BROADCAST_IDENTICALLY;
    }
}

void CollaborationInfo::Deserialize(RakNet::DeserializeParameters *deserializeParameters) {
    ShipSystem toRepair2 = SS_NONE;
    int repairAmount2 = 0;

    deserializeParameters->serializationBitstream[0].Read(isReady);
    deserializeParameters->serializationBitstream[0].Read(hasCompletedTutorial);
    deserializeParameters->serializationBitstream[0].Read(toRepair2);
    if (toRepair2 != SS_NONE) toRepair = toRepair2;
    deserializeParameters->serializationBitstream[0].Read(repairAmount2);
    if (repairAmount2 != 0) repairAmount = repairAmount2;
    deserializeParameters->serializationBitstream[0].Read(hasQuit);
    deserializeParameters->serializationBitstream[0].Read(repairing);

}
