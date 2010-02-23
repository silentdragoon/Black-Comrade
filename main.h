
#ifndef MAIN_H
#define MAIN_H

// Includes all Ogre classes
#include <OGRE/Ogre.h>
//#include <CEGUI/CEGUI.h>
//#include <CEGUI/CEGUIBase.h>
//#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h>
//#include <CEGUI/CEGUIImageset.h>

#include "networkingManager.h"
#include "networkRole.h"
#include "stateUpdate.h"
#include "shipState.h"
#include "inputState.h"
#include "gunState.h"
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
#include "flying.h"
#include "navigatorControls.h"
#include "engineerControls.h"
#include "guiManager.h"
#include "guiStatusUpdater.h"
#include "sceneNodeManager.h"
#include "damageState.h"
#include "lines.h"
#include "notificationManager.h"
#include "systemManager.h"
#include "radarGui.h"

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
    SceneNodeManager *sceneNodeMgr;
    RenderWindow *window;
    NetworkingManager *networkingManager;
    bool isServer;
    CollaborationInfo *collabInfo;
    SoundManager *soundMgr;
    AccelerationState *as;
    MotionState *ms;
    Flying *flying;
    InputState *inputState;
    AccelerationState *accelerationState;
    MotionState *motionState;
    AudioState *audioState;
    MiniGameManager *miniGameMgr;
    BulletManager *bulletMgr;
    CollisionManager *collisionMgr;
    GameStateMachine *gameStateMachine;
    GameParameterMap *gameParameterMap;
    PrintState *printState;
    NotificationManager *notificationMgr;
    
    PilotControls *pilotControls;
    NavigatorControls *navigatorControls;
    EngineerControls *engineerControls;
   
    GunState *pilotGunState;
    GunState *engineerGunState;
    GunState *navigatorGunState;
    
    SceneNode *mapNode;
    
    StateUpdate *gameLoop;
    SceneNode *shipSceneNode;
    ShipState *shipState;
    DamageState *damageState;
    SystemManager *systemManager;
    
    MapManager *mapMgr;
    SwarmManager *swarmMgr;
    GuiManager *guiMgr;
    GuiStatusUpdater *guiStatusUpdater;
    RadarGui *radarGui;
    
    Lines *lines;

    Camera *createCamera(SceneNode *shipSceneNode);
    void createScene();
    void createViewPort();
    CollaborationInfo *runLoby(NetworkingManager *networkingManager);
    void addCrossHair();
    Root *configRoot();
    void configResources();
    
public:
    //enemies cannot be turned off yet
    Main( bool useKey, bool useMouse, bool enemies, bool collisions );
    ~Main();
    
    void exit();
};

#endif
