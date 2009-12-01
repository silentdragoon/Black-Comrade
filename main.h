
#ifndef MAIN_H
#define MAIN_H

// Includes all Ogre classes
#include <OGRE/Ogre.h>

#include "enemyState.h"
#include "stateUpdate.h"
#include "shipState.h"
#include "keyState.h"
#include "frontGunState.h"
#include "shipControls.h"
#include "accelerationState.h"
#include "motionState.h"
#include "mapCreate.h"
#include "audioState.h"
#include "IExit.h"

using namespace Ogre;

class Main : public IExit {
private:
    Root *root;
    Camera *camera;
    SceneManager *sceneMgr;
    RenderWindow *window;
    
    KeyState *ks;
    FrontGunState *frontGunState;
    ShipControls *sc;
    AccelerationState *as;
    MotionState *ms;
    AudioState *audioState;
    
    SceneNode *mapNode;
    
    StateUpdate *stateUpdate;
    SceneNode *shipSceneNode;
    SceneNode *enemySceneNode;
    ShipState *shipState;
    EnemyState *enemyState;
    
    MapCreate *mc;
    
    void createCamera();
    void createScene();
    void createViewPort();
    
public:
    Main();
    ~Main();
    
    void exit();
};

#endif
