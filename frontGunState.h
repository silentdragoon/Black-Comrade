
#ifndef FRONT_GUN_STATE_H
#define FRONT_GUN_STATE_H

#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>
#include <OGRE/Ogre.h>
#include <iostream>
#include "ITickable.h"
#include "shipControls.h"
#include "bullet.h"

#define MIN_SHOOT_PERIOD 15

class FrontGunState : public ITickable
{
    private:
        int timeSinceLastFire;
        bool isFire;
        
        ShipControls *shipControls;
        Bullet *bullet;
        
    public:
        bool fire();
        
        virtual void tick();
        
        FrontGunState(ShipControls *shipControls, Bullet *bullet);
        ~FrontGunState();
};

#endif
