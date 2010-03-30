#ifndef BULLET_TARGET_H
#define BULLET_TARGET_H

#include "IEntity.h"
#include "IDrawable.h"
#include "effectType.h"

using namespace Ogre;

class IBulletTarget : public IEntity {

public:

    virtual void damage(float amount) {}

    virtual EffectType getHitEffectType() { return ET_SPARK; };
    virtual EffectType getDestroyedEffectType() { return ET_NONE; };
};

#endif
