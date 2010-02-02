#include "gunState.h"
#include "const.h"

bool GunState::pilotFire()
{
    return isPilotFire;
}

bool GunState::navFire()
{
    return isNavFire;
}

bool GunState::engFire()
{
    return isEngFire;
}
        
void GunState::tick()
{

    pilotTimeSinceLastFire++;
    navTimeSinceLastFire++;
    engTimeSinceLastFire++;

    isPilotFire = false;
    isNavFire = false;
    isEngFire = false;

    if(pilotControls!=0 && pilotControls->fire() && pilotTimeSinceLastFire >= Const::MIN_SHOOT_PERIOD) {
        isPilotFire = true;
        pilotTimeSinceLastFire = 0;
    }

}
        
GunState::GunState(PilotControls *pilotControls)
    : pilotControls(pilotControls)
    , navControls(0)
    , engControls(0)
    , isPilotFire(false)
    , isNavFire(false)
    , isEngFire(false)
    , pilotTimeSinceLastFire(0)
    , navTimeSinceLastFire(0)
    , engTimeSinceLastFire(0)
{
}

GunState::GunState(NavigatorControls *navControls)
    : pilotControls(0)
    , navControls(navControls)
    , engControls(0)
    , isPilotFire(false)
    , isNavFire(false)
    , isEngFire(false)
    , pilotTimeSinceLastFire(0)
    , navTimeSinceLastFire(0)
    , engTimeSinceLastFire(0)
{
}

GunState::GunState(EngineerControls *engControls)
    : pilotControls(0)
    , navControls(0)
    , engControls(engControls)
    , isPilotFire(false)
    , isNavFire(false)
    , isEngFire(false)
    , pilotTimeSinceLastFire(0)
    , navTimeSinceLastFire(0)
    , engTimeSinceLastFire(0)
{
}

GunState::GunState()
    : pilotControls(0)
    , navControls(0)
    , engControls(0)
    , isPilotFire(false)
    , isNavFire(false)
    , isEngFire(false)
    , pilotTimeSinceLastFire(0)
    , navTimeSinceLastFire(0)
    , engTimeSinceLastFire(0)
{}

GunState::~GunState() {}

RakNet::RakString GunState::GetName(void) const {return RakNet::RakString("GunState");}

RM3SerializationResult GunState::Serialize(SerializeParameters *serializeParameters) {
    serializeParameters->outputBitstream[0].Write(isPilotFire);
    return RM3SR_BROADCAST_IDENTICALLY;
}

void GunState::Deserialize(RakNet::DeserializeParameters *deserializeParameters) {
    bool isFire2 = false;
    deserializeParameters->serializationBitstream[0].Read(isFire2);
    if (isFire2) isPilotFire = true;
}
