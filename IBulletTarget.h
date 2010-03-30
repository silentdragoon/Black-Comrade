#ifndef BULLET_TARGET_H
#define BULLET_TARGET_H

#include "IEntity.h"
#include "IDrawable.h"
#include "effectType.h"

using namespace Ogre;

class IBulletTarget : public IEntity {

public:

    virtual float getHealth() { return 1; }
    virtual void setHealth(float health) {}

    virtual EffectType getHitEffectType() { return ET_NONE; };
    virtual EffectType getDestroyedEffectType() { return ET_NONE; };
};

#endif
