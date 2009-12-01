
#include "frontGunState.h"

bool FrontGunState::fire()
{
    return isFire;
}
        
void FrontGunState::tick()
{

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
{
}

FrontGunState::~FrontGunState() {}
