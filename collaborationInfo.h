#ifndef COLLABORATIONINFO_H
#define COLLABORATIONINFO_H

#include "gameRole.h"
#include "networkRole.h"
#include <string>

using namespace std;

class CollaborationInfo
{

    private:
    string nick;
    GameRole gameRole;
    NetworkRole networkRole;

    public:
    CollaborationInfo(string nick, NetworkRole networkRole, GameRole gameRole);
    string getNick();
    GameRole getGameRole();
    string getGameRoleString();
    NetworkRole getNetworkRole();
};

#endif

