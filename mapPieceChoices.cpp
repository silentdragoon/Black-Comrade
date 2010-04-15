#include "mapPieceChoices.h"
#include <iostream>

MapPieceChoices::MapPieceChoices(std::vector<int> choices)
    : choices(choices)
    , choiceIndex(0)
{
    //std::cout << "MADE CHOICES:\n";
    //for (int i=0;i<choices.size();i++) {
    //    std::cout << choices.at(i) << "\n";
    //}
}

MapPieceChoices::MapPieceChoices()
    : choices(std::vector<int>())
    , choiceIndex(0)
{}

int MapPieceChoices::getNextChoice() {
    if (choices.size() == 0) {
        return -1;
    } else {
        choiceIndex ++;
        return choices.at(choiceIndex - 1);
    }
}

RakNet::RakString MapPieceChoices::GetName(void) const {
    return RakNet::RakString("MapPieceChoices");
}

void MapPieceChoices::SerializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *destinationConnection) {
    constructionBitstream->Write((int) choices.size());
    for (int i=0;i<choices.size();i++) {
        constructionBitstream->Write(choices.at(i));
        //std::cout << "Writing choice " << choices.at(i) << " (" << i+1 << " of " << choices.size() << ")\n";
    }
}

bool MapPieceChoices::DeserializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *sourceConnection) {
    int length = 0;
    constructionBitstream->Read(length);
    for (int i=0;i<length;i++) {
        int choice = 0;
        constructionBitstream->Read(choice);
        choices.push_back(choice);
        //std::cout << "Reading choice " << choices.at(i) << " (" << i+1 << " of " << length << ")\n";
    }
    return true;
}

RM3SerializationResult MapPieceChoices::Serialize(SerializeParameters *serializeParameters) {
    return RM3SR_DO_NOT_SERIALIZE;
}

void MapPieceChoices::Deserialize(RakNet::DeserializeParameters *deserializeParameters) {}
