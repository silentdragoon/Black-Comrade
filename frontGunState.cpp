
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
        bullet->fire();
    }
}
        
FrontGunState::FrontGunState(ShipControls *shipControls, Bullet *bullet)
    : shipControls(shipControls)
    , bullet(bullet)
    , isFire(false)
    , timeSinceLastFire(0)
{
    
}

FrontGunState::~FrontGunState() {}
