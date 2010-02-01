#include "ourReplicaManager.h"

#include "ReplicaManager3.h"
#include "replicaConnection.h"
#include "GetTime.h"
#include "MessageIdentifiers.h"
#include "RakPeerInterface.h"
#include "NetworkIDManager.h"

using namespace RakNet;

Connection_RM3* OurReplicaManager::AllocConnection(SystemAddress systemAddress, RakNetGUID rakNetGUID) const {
    return new ReplicaConnection(systemAddress,rakNetGUID);
}

void OurReplicaManager::DeallocConnection(Connection_RM3 *connection) const {
    delete connection;
}

void OurReplicaManager::doUpdate() {
    DataStructures::DefaultIndexType index,index2;

    for (index=0; index < userReplicaList.GetSize(); index++)
    {
        userReplicaList[index]->forceSendUntilNextUpdate=false;
    }

    RakNetTime time = RakNet::GetTime();

    SerializeParameters sp;
    sp.curTime=time;
    ReplicaConnection *connection;
    SendSerializeIfChangedResult ssicr;
    sp.messageTimestamp=0;
    sp.pro=defaultSendParameters;
    for (index=0; index < connectionList.GetSize(); index++)
    {
        connection = (ReplicaConnection*) connectionList[index];
        sp.bitsWrittenSoFar=0;
        index2=0;
        while (index2 < connection->getList().GetSize())
        {
            sp.destinationConnection=connection;
            sp.whenLastSerialized=connection->getList()[index2]->whenLastSerialized;
            ssicr=connection->SendSerializeIfChanged(index2, &sp, GetRakPeerInterface(), GetWorldID(), this);
            if (ssicr==SSICR_SENT_DATA)
            {
                connection->getList()[index2]->whenLastSerialized=time;
                index2++;
            }
            else if (ssicr==SSICR_NEVER_SERIALIZE)
            {
                // Removed from the middle of the list
            }
            else
                index2++;
        }
    }
}
