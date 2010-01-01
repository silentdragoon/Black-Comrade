#ifndef LOBBY_H
#define LOBBY_H

// raknet required includes
#include "StringTable.h"
#include "RakPeerInterface.h"
#include "RakNetworkFactory.h"
#include <stdio.h>
#include "BitStream.h"
#include "MessageIdentifiers.h"
#include "ReplicaManager3.h"
#include "NetworkIDManager.h"
#include "RakSleep.h"
#include "FormatString.h"
#include "RakString.h"
#include "GetTime.h"
#include "SocketLayer.h"

#include "DiscoveryAgent.h"
#include "networkRole.h"
#include "gameRole.h"

#include <iostream>
#include <string>
#include <cstdio>

const unsigned char START_GAME = 100;
const unsigned char GAME_ROLE_CHOICES = 101;
const unsigned char GAME_ROLE_CHOICE = 102;

using namespace std;
using namespace RakNet;

class Lobby {
    private:
    RakPeerInterface *rakPeer;
    Packet *packet;
    DiscoveryAgent *discoveryAgent;
    GameRole gameRole;
    NetworkRole networkRole;
    void checkForRoleChoice();

    void process();

    void sendGameRoleChoice(GameRole chosenRole);
    void offerGameRoleChoices();
    void sendGameRoleChoices(SystemAddress recipient);
    void sendGameRoleChoices();

    bool pilotTaken;
    bool navTaken;
    bool engTaken;

    string nick;

    public:
    Lobby(RakPeerInterface *rp, DiscoveryAgent *da, NetworkRole nr);
    void enter();
    bool wait();
    void connect(string address, int port);
    GameRole getChosenGameRole();
    string getChosenNick();
};

#endif