#include "gunState.h"
#include "const.h"

bool GunState::fire()
{
    return isFire;
}
        
void GunState::tick()
{
    timeSinceLastFire++;

    isFire = false;
    if (playerControls == 0) return;


    if(playerControls->fire() && timeSinceLastFire >= Const::MIN_SHOOT_PERIOD) {
        isFire = true;
        timeSinceLastFire = 0;
    }

}
        
GunState::GunState(IPlayerControls *playerControls, GameRole owner)
    : playerControls(playerControls)
    , isFire(false)
    , timeSinceLastFire(0)
    , owner(owner)
{
}

GunState::GunState()
    : playerControls(0)
    , isFire(false),
      owner(NO_GAME_ROLE)
{}

GunState::~GunState() {}

void GunState::SerializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *destinationConnection) {
    constructionBitstream->Write(owner);
}

bool GunState::DeserializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *sourceConnection) {
    constructionBitstream->Read(owner);
    return true;
}

RakNet::RakString GunState::GetName(void) const {
    if (owner == PILOT) return RakNet::RakString("PilotGunState");
    if (owner == NAVIGATOR) return RakNet::RakString("NavigatorGunState");
    if (owner == ENGINEER) return RakNet::RakString("EngineerGunState");

    return RakNet::RakString("");
}

RM3SerializationResult GunState::Serialize(SerializeParameters *serializeParameters) {
    serializeParameters->outputBitstream[0].Write(isFire);
    return RM3SR_BROADCAST_IDENTICALLY;
}

void GunState::Deserialize(RakNet::DeserializeParameters *deserializeParameters) {
    bool isFire2 = false;
    deserializeParameters->serializationBitstream[0].Read(isFire2);
    if (isFire2) {
       isFire = true;
       std::cout << "FIRE" << std::endl;
    }
}
