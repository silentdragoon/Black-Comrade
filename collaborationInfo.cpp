#include "collaborationInfo.h"

using namespace std;

CollaborationInfo::CollaborationInfo(string nick, NetworkRole networkRole, GameRole gameRole) :
    nick(nick),
    networkRole(networkRole),
    gameRole(gameRole),
    toRepair(SS_NONE),
    repairAmount(0),
    killCount(0)
{
}

CollaborationInfo::CollaborationInfo()
    : networkRole(NO_NETWORK_ROLE)
    , gameRole(NO_GAME_ROLE)
    , nick("")
    , toRepair(SS_NONE)
    , repairAmount(0)
    , killCount(0)
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
    serializeParameters->outputBitstream[0].Write(toRepair);
    serializeParameters->outputBitstream[0].Write(repairAmount);
    serializeParameters->outputBitstream[0].Write(killCount);
    return RM3SR_BROADCAST_IDENTICALLY;
}

void CollaborationInfo::Deserialize(RakNet::DeserializeParameters *deserializeParameters) {
    deserializeParameters->serializationBitstream[0].Read(toRepair);
    deserializeParameters->serializationBitstream[0].Read(repairAmount);
    deserializeParameters->serializationBitstream[0].Read(killCount);
}
