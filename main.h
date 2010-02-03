
#ifndef MAIN_H
#define MAIN_H

// Includes all Ogre classes
#include <OGRE/Ogre.h>

#include "networkingManager.h"
#include "networkRole.h"
#include "stateUpdate.h"
#include "shipState.h"
#include "inputState.h"
#include "frontGunState.h"
#include "Kbhit.h"
#include "pilotControls.h"
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
#include "swarmManager.h"
#include "navigatorControls.h"

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
    InputState *inputState;
    FrontGunState *frontGunState;
    PilotControls *pilotControls;
    AccelerationState *accelerationState;
    MotionState *motionState;
    AudioState *audioState;
    MiniGameManager *miniGameMgr;
    BulletManager *bulletMgr;
    CollisionManager *collisionMgr;
    GameStateMachine *gameStateMachine;
    GameParameterMap *gameParameterMap;
    PrintState *printState;
    NavigatorControls *navControls;
    
    SceneNode *mapNode;
    
    StateUpdate *gameLoop;
    SceneNode *shipSceneNode;
    ShipState *shipState;
    
    MapManager *mapMgr;
    SwarmManager *swarmMgr;
    
    Camera *createCamera(SceneNode *shipSceneNode);
    void createScene();
    void createViewPort();
    CollaborationInfo *runLoby(NetworkingManager *networkingManager);
    
    Root *configRoot();
    void configResources();
    
public:
    Main();
    ~Main();
    
    void exit();
};

#endif
