
#ifndef MAIN_H
#define MAIN_H

// Includes all Ogre classes
#include <OGRE/Ogre.h>

#include "networkingManager.h"
#include "networkRole.h"
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
#include "miniGameManager.h"
#include "IExit.h"

//include networking stuff
#include <string.h>
#include <string>
#include <cstdio>

using namespace Ogre;
using namespace std;
using namespace RakNet;

class Main : public IExit {
private:
    Root *root;
    Camera *camera;
    SceneManager *sceneMgr;
    RenderWindow *window;
    NetworkingManager *networkingManager;
    bool isServer;
    NetworkRole role;
    SoundManager *soundMgr;
    KeyState *ks;
    FrontGunState *frontGunState;
    ShipControls *sc;
    AccelerationState *as;
    MotionState *ms;
    AudioState *audioState;
    MiniGameManager *miniGameMgr;
    
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
    NetworkRole startNetworking();

    void clientStartup();
    void serverStartup();

    void createSoundManager();
    
public:
    Main();
    ~Main();
    
    void exit();
};

#endif
