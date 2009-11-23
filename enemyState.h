
#ifndef ENEMY_STATE_H
#define ENEMY_STATE_H

// Includes all Ogre classes
#include <OGRE/Ogre.h>

#include "ITickable.h"

using namespace Ogre;

class EnemyState : public ITickable {
    
private:
    Vector3 *position;
    Real roll;
    Real pitch;
    Real yaw;
    
    SceneNode *eSceneNode;

    
public:
    
    EnemyState(SceneNode *neSceneNode );
    
    void tick();
    
    void updateOgre();
};

#endif
