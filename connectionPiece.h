#ifndef ConnPiece_H
#define ConnPiece_H

#include <OGRE/Ogre.h>

#include "IDrawable.h"
#include "ITickable.h"

using namespace Ogre;
using namespace std;

class ConnectionPiece : public IDrawable, public ITickable
{
    private:
        Vector3 position;

        Vector3 orientation;
        Vector3 meshOrientation;

    public:

        ConnectionPiece(Vector3 position, float yaw);

        void tick();

        Vector3 *getPosition();
        Vector3 *getOrientation();
        Vector3 *getMeshOrientation();
        IDrawable *getParentObject();
        std::string getMeshName();
};

#endif
