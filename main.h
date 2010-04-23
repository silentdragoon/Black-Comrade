#ifndef MAIN_H
#define MAIN_H

#include <OGRE/Ogre.h>
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
#include "mapManager2.h"
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
#include "hud.h"
#include "console.h"
#include "preGame.h"
#include "particleSystemEffectManager.h"
#include "playerStats.h"
#include "postGame.h"
#include "gameEnder.h"
#include "objective.h"
#include "fader.h"
#include "door.h"
#include "mapPieceChoices.h"
#include "tutorial.h"
#include <iostream>
#include "stateUpdate.h"
#include "networkRole.h"
#include "collaborationInfo.h"
#include "networkingManager.h"
#include "constManager.h"
#include <string.h>
#include <string>
#include <cstdio>
#include "spotLight.h"
#include "lightManager.h"

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
    CollaborationInfo *pilotInfo;
    CollaborationInfo *engineerInfo;
    CollaborationInfo *navigatorInfo;

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

    GunnerControls *myControls;
    PilotControls *pilotControls;
    NavigatorControls *navigatorControls;
    EngineerControls *engineerControls;

    GunState *pilotGunState;
    GunState *engineerGunState;
    GunState *navigatorGunState;

    SpotLight *pilotSpotLight;
    SpotLight *engineerSpotLight;
    SpotLight *navigatorSpotLight;

    SceneNode *mapNode;

    float shipScale;

    StateUpdate *gameLoop;
    SceneNode *shipSceneNode;
    ShipState *shipState;
    DamageState *damageState;
    SystemManager *systemManager;

    MapManager *mapMgr;
    SwarmManager *swarmMgr;
    GuiManager *guiMgr;
    GuiStatusUpdater *guiStatusUpdater;
    RadarGui *bigRadarGui;
    RadarGui *smallRadarGui;
    HUD *hud;
    Console *cons;
    Lines *lines;
    ParticleSystemEffectManager *particleSystemEffectManager;
    Objective *objective;

    LightManager *lightMgr;

    Fader *fader;

    Door *door;

    GameEnder *gameEnder;

    Tutorial *tutorial;

    Camera *createCamera(SceneNode *shipSceneNode);
    void createScene();
    void createViewPort();

    Root *configRoot();
    void configResources();

public:
    //enemies cannot be turned off yet
    Main( bool useKey, bool useMouse, bool enemies, bool collisions, bool rebuildCollisionMeshes );
    ~Main();

    void exit();
};

#endif
