
#ifndef SHIP_STATE_H
#define SHIP_STATE_H

// Includes all Ogre classes
#include <OGRE/Ogre.h>

#include "IMotionState.h"

using namespace Ogre;

class ShipState {
private:
    Vector3 *position;
    Real roll;
    Real pitch;
    Real yaw;
    
    SceneNode *shipSceneNode;
    IMotionState *motionState;
    
public:
    
    ShipState(SceneNode *shipSceneNode, IMotionState *motionState);
    
    void tick();
    
    void updateOgre();
};

#endif
