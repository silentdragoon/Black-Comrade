#include "gunState.h"
#include "const.h"

bool GunState::fire()
{
    return isFire;
}
        
void GunState::tick()
{
    timeSinceLastFire++;

    if((isFire)&&(systemManager!=0)) systemManager->fireWeapon();

    isFire = false;

    if (playerControls == 0) return;

    position = playerControls->cameraPosition();
    orientation = playerControls->cameraOrientation();

    if(playerControls->fire() && timeSinceLastFire >= Const::MIN_SHOOT_PERIOD
        && damageState->getWeaponHealth() > 0.0) {
        if(systemManager->getWeaponCharge()>1.0) {
            isFire = true;
        }
        timeSinceLastFire = 0;
    }

}

Vector3 GunState::getPosition() { return position; }
Quaternion GunState::getOrientation() { return orientation; }
        
GunState::GunState(IPlayerControls *playerControls, DamageState *damageState, SystemManager *systemManager, GameRole owner)
    : playerControls(playerControls)
    , damageState(damageState)
    , systemManager(systemManager)
    , isFire(false)
    , timeSinceLastFire(0)
    , owner(owner)
    , position(Vector3(0.0,0.0,0.0))
    , orientation(Quaternion(1.0,0.0,0.0,0.0))
{
}

GunState::GunState()
    : playerControls(0)
    , isFire(false)
    , systemManager(0)
    , owner(NO_GAME_ROLE)
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
    serializeParameters->outputBitstream[0].Write(position.x);
    serializeParameters->outputBitstream[0].Write(position.y);
    serializeParameters->outputBitstream[0].Write(position.z);
    serializeParameters->outputBitstream[0].Write(orientation.w);
    serializeParameters->outputBitstream[0].Write(orientation.x);
    serializeParameters->outputBitstream[0].Write(orientation.y);
    serializeParameters->outputBitstream[0].Write(orientation.z);
    
    if (!isFire) return RM3SR_DO_NOT_SERIALIZE;
    return RM3SR_SERIALIZED_ALWAYS_IDENTICALLY;
}

void GunState::Deserialize(RakNet::DeserializeParameters *deserializeParameters) {
    deserializeParameters->serializationBitstream[0].Read(isFire);
    deserializeParameters->serializationBitstream[0].Read(position.x);
    deserializeParameters->serializationBitstream[0].Read(position.y);
    deserializeParameters->serializationBitstream[0].Read(position.z);
    deserializeParameters->serializationBitstream[0].Read(orientation.w);
    deserializeParameters->serializationBitstream[0].Read(orientation.x);
    deserializeParameters->serializationBitstream[0].Read(orientation.y);
    deserializeParameters->serializationBitstream[0].Read(orientation.z);
}
