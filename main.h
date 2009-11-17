
#ifndef MAIN_H
#define MAIN_H

// Includes all Ogre classes
#include <OGRE/Ogre.h>

#include "ExampleFrameListener.h"
#include "stateUpdate.h"
#include "shipState.h"
#include "keyState.h"
#include "accelerationState.h"
#include "motionState.h"
#include "IExit.h"

using namespace Ogre;

class Main : public IExit {
private:
    Root *root;
    Camera *camera;
    SceneManager *sceneMgr;
    RenderWindow *window;
    ExampleFrameListener *frameListener;
    
    KeyState *ks;
    AccelerationState *as;
    MotionState *ms;
    
    SceneNode *robotNode;
    
    StateUpdate *stateUpdate;
    SceneNode *shipSceneNode;
    ShipState *shipState;
    
    void createCamera();
    void createScene();
    void createViewPort();
    void createFrameListener(void);
    
public:
    Main();
    ~Main();
    
    void exit();
};

#endif
