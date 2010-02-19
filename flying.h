#ifndef _FLYING_H
#define _FLYING_H

#include <OGRE/Ogre.h>
#include <math.h>
#include "ITickable.h"
#include "pilotControls.h"
#include "collisionManager.h"
#include "shipState.h"
#include "damageState.h"

using namespace Ogre;
using namespace std;

class Flying : public ITickable
{
    private:
        void updateShipState();
        
        bool useCollisions;
    
        int hitCountDown;
        CollisionManager *colMgr;
        PilotControls *sc;
        ShipState *shipState;
        DamageState *damageState;
        double vFactor;

        double zVel;
        double xVel;
        double yVel;

        double addPitch;
        double addRoll;
        double addYaw;
        double yawMom;

        double flyPitch;
        double flyYaw;
        double flyRoll;
        
        static const double EngineForce = 0.5;
        static const double SideForce = 0.13;
        
        double damage;

        void updateAngels();

        // double getDrag(string dir);
        void updatePosition();
        // void changeVector();
    public:
        Vector3 *position; //
        // Vector3 *velocity; // ( Units per tick )
        //Vector3 *angularVelocity; // pitch yaw roll ( Radians per tick )
        //Vector3 *orientation; // pitch yaw roll ( Radians )
        double pitch;
        double yaw;
        double roll;

        Flying(PilotControls *sc, ShipState *shipState, DamageState *damageState, CollisionManager *colMgr, bool collisions, double x, double y, double z);

        ~Flying();

        virtual void tick();
};

#endif

