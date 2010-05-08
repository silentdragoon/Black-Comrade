#ifndef FAN_H
#define FAN_H

#include <OGRE/Ogre.h>

#include "IDrawable.h"
#include "ITickable.h"

using namespace Ogre;
using namespace std;

class Fan : public IDrawable, public ITickable
{
    private:

        Vector3 startPosition;
        Vector3 position;

        Vector3 orientation;
        Vector3 meshOrientation;

    public:

        Fan(Vector3 position, float yaw);

        void tick();

        Vector3 *getPosition();
        Vector3 *getOrientation();
        Vector3 *getMeshOrientation();
        IDrawable *getParentObject();
        std::string getMeshName();
};

#endif
