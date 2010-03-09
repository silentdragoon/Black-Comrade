#include "playerStats.h"

PlayerStats::PlayerStats()
    : owner(NO_GAME_ROLE)
    , shotsFired(0)
    , shotsHit(0)
    , enemiesDestroyed(0)
    , collisions(0)
    , averageSpeed(0.0)
    , friendlyFire(0)
{}

PlayerStats::PlayerStats(GameRole owner)
    : owner(owner)
    , shotsFired(0)
    , shotsHit(0)
    , enemiesDestroyed(0)
    , collisions(0)
    , averageSpeed(0.0)
    , friendlyFire(0)
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
    serializeParameters->outputBitstream[0].Write(friendlyFire);

    return RM3SR_BROADCAST_IDENTICALLY;
}

void PlayerStats::Deserialize(RakNet::DeserializeParameters *deserializeParameters) {
    deserializeParameters->serializationBitstream[0].Read(shotsFired);
    deserializeParameters->serializationBitstream[0].Read(shotsHit);
    deserializeParameters->serializationBitstream[0].Read(enemiesDestroyed);
    deserializeParameters->serializationBitstream[0].Read(collisions);
    deserializeParameters->serializationBitstream[0].Read(averageSpeed);
    deserializeParameters->serializationBitstream[0].Read(friendlyFire);
}

void PlayerStats::print() {
    if (owner == PILOT) {
        std::cout << "Collisions: " << collisions << "\n";
        std::cout << "Avg. speed: " << averageSpeed << "\n";
    }
    std::cout << "Shots fired: " << shotsFired << "\n";
    std::cout << "..on target: " << shotsHit << "\n";
    std::cout << "Enemies destroyed: " << enemiesDestroyed << "\n";
    std::cout << "Friendly fire: " << friendlyFire << "\n";
}
