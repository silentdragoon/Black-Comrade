
#ifndef I_PLAYER_CONTROLS
#define I_PLAYER_CONTROLS

#include <OGRE/Ogre.h>

using namespace Ogre;

class IPlayerControls {
public:
	virtual ~IPlayerControls() {}
	virtual bool fire() = 0;
        virtual Vector3 cameraPosition() = 0;
        virtual Quaternion cameraOrientation() = 0;
};

#endif
