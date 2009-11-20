#include "StringTable.h"
#include "RakPeerInterface.h"
#include "RakNetworkFactory.h"
#include <stdio.h>
#include "Kbhit.h"
#include <string.h>
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

#include "ReplicaObject.h"
#include "shipState.h"
#include "SystemsState.h"
#include "ReplicaConnection.h"
#include "OurReplicaManager.h"

#include <string>
#include <cstdio>

enum Topology
{
	CLIENT,
	SERVER,
} topology;

using namespace std;

using namespace RakNet;

int main(void)
{
	DiscoveryAgent *discoveryAgent;
       ShipState *shipState;
	SystemsState *systemsState;
	char ch;
	SocketDescriptor sd;
	char ip[128];
       int counter = 0;
	string serverAddress = "";
	static const int SERVER_PORT=6001;

	NetworkIDManager networkIdManager;
	RakPeerInterface *rakPeer;
	OurReplicaManager replicaManager;

	discoveryAgent = new DiscoveryAgent();

	printf("Start as (c)lient, (s)erver?\n");
	ch=getch();

	if (ch=='c' || ch=='C')
	{
		serverAddress = discoveryAgent->findServer(6001,6000,5);
		if (serverAddress.compare("") == 0) return 0;
		topology=CLIENT;
		sd.port=0;
	}
	else if (ch=='s' || ch=='S')
	{
		topology=SERVER;
		sd.port=SERVER_PORT;
		discoveryAgent->beServer(6001,20);
	}
       
	rakPeer = RakNetworkFactory::GetRakPeerInterface();

	rakPeer->SetNetworkIDManager(&networkIdManager);
	networkIdManager.SetIsNetworkIDAuthority(topology==SERVER);
	rakPeer->Startup(3,100,&sd,1);
	rakPeer->AttachPlugin(&replicaManager);
	rakPeer->SetMaximumIncomingConnections(3);

	printf("\n");
	if (topology==CLIENT)
	{
		rakPeer->Connect(serverAddress.c_str(),SERVER_PORT,0,0,0);
		printf("Connecting...\n");
	}

	printf("Commands:\n(Q)uit\n'C'reate objects\n'D'estroy my objects\n");

	// Enter infinite loop to run the system
	Packet *packet;
	bool quit=false;
       bool amPilot=false;
	bool amEngineer=false;
	while (!quit)
	{
		for (packet = rakPeer->Receive(); packet; rakPeer->DeallocatePacket(packet), packet = rakPeer->Receive())
		{
			switch (packet->data[0])
			{
			case ID_CONNECTION_ATTEMPT_FAILED:
				printf("ID_CONNECTION_ATTEMPT_FAILED\n");
				quit=true;
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("ID_NO_FREE_INCOMING_CONNECTIONS\n");
				quit=true;
				break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
				printf("ID_CONNECTION_REQUEST_ACCEPTED\n");
				break;
			case ID_NEW_INCOMING_CONNECTION:
				printf("ID_NEW_INCOMING_CONNECTION from %s\n", packet->systemAddress.ToString());
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				printf("ID_DISCONNECTION_NOTIFICATION\n");
				break;
			case ID_CONNECTION_LOST:
				printf("ID_CONNECTION_LOST\n");
				break;
			case ID_ADVERTISE_SYSTEM:
				rakPeer->Connect(packet->systemAddress.ToString(false), packet->systemAddress.port,0,0);
				break;
			}
		}

		if (kbhit())
		{
			ch=getch();
			if (ch=='q' || ch=='Q')
			{
				printf("Quitting.\n");
				quit=true;
			}
			if (ch=='p' || ch=='P' && amPilot == false && amEngineer == false)
			{
                            shipState = new ShipState();
				replicaManager.Reference(shipState);
				shipState->print();
				amPilot = true;
			}
			if(ch=='e' && amPilot == false && amEngineer == false)
			{
				shipState = (ShipState*) replicaManager.GetReplicaAtIndex(0);
                            systemsState = new SystemsState();
				replicaManager.Reference(systemsState);
				systemsState->Print();
				amEngineer = true;
			}
			if(ch=='x' && amPilot == true)
			{
				shipState->setX(shipState->getX() + 1);
				shipState->print();
			}
			if(ch=='y' && amPilot == true)
			{
				shipState->setY(shipState->getY() + 1);
				shipState->print();
			}
			if(ch=='z' && amPilot == true)
			{
				shipState->setZ(shipState->getZ() + 1);
				shipState->print();
			}
			if(ch=='h' && amEngineer == true)
			{
				systemsState->increaseRearShieldPower();
				systemsState->Print();
			}
			if(ch=='j' && amEngineer == true)
			{
				systemsState->increaseFrontShieldPower();
				systemsState->Print();
			}
			if(ch=='k' && amEngineer == true)
			{
				systemsState->increaseEnginePower();
				systemsState->Print();
			}
			if(ch=='l' && amEngineer == true)
			{
				systemsState->increaseWeaponsPower();
				systemsState->Print();
			}
			if (ch=='d' || ch=='D')
			{
				printf("My objects destroyed.\n");
				DataStructures::Multilist<ML_STACK, Replica3*> replicaListOut;
				replicaManager.GetReplicasCreatedByMe(replicaListOut);
				replicaManager.BroadcastDestructionList(replicaListOut, UNASSIGNED_SYSTEM_ADDRESS);
				replicaListOut.ClearPointers( true, __FILE__, __LINE__ );
			}

		}

		RakSleep(30);
		for (int i=0; i < 32; i++)
		{
			if (rakPeer->GetInternalID(UNASSIGNED_SYSTEM_ADDRESS,0).port!=SERVER_PORT+i)
				rakPeer->AdvertiseSystem("255.255.255.255", SERVER_PORT+i, 0,0,0);
		}
	}

	rakPeer->Shutdown(100,0);
	RakNetworkFactory::DestroyRakPeerInterface(rakPeer);
}
