#include "collaborationInfo.h"

using namespace std;

CollaborationInfo::CollaborationInfo(string nick, NetworkRole networkRole, GameRole gameRole) :
    nick(nick),
    networkRole(networkRole),
    gameRole(gameRole)
{
}

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
