#ifndef BULLET_TARGET_H
#define BULLET_TARGET_H

#include "INamedEntity.h"

using namespace Ogre;

class IBulletTarget : public INamedEntity{
public:

    virtual EffectType getHitEffectType();
    virtual EffectType getDestroyedEffectType();
};

#endif
