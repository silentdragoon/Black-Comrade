#ifndef MAP_PIECE_CHOICES_H
#define MAP_PIECE_CHOICES_H

#include <vector>
// include Raknet classes
#include "replicaObject.h"
#include "ReplicaManager3.h"

class MapPieceChoices : public ReplicaObject
{

private:
    std::vector<int> choices;
    int choiceIndex;

public:
    MapPieceChoices();
    MapPieceChoices(std::vector<int> choices);

    int getNextChoice();
 
    virtual RakNet::RakString GetName(void) const;
    virtual RM3SerializationResult Serialize(SerializeParameters *serializeParameters);
    virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters);
    virtual void SerializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *destinationConnection);
    virtual bool DeserializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *sourceConnection);
};

#endif
