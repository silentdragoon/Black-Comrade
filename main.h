
#ifndef MAIN_H
#define MAIN_H

// Includes all Ogre classes
#include <OGRE/Ogre.h>

#include "ExampleFrameListener.h"
#include "stateUpdate.h"
#include "shipState.h"
#include "engineState.h"
#include "motionState.h"
#include "mapCreate.h"
//#include "fixedMotionState.h"

using namespace Ogre;

class Main {
private:
    Root *root;
    Camera *camera;
    SceneManager *sceneMgr;
    RenderWindow *window;
    ExampleFrameListener *frameListener;
    
    SceneNode *mapNode;
    
    StateUpdate *stateUpdate;
    SceneNode *shipSceneNode;
    ShipState *shipState;
    
    MapCreate *mc;
    
    void createCamera();
    void createScene();
    void createViewPort();
    void createFrameListener(void);
    
public:
    Main();
};

#endif
