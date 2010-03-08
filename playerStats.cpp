#include "playerStats.h"

PlayerStats::PlayerStats()
    : owner(NO_GAME_ROLE)
{}

PlayerStats::PlayerStats(GameRole owner)
    : owner(owner)
{}

void PlayerStats::SerializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *destinationConnection) {
    constructionBitstream->Write(owner);
}

bool PlayerStats::DeserializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *sourceConnection) {
    constructionBitstream->Read(owner);
    return true;
}

RakNet::RakString PlayerStats::GetName(void) const {
    if (owner == PILOT) return RakNet::RakString("PilotStats");
    if (owner == NAVIGATOR) return RakNet::RakString("NavigatorStats");
    if (owner == ENGINEER) return RakNet::RakString("EngineerStats");

    return RakNet::RakString("");
}

RM3SerializationResult PlayerStats::Serialize(SerializeParameters *serializeParameters) {
    serializeParameters->outputBitstream[0].Write(shotsFired);
    serializeParameters->outputBitstream[0].Write(shotsHit);
    serializeParameters->outputBitstream[0].Write(enemiesDestroyed);
    serializeParameters->outputBitstream[0].Write(collisions);
    serializeParameters->outputBitstream[0].Write(averageSpeed);

    return RM3SR_BROADCAST_IDENTICALLY;
}

void PlayerStats::Deserialize(RakNet::DeserializeParameters *deserializeParameters) {
    deserializeParameters->serializationBitstream[0].Read(shotsFired);
    deserializeParameters->serializationBitstream[0].Read(shotsHit);
    deserializeParameters->serializationBitstream[0].Read(enemiesDestroyed);
    deserializeParameters->serializationBitstream[0].Read(collisions);
    deserializeParameters->serializationBitstream[0].Read(averageSpeed);
}

void PlayerStats::print() {
    if (owner == PILOT) {
        std::cout << "Collisions: " << collisions << "\n";
        std::cout << "Avg. speed: " << averageSpeed << "\n";
    }
    std::cout << "Shots fired: " << shotsFired << "\n";
    std::cout << "..on target: " << shotsHit << "\n";
    std::cout << "Enemies destroyed: " << enemiesDestroyed << "\n";
}
