
#ifndef MAIN_H
#define MAIN_H

// Includes all Ogre classes
#include <OGRE/Ogre.h>

#include "stateUpdate.h"
#include "shipState.h"
#include "engineState.h"
#include "motionState.h"
//#include "fixedMotionState.h"

using namespace Ogre;

class Main {
private:
    Root *root;
    Camera *camera;
    SceneManager *sceneMgr;
    RenderWindow *window;
    
    SceneNode *robotNode;
    
    StateUpdate *stateUpdate;
    SceneNode *shipSceneNode;
    ShipState *shipState;
    
    void createCamera();
    void createScene();
    void createViewPort();
    
public:
    Main();
};

#endif
