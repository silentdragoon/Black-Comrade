
#ifndef MAIN_H
#define MAIN_H

// Includes all Ogre classes
#include <OGRE/Ogre.h>

#include "networkingManager.h"
#include "networkRole.h"
#include "stateUpdate.h"
#include "shipState.h"
#include "keyState.h"
#include "frontGunState.h"
#include "Kbhit.h"
#include "shipControls.h"
#include "accelerationState.h"
#include "motionState.h"
#include "soundManager.h"
#include "mapManager.h"
#include "audioState.h"
#include "miniGameManager.h"
#include "IExit.h"
#include "bulletManager.h"
#include "collisionManager.h"
#include "gameStateMachine.h"
#include "gameParameterMap.h"
#include "printState.h"

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
    CollaborationInfo *collabInfo;
    SoundManager *soundMgr;
    KeyState *ks;
    FrontGunState *frontGunState;
    ShipControls *sc;
    AccelerationState *as;
    MotionState *ms;
    AudioState *audioState;
    MiniGameManager *miniGameMgr;
    BulletManager *bulletMgr;
    CollisionManager *collisionMgr;
    GameStateMachine *gameStateMachine;
    GameParameterMap *gameParameterMap;
    PrintState *printState;
    
    SceneNode *mapNode;
    
    StateUpdate *stateUpdate;
    SceneNode *shipSceneNode;
    ShipState *shipState;
    
    MapManager *mapMgr;
    
    void createCamera();
    void createScene();
    void createViewPort();
    void startNetworking();

    void clientStartup();
    void serverStartup();
    
public:
    Main();
    ~Main();
    
    void exit();
};

#endif
