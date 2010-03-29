#ifndef BULLET_OWNER_H
#define BULLET_OWNER_H

#include "IEntity.h"
#include "effectType.h"

using namespace Ogre;

class IBulletOwner : public IEntity{
public:

    virtual Vector3 getBulletOrigin() { return Vector3(0.0,0.0,0.0); };
    virtual Vector3 getBulletDirection() { return Vector3(0.0,0.0,0.0); };
    virtual ColourValue getBulletColour() { return ColourValue(1.0f,1.0f,1.0f); } ;
    virtual int getBulletVelocity() { return 50;};
    virtual int getBulletDamage() { return 1; };
    virtual EffectType getMuzzleFlashEffectType() { return ET_NONE; } ;
};

#endif
