
#include "shipState.h"

#include <iostream>

ShipState::ShipState(SceneNode *shipSceneNode, IMotionState *motionState) :
    position(new Vector3()),
    roll(0.0),
    pitch(0.0),
    yaw(0.0),
    shipSceneNode(shipSceneNode),
    motionState(motionState)
{}

void ShipState::tick()
{
    position->x += motionState->xVelocity();
    position->y += motionState->yVelocity();
    position->z += motionState->zVelocity();
    
    updateOgre();
}

void ShipState::updateOgre()
{
    shipSceneNode->setPosition(*position);
    
    //std::cout << shipSceneNode->getPosition().z << std::endl;
}
