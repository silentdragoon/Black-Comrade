#ifndef _FLYING_H 
#define _FLYING_H

#include <OGRE/Ogre.h>
#include <math.h>
#include "ITickable.h"
#include "pilotControls.h"
#include "collisionManager.h"

using namespace Ogre;
using namespace std;

class Flying : public ITickable
{
    private:
        int hitCountDown;
        CollisionManager *colMgr;
        PilotControls *sc;
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
        
        static const double EngineForce = 0.3;
        static const double SideForce = 0.2;

        
        
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
        
        Flying(PilotControls *sc,CollisionManager *colMgr);

        ~Flying();

        virtual void tick();
};

#endif

