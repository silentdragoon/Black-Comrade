
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
#include "soundManager.h"
#include "mapCreate.h"
#include "audioState.h"
#include "IExit.h"
#include "bulletManager.h"

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
    Bullet *bullet;
    
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
    void createSoundManager();
    
public:
    Main();
    ~Main();
    
    void exit();
};

#endif
