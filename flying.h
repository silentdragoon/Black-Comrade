#ifndef _FLYING_H 
#define _FLYING_H

#include <OGRE/Ogre.h>
#include <math.h>
#include "ITickable.h"
#include "keyState.h"

using namespace Ogre;
using namespace std;

class Flying : public ITickable
{
    private:
        KeyState *ks;
        double getDrag(string dir);
        double getVelocity();
        void updatePosition();
    public:
        Vector3 *position; //
        Vector3 *velocity; // ( Units per tick )
        Vector3 *angularVelocity; // pitch yaw roll ( Radians per tick )
        Vector3 *orientation; // pitch yaw roll ( Radians )
        
        Flying(KeyState *ks, Vector3 *position, Vector3 *orientation);

        ~Flying();

        virtual void tick();
};

#endif

