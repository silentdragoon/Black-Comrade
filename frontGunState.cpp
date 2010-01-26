
#include "frontGunState.h"
#include "const.h"

bool FrontGunState::fire()
{
    return isFire;
}
        
void FrontGunState::tick()
{

    timeSinceLastFire++;

    isFire = false;
    if (shipControls == 0) return;


    if(shipControls->fire() && timeSinceLastFire >= Const::MIN_SHOOT_PERIOD) {
        isFire = true;
        timeSinceLastFire = 0;
    }

}
        
FrontGunState::FrontGunState(ShipControls *shipControls)
    : shipControls(shipControls)
    , isFire(false)
    , timeSinceLastFire(0)
{
}

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
    bool isFire2 = false;
    deserializeParameters->serializationBitstream[0].Read(isFire2);
    if (isFire2) isFire = true;
}
