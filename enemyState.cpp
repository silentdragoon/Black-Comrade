
#include "enemyState.h"

#include <iostream>

EnemyState::EnemyState(SceneNode *neSceneNode, SceneManager *mSceneMgr) :
    position(new Vector3(0.0,0.0,-50.0)),
    roll(0.0),
    pitch(0.0),
    yaw(0.0),
    eSceneNode(neSceneNode),
    mSceneMgr(mSceneMgr)
{
    rRayQuery = new RayQuery( mSceneMgr );
    
    updateOgre();
}

void EnemyState::tick()
{
    
    Vector3 result(0,0,0);
    float dRight, dLeft, tmp;
    
    Vector3 futPos( position->x+(FVEL*LOOKA)*sin(yaw), position->y, position->z+(FVEL*LOOKA)*cos(yaw));
    
    Vector3 left(sin(yaw+1.57),0,cos(yaw+1.57));
    dLeft = rRayQuery->RaycastFromPoint(futPos, left, result);
    cout << " LD: " << dLeft;
    
    Vector3 right(sin(yaw-1.57),0,cos(yaw-1.57));
    dRight = rRayQuery->RaycastFromPoint(futPos, right, result);
    cout << " RD: " << dRight << endl;
    
    tmp = (dLeft + dRight) /2 - dRight;
    
    cout << " angle: " << 1.0f/2.0f*(atan(tmp/(FVEL*LOOKA))) << endl;
    
    yaw +=   1.0f/2.0f*atan(tmp/(FVEL*LOOKA));
    
    
    position->x += FVEL * sin(yaw);
    position->z += FVEL * cos(yaw);
    
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
