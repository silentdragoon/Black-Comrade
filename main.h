
#ifndef MAIN_H
#define MAIN_H

// Includes all Ogre classes
#include <OGRE/Ogre.h>

#include "stateUpdate.h"
#include "shipState.h"
#include "keyState.h"
#include "frontGunState.h"
#include "shipControls.h"
#include "accelerationState.h"
#include "motionState.h"
#include "soundManager.h"
#include "audioState.h"
#include "IExit.h"

using namespace Ogre;

class Main : public IExit {
private:
    Root *root;
    Camera *camera;
    SceneManager *sceneMgr;
    RenderWindow *window;
    SoundManager *soundMgr;
    KeyState *ks;
    FrontGunState *frontGunState;
    ShipControls *sc;
    AccelerationState *as;
    MotionState *ms;
    AudioState *audioState;
    
    SceneNode *robotNode;
    
    StateUpdate *stateUpdate;
    SceneNode *shipSceneNode;
    ShipState *shipState;
    
    void createCamera();
    void createScene();
    void createViewPort();
    void createSoundManager();
    
public:
    Main();
    ~Main();
    
    void exit();
};

#endif
