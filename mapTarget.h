#ifndef MAP_TARGET_H 
#define MAP_TARGET_H

#include <OGRE/Ogre.h>
#include <string>
#include <iostream>
#include <math.h>

#include "IBulletTarget.h"

using namespace Ogre;
using namespace std;

class MapTarget : public IBulletTarget
{
    private:
        
    public:

        EntityType getEntityType() { return ENTT_MAP; };
        EffectType getHitEffectType() { return ET_SPARK; } ;
};

#endif
