#ifndef DOOR_H 
#define DOOR_H

#include <OGRE/Ogre.h>

#include "IDrawable.h"

using namespace Ogre;
using namespace std;

class Door : public IDrawable
{
    private:
    
        Vector3 position;
        
        Vector3 orientation;
        Vector3 meshOrientation;
        
    public:

        Door(Vector3 position, float yaw);
        
        Vector3 *getPosition();
        Vector3 *getOrientation();
        Vector3 *getMeshOrientation();
        IDrawable *getParentObject();
        std::string getMeshName();        
};

#endif
