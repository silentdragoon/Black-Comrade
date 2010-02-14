#ifndef I_DRAWABLE
#define I_DRAWABLE

#include <OGRE/Ogre.h>
#include <string>

using namespace Ogre;

class IDrawable {
public:
	virtual ~IDrawable() {};
        virtual Vector3 *getPosition()  = 0;
        virtual Vector3 *getOrientation() = 0;
        virtual Vector3 *getMeshOrientation() = 0;
        virtual IDrawable *getParentObject() = 0;
        virtual std::string getMeshName() = 0;
};

#endif
