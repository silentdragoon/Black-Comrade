
#ifndef MAIN_H
#define MAIN_H

// Includes all Ogre classes
#include <OGRE/Ogre.h>

#include "ExampleFrameListener.h"
#include "stateUpdate.h"
#include "shipState.h"
#include "engineState.h"
#include "motionState.h"
#include "soundManager.h"
//#include "fixedMotionState.h"

using namespace Ogre;

class Main {
private:
    Root *root;
    Camera *camera;
    SceneManager *sceneMgr;
    RenderWindow *window;
    ExampleFrameListener *frameListener;
    SoundManager *soundMgr;
    
    SceneNode *robotNode;
    
    StateUpdate *stateUpdate;
    SceneNode *shipSceneNode;
    ShipState *shipState;
    
    void createCamera();
    void createScene();
    void createViewPort();
    void createFrameListener(void);
    void createSoundManager();
    
public:
    Main();
};

#endif
