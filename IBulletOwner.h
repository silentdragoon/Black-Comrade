#ifndef BULLET_OWNER_H
#define BULLET_OWNER_H

#include "INamedEntity.h"

using namespace Ogre;

class IBulletOwner : public INamedEntity{
public:

    virtual EffectType getMuzzleFlashEffectType();
};

#endif
