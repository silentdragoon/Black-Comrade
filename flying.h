#ifndef _FLYING_H 
#define _FLYING_H

#include <OGRE/Ogre.h>
#include <math.h>
#include "ITickable.h"

using namespace Ogre;
using namespace std;

class Flying : public ITickable
{
    private:
               
    public:
        Flying();

        ~Flying();

        virtual void tick();
};

#endif

