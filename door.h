#ifndef DOOR_H 
#define DOOR_H

#include <OGRE/Ogre.h>

#include "IDrawable.h"
#include "ITickable.h"
#define PI 3.141592654

#include "constManager.h"

using namespace Ogre;
using namespace std;

class Door : public IDrawable, public ITickable
{
    private:
        
        Vector3 startPosition;
        Vector3 position;
        
        Vector3 orientation;
        Vector3 meshOrientation;
        
        // in [0,1]
        float fracOpen;
        
        float motion;
        
        float moveSpeed();
        
    public:

        Door(Vector3 position, float yaw);
        
        bool isOpen();
        bool isClosed();
        
        void open();
        void close();
        
        void tick();
        
        Vector3 *getPosition();
        Vector3 *getOrientation();
        Vector3 *getMeshOrientation();
        IDrawable *getParentObject();
        std::string getMeshName();        
};

#endif
