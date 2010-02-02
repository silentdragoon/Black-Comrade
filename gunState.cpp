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
    if (pilotControls == 0) return;


    if(pilotControls->fire() && timeSinceLastFire >= Const::MIN_SHOOT_PERIOD) {
        isFire = true;
        timeSinceLastFire = 0;
    }

}
        
GunState::GunState(PilotControls *pilotControls)
    : pilotControls(pilotControls)
    , isFire(false)
    , timeSinceLastFire(0)
{
}

GunState::GunState()
    : pilotControls(0)
{}

GunState::~GunState() {}

RakNet::RakString GunState::GetName(void) const {return RakNet::RakString("GunState");}

RM3SerializationResult GunState::Serialize(SerializeParameters *serializeParameters) {
    serializeParameters->outputBitstream[0].Write(isFire);
    return RM3SR_BROADCAST_IDENTICALLY;
}

void GunState::Deserialize(RakNet::DeserializeParameters *deserializeParameters) {
    bool isFire2 = false;
    deserializeParameters->serializationBitstream[0].Read(isFire2);
    if (isFire2) isFire = true;
}
