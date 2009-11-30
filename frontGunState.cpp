
#include "frontGunState.h"

bool FrontGunState::fire()
{
    return isFire;
}
        
void FrontGunState::tick()
{
    if (shipControls == 0) return;

    isFire = false;

    timeSinceLastFire++;
    
    if(shipControls->fire() && timeSinceLastFire >= MIN_SHOOT_PERIOD) {
        isFire = true;
        timeSinceLastFire = 0;
    }
}
        
FrontGunState::FrontGunState(ShipControls *shipControls)
    : shipControls(shipControls)
    , isFire(false)
    , timeSinceLastFire(0)
{}

FrontGunState::FrontGunState()
    : shipControls(0)
{}

FrontGunState::~FrontGunState() {}

RakNet::RakString FrontGunState::GetName(void) const {return RakNet::RakString("FrontGunState");}

RM3SerializationResult FrontGunState::Serialize(SerializeParameters *serializeParameters) {
    serializeParameters->outputBitstream[0].Write(isFire);
    return RM3SR_BROADCAST_IDENTICALLY;
}

void FrontGunState::Deserialize(RakNet::DeserializeParameters *deserializeParameters) {
    deserializeParameters->serializationBitstream[0].Read(isFire);
    std::cout << isFire << std::endl;
}
