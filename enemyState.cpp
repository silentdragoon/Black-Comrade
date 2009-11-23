
#include "enemyState.h"

#include <iostream>

EnemyState::EnemyState(SceneNode *neSceneNode) :
    position(new Vector3(0.0,0.0,-500.0)),
    roll(0.0),
    pitch(0.0),
    yaw(0.0),
    eSceneNode(neSceneNode)
{}

void EnemyState::tick()
{
    position->z += 0.1;
    updateOgre();
}

void EnemyState::updateOgre()
{
	eSceneNode->resetOrientation();

    eSceneNode->setPosition(*position);
    
    Radian roll(this->roll);
    Radian pitch(this->pitch);
    Radian yaw(this->yaw);
    
    eSceneNode->yaw(yaw);
    eSceneNode->roll(roll);
    eSceneNode->pitch(pitch);
        
}
