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

    if(navControls!=0 && navControls->fire() && navTimeSinceLastFire >= Const::MIN_SHOOT_PERIOD) {
        isNavFire = true;
        navTimeSinceLastFire = 0;
    }

    if(engControls!=0 && engControls->fire() && engTimeSinceLastFire >= Const::MIN_SHOOT_PERIOD) {
        isEngFire = true;
        engTimeSinceLastFire = 0;
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
    className =  RakNet::RakString("PilotGunState");
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
    className = RakNet::RakString("NavigatorGunState");
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
    className = RakNet::RakString("EngineerGunState");
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
{
}

GunState::~GunState() {}

RakNet::RakString GunState::GetName(void) const {return className;}

RM3SerializationResult GunState::Serialize(SerializeParameters *serializeParameters) {
    serializeParameters->outputBitstream[0].Write(isPilotFire);
    serializeParameters->outputBitstream[0].Write(isNavFire);
    serializeParameters->outputBitstream[0].Write(isEngFire);
    serializeParameters->outputBitstream[0].Write(className);
    return RM3SR_BROADCAST_IDENTICALLY;
}

void GunState::Deserialize(RakNet::DeserializeParameters *deserializeParameters) {
    bool pilotFire2 = false;
    deserializeParameters->serializationBitstream[0].Read(pilotFire2);
    if (pilotFire2) isPilotFire = true;
    
    bool navFire2 = false;
    deserializeParameters->serializationBitstream[0].Read(navFire2);
    if (navFire2) isNavFire = true;

    bool engFire2 = false;
    deserializeParameters->serializationBitstream[0].Read(engFire2);
    if (engFire2) isEngFire = true;
    deserializeParameters->serializationBitstream[0].Read(className);
}
