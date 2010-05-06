#include "main.h"

Main::Main(  bool useKey, bool useMouse, bool enemies, bool collisions, bool rebuildCollisionMeshes) {
    // Start Ogre
    root = configRoot();

    // Load constants
    ConstManager::getSingleton();

    sceneMgr = root->createSceneManager(ST_GENERIC);
    window = root->initialise(true, "BlackComrade");
    //sceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_MODULATIVE);

    // Static Lines class
    lines = new Lines(sceneMgr);

    configResources();

    // GUI Manager
    guiMgr = new GuiManager(sceneMgr);

    // Game Loop
    gameLoop = new StateUpdate();

    // SceneNode Manager
    sceneNodeMgr = new SceneNodeManager(sceneMgr);

    // User Input
    inputState = new InputState(window,true,true,useMouse);
    gameLoop->addTickable(inputState,"inputState");

    // Screen fader
    fader = new Fader(guiMgr);
    gameLoop->addTickable(fader,"fader");

    // Networking
    networkingManager = new NetworkingManager(this);

    // Pre-game environment
    PreGame *preGame = new PreGame(sceneMgr,window,inputState,guiMgr,networkingManager);

    // Sound manager
    soundMgr = new SoundManager();
    
    // Screenshots
    screenshot = new Screenshot(inputState,window);
    gameLoop->addTickable(screenshot,"screenshot");

    collabInfo = preGame->showMenus();

    lightMgr = new LightManager(sceneMgr);

    // Map
    MapPieceChoices *mapPieceChoices;
    std::string mapFileName = ConstManager::getString("map_file_name");
    if (mapFileName == "") mapFileName = "examplemap_new.txt";
    if (collabInfo->getGameRole() == PILOT) {
        mapMgr = new MapManager((char*)mapFileName.c_str(), sceneMgr,lightMgr);
        mapPieceChoices = mapMgr->getChosenPieces();
        networkingManager->replicate(mapPieceChoices);
    } else {
        mapPieceChoices = (MapPieceChoices*) networkingManager->getReplica("MapPieceChoices",true);
        mapMgr = new MapManager((char*)mapFileName.c_str(), mapPieceChoices, sceneMgr, lightMgr);
    }


    if (!useMouse) // || collabInfo->getNetworkRole() == DEVELOPMENTSERVER)
        inputState->releaseMouse();
    if (!useKey) inputState->releaseKeyboard();

    // Player info
    networkingManager->replicate(collabInfo);
    pilotInfo = (CollaborationInfo*) networkingManager->getReplica("PilotInfo",true);

    if (collabInfo->getNetworkRole() != DEVELOPMENTSERVER) {
        engineerInfo = (CollaborationInfo*) networkingManager->getReplica("EngineerInfo",true);
        navigatorInfo = (CollaborationInfo*) networkingManager->getReplica("NavigatorInfo",true);
    } else {
        engineerInfo = new CollaborationInfo("Engineer",CLIENT,ENGINEER);
        navigatorInfo = new CollaborationInfo("Navigator",CLIENT,NAVIGATOR);
        engineerInfo->hasCompletedTutorial = true;
        navigatorInfo->hasCompletedTutorial = true;
    }

    // Player stats
    PlayerStats *myStats = new PlayerStats(collabInfo->getGameRole());
    PlayerStats *pilotStats, *engStats, *navStats;
    collabInfo->setPlayerStats(myStats);
    networkingManager->replicate(myStats);
    pilotStats = (PlayerStats*) networkingManager->getReplica("PilotStats",true);
    pilotInfo->setPlayerStats(pilotStats);

    if (collabInfo->getNetworkRole() != DEVELOPMENTSERVER) {
        engStats = (PlayerStats*) networkingManager->getReplica("EngineerStats",true);
        engineerInfo->setPlayerStats(engStats);
        navStats = (PlayerStats*) networkingManager->getReplica("NavigatorStats",true);
        navigatorInfo->setPlayerStats(navStats);
    } else {
        engStats = new PlayerStats(ENGINEER);
        engineerInfo->setPlayerStats(engStats);
        navStats = new PlayerStats(NAVIGATOR);
        navigatorInfo->setPlayerStats(navStats);
    }

    std::cout << "Your pilot is " << pilotInfo->getNick() << std::endl;
    std::cout << "Your engineer is " << engineerInfo->getNick() << std::endl;
    std::cout << "Your navigator is " << navigatorInfo->getNick() << std::endl;

    // Effects creator
    particleSystemEffectManager = new ParticleSystemEffectManager(sceneMgr, mapMgr, shipSceneNode);
    //particleSystemEffectManager->createEngineGlow();

    // Objective
    if (collabInfo->getGameRole() == PILOT) {
        //objective = new Objective(particleSystemEffectManager,collisionMgr);
        objective = new Objective(particleSystemEffectManager);
        networkingManager->replicate(objective);
    } else {
        objective = (Objective*) networkingManager->getReplica("Objective",true);
        objective->setParticleSystemEffectManager(particleSystemEffectManager);
    }
    gameLoop->addTickable(objective,"objective");

    // Collision Manager (takes 99% of our loading time)
    collisionMgr = new CollisionManager(sceneMgr,mapMgr,objective,preGame->getLoadingScreen(), rebuildCollisionMeshes);

    // Damage State
    if (collabInfo->getGameRole() == PILOT || collabInfo->getNetworkRole() == DEVELOPMENTSERVER) {
        damageState = new DamageState(pilotInfo,engineerInfo,navigatorInfo);
        networkingManager->replicate(damageState);
    } else {
        damageState =
                (DamageState*) networkingManager->getReplica("DamageState",true);
    }

    MapTile *startMapTile = mapMgr->getMapTile(&mapMgr->getStartingPosition());
    int i = (startMapTile->getConnections())[0];

    // Ship State
    if(collabInfo->getGameRole() == PILOT) {
        shipState = new ShipState();
        networkingManager->replicate(shipState);
    } else {
        shipState =
            (ShipState*) networkingManager->getReplica("ShipState",true);
    }
    Vector3 startingPosition = mapMgr->getStartingPosition();
    shipState->setDamageState(damageState);
    shipState->setX(startingPosition.x);
    shipState->setY(0);
    shipState->setZ(startingPosition.z);
    shipState->yaw = (i % 2) ? 0 :  PI / 2;
    //cout << mapMgr->startx << ", " << mapMgr->starty << endl;
    gameLoop->addTickable(shipState, "shipState");
    soundMgr->setShipState(shipState);

    // Ship Node
    shipSceneNode = sceneNodeMgr->createNode(shipState);

    // SCALE SHIP!!!!
    shipScale = ConstManager::getFloat("ship_scale");
    shipSceneNode->setScale(shipScale,shipScale,shipScale);

    soundMgr->setShipNode(shipSceneNode);
    Entity *shipEntity = sceneNodeMgr->getEntity(shipState);
    if (collabInfo->getGameRole() == PILOT) {
        shipEntity->setVisible(false);
    }

    // Door
    Vector3 doorPos = *startMapTile->getSpawn(i);
    Door *door = new Door(doorPos,(i % 2) ? 0 :  PI / 2);
    sceneNodeMgr->createNode(door);
    collisionMgr->addColidableMovableObject(sceneNodeMgr->getEntity(door));
    gameLoop->addTickable(door, "Door");

    // Camera
    camera = createCamera(shipSceneNode);
    if(collabInfo->getGameRole() == PILOT) {
        camera->setPosition(Vector3(0,0,-8));
    } else if(collabInfo->getGameRole() == NAVIGATOR) {
        camera->setPosition(Vector3(0,4.3,0));
    } else if(collabInfo->getGameRole() == ENGINEER) {
        camera->setPosition(Vector3(0,-4.3,0));
    }

    // Engineer Controls
    if(collabInfo->getGameRole() == ENGINEER) {
        engineerControls = new EngineerControls(inputState,camera);
        gameLoop->addTickable(engineerControls,"engineerControls");

        systemManager = new SystemManager(engineerControls, damageState);

        networkingManager->replicate(systemManager);
    } else {
        if (collabInfo->getNetworkRole() == DEVELOPMENTSERVER) {
            std::cout << "Making dummy sysmanager\n";
            systemManager = new SystemManager();
        } else {
            systemManager = (SystemManager*) networkingManager->
                getReplica("SystemManager",true);
        }
    }

    // Pilot Controls
    if(collabInfo->getGameRole() == PILOT) {
        collisionMgr->addShipMesh(shipEntity);
        pilotControls = new PilotControls(inputState,camera);
        //last 3 terms of flying are the starting position x y z. Note mapMgr->starty = z
        flying = new Flying( sceneNodeMgr, pilotControls, shipState,
                             damageState, collisionMgr, systemManager,
                             collisions, startingPosition.x, 0.0, startingPosition.z, (i % 2) ? 0 :  PI / 2,
                             pilotInfo->getPlayerStats() );
        gameLoop->addTickable(pilotControls,"pilotControls");
        gameLoop->addTickable(flying,"flying");
    }

    // Navigator Controls
    if(collabInfo->getGameRole() == NAVIGATOR) {
        navigatorControls = new NavigatorControls(inputState,camera);
        gameLoop->addTickable(navigatorControls,"navigatorControls");
    }

    // My controls
    if (collabInfo->getGameRole() == PILOT) {
        myControls = pilotControls;
    } else if (collabInfo->getGameRole() == NAVIGATOR) {
        myControls = navigatorControls;
    } else if (collabInfo->getGameRole() == ENGINEER) {
        myControls = engineerControls;
    }

    // Console
    cons = new Console(sceneMgr);
    gameLoop->addTickable(cons,"console");

    // Minigame manager
    miniGameMgr = new MiniGameManager(cons,inputState,myControls,sceneMgr,collabInfo,this);
    gameLoop->addTickable(miniGameMgr,"miniGameManager");

    // Tutorial
    tutorial = new Tutorial(collabInfo,pilotInfo,navigatorInfo,engineerInfo,guiMgr,hud,
                            miniGameMgr,damageState,systemManager,shipState, door,inputState);
    gameLoop->addTickable(tutorial,"tutorial");

    // GameState
    if(collabInfo->getGameRole() == PILOT) {
        gameStateMachine = new GameStateMachine(mapMgr,inputState,
                                                pilotInfo,engineerInfo,navigatorInfo,
                                                tutorial,shipState,damageState,objective);
        networkingManager->replicate(gameStateMachine);
    } else {
        gameStateMachine =
            (GameStateMachine*) networkingManager->
                getReplica("GameStateMachine",true);
        gameStateMachine->setInputState(inputState);
        gameStateMachine->setInfos(pilotInfo,navigatorInfo,engineerInfo);
    }
    gameLoop->addTickable(gameStateMachine,"gameStateMachine");
    gameParameterMap = new GameParameterMap(gameStateMachine);

    // Print Game State changes
    printState = new PrintState(gameStateMachine);
    gameLoop->addTickable(printState,"printState");

    // Notifications
    if (collabInfo->getGameRole() == NAVIGATOR || collabInfo->getNetworkRole() == DEVELOPMENTSERVER) {
        notificationMgr = new NotificationManager(collabInfo, gameStateMachine, mapMgr, shipState,
                                                  damageState, systemManager, tutorial, objective);
        networkingManager->replicate(notificationMgr);
    } else {
        notificationMgr = (NotificationManager*) networkingManager->
            getReplica("NotificationManager",true);
        notificationMgr->setCollaborationInfo(collabInfo);
        notificationMgr->setTutorial(tutorial);
    }
    gameLoop->addTickable(notificationMgr,"notifications");

    // Pilot Gun State
    if(collabInfo->getGameRole() == PILOT) {
        pilotGunState = new GunState(pilotControls,damageState,systemManager,collabInfo);
        networkingManager->replicate(pilotGunState);
    } else if(collabInfo->getGameRole() == ENGINEER) {
        pilotGunState = (GunState*) networkingManager->
            getReplica("PilotGunState",true);
        pilotGunState->setSystemManager(systemManager);
        std::cout << "Got pilot gun from net" << std::endl;
    } else {
        pilotGunState = (GunState*) networkingManager->
        getReplica("PilotGunState",true);
    }
    gameLoop->addTickable(pilotGunState,"pilotGunState");

    // Navigator Gun State
    if(collabInfo->getGameRole() == NAVIGATOR) {
        navigatorGunState = new GunState(navigatorControls,damageState,systemManager,collabInfo);
        networkingManager->replicate(navigatorGunState);
        gameLoop->addTickable(navigatorGunState,"navigatorGunState");
    } else if(collabInfo->getGameRole() == ENGINEER) {
        navigatorGunState = (GunState*) networkingManager->
            getReplica("NavigatorGunState",true);
        navigatorGunState->setSystemManager(systemManager);
        std::cout << "Got nav gun from net" << std::endl;
        gameLoop->addTickable(navigatorGunState,"navigatorGunState");
    } else {
        if (collabInfo->getNetworkRole() != DEVELOPMENTSERVER) {
            navigatorGunState = (GunState*) networkingManager->
                getReplica("NavigatorGunState",true);
                std::cout << "Got nav gun from net" << std::endl;
            gameLoop->addTickable(navigatorGunState,"navigatorGunState");
        }
    }

    // Engineer Gun State
    if(collabInfo->getGameRole() == ENGINEER) {
        engineerGunState = new GunState(engineerControls,damageState,systemManager,collabInfo);
        networkingManager->replicate(engineerGunState);
        gameLoop->addTickable(engineerGunState,"engineerGunState");
    } else {
        if (collabInfo->getNetworkRole() != DEVELOPMENTSERVER) {
            engineerGunState = (GunState*) networkingManager->
                getReplica("EngineerGunState",true);
                std::cout << "Got eng gun from net" << std::endl;
            gameLoop->addTickable(engineerGunState,"engineerGunState");
        }
    }

    // Spot Lights
    pilotSpotLight = new SpotLight(sceneMgr, shipSceneNode, pilotGunState);
    gameLoop->addTickable(pilotSpotLight, "pilotSpotLight");

    if(engineerGunState) {
        engineerSpotLight = new SpotLight(sceneMgr, shipSceneNode,
             engineerGunState);
        gameLoop->addTickable(engineerSpotLight, "engineerSpotLight");
    }
    if(navigatorGunState) {
        navigatorSpotLight = new SpotLight(sceneMgr, shipSceneNode,
             navigatorGunState);
        gameLoop->addTickable(navigatorSpotLight, "navigatorSpotLight");
    }

    // TODO: start the enemies pointing towards the ship?
    // Swarm Manager
    if (collabInfo->getGameRole() == PILOT) {
        swarmMgr = new SwarmManager(sceneMgr, sceneNodeMgr, gameParameterMap, mapMgr,
            shipState,collisionMgr,networkingManager,lines,gameStateMachine,particleSystemEffectManager,soundMgr);
    } else {
        swarmMgr = new SwarmManager(sceneMgr, sceneNodeMgr, gameParameterMap,
            networkingManager,particleSystemEffectManager,soundMgr,collisionMgr);
    }

    // Networking
    gameLoop->addTickable(networkingManager,"networkingManager");

    gameLoop->addTickable(damageState, "damageState");

    gameLoop->addTickable(particleSystemEffectManager, "psem");

    // Bullet Manager
    bulletMgr = new BulletManager(shipState,sceneMgr,pilotGunState,
        engineerGunState,navigatorGunState,collisionMgr,swarmMgr,sceneNodeMgr,
        damageState,particleSystemEffectManager,objective);

    gameLoop->addTickable(bulletMgr,"bulletManager");
    gameLoop->addTickable(swarmMgr, "swarmMgr");


    gameLoop->addTickable(systemManager,"systemManager");

    // Audio
    gameLoop->addTickable(soundMgr,"soundManager");
    audioState = new AudioState(pilotGunState,soundMgr,shipSceneNode,
                                notificationMgr,bulletMgr,miniGameMgr,
                                gameStateMachine);
    gameLoop->addTickable(audioState,"audioState");

    // Radar GUI
    if (collabInfo->getGameRole() == ENGINEER) {
    	bigRadarGui = new RadarGui(guiMgr, shipState, swarmMgr, hud, true, 
            "BigRadar", engineerControls, damageState);
        gameLoop->addTickable(bigRadarGui,"BigRadar");
    	smallRadarGui = new RadarGui(guiMgr, shipState, swarmMgr, hud, false,
            "SmallRadar", engineerControls, damageState);
        gameLoop->addTickable(smallRadarGui,"SmallRadar");
    }
    gameLoop->addTickable(sceneNodeMgr,"sceneNodeMgr");

    // Game timer
    GameTimer *timer = new GameTimer(gameStateMachine);
    gameLoop->addTickable(timer,"timer");

    // Game ender
    gameEnder = new GameEnder(gameStateMachine,guiMgr,this);
    gameLoop->addTickable(gameEnder,"gameEnder");

    // Add the reactor core effects
    particleSystemEffectManager->makeObjective();

    // Wait for the players to be ready
    std::cout << "Waiting for players...\n";
    preGame->waitForPlayers();

    // CEGUI Stuff
    hud = new HUD(guiMgr, shipState,collabInfo->getGameRole(),mapMgr);
    guiStatusUpdater = new GuiStatusUpdater(guiMgr,gameLoop,damageState,myControls,
                                            collabInfo->getGameRole(),systemManager,hud,
                                            flying,notificationMgr,gameStateMachine,objective,
                                            cons, pilotInfo,navigatorInfo,engineerInfo, tutorial);
    gameLoop->addTickable(guiStatusUpdater,"guiStatusUpdater");

    gameLoop->addTickable(lightMgr,"lightMgr");

    soundMgr->changeMusic(MS_STEALTH); // Switch to stealth music

    // Viewport
    createViewPort();

    // Start Rendering Loop
    gameLoop->startLoop();

    // Hide the console if the game has ended
    cons->forceHide();

    networkingManager->endGame();

    // Post-game environment
    PostGame *postGame = new PostGame(sceneMgr,window,inputState,
                                      guiMgr,soundMgr,pilotInfo,navigatorInfo,
                                      engineerInfo,gameStateMachine->currentGameState(),
                                      damageState,timer->getTime());

    std::cout << "Pilot stats:" << "\n";
    pilotInfo->getPlayerStats()->print();

    std::cout << "Nav stats:" << "\n";
    navigatorInfo->getPlayerStats()->print();

    std::cout << "Eng stats:" << "\n";
    engineerInfo->getPlayerStats()->print();

    postGame->showMenus();

    networkingManager->stopNetworking();
}

Root *Main::configRoot()
{
    Root *root = new Root();

    if (!root->restoreConfig())
        root->showConfigDialog();

    return root;
}

void Main::configResources()
{
    ResourceGroupManager::getSingleton().addResourceLocation(
                    ConstManager::getString("map_file_path"),"FileSystem", "General");

    ResourceGroupManager::getSingleton().addResourceLocation(
                    ConstManager::getString("scripts_file_path"),"FileSystem", "General");

    ResourceGroupManager::getSingleton().addResourceLocation(
                    ConstManager::getString("textures_file_path"),"FileSystem", "General");

    ResourceGroupManager::getSingleton().addResourceLocation(
                    ".", "FileSystem", "General");

    ResourceGroupManager::getSingleton().addResourceLocation(
                    ConstManager::getString("models_file_path"), "FileSystem", "General");

    ResourceGroupManager::getSingleton().addResourceLocation(
                    "sounds", "FileSystem", "General");

    ResourceGroupManager::getSingleton().addResourceLocation(
                    "materials/programs", "FileSystem", "General");

    ResourceGroupManager::getSingleton().addResourceLocation(
                    "particles", "FileSystem", "General");

    ResourceGroupManager::getSingleton().addResourceLocation(
                    "cegui/fonts", "FileSystem", "fonts");

    ResourceGroupManager::getSingleton().addResourceLocation(
                    "cegui/imagesets", "FileSystem", "imagesets");

    ResourceGroupManager::getSingleton().addResourceLocation(
                    "cegui/layouts", "FileSystem", "layouts");

    ResourceGroupManager::getSingleton().addResourceLocation(
                    "cegui/looknfeel", "FileSystem", "looknfeels");

    ResourceGroupManager::getSingleton().addResourceLocation(
                    "cegui/lua_scripts", "FileSystem", "lua_scripts");

    ResourceGroupManager::getSingleton().addResourceLocation(
                    "cegui/schemes", "FileSystem", "schemes");


    ResourceGroupManager::getSingleton().addResourceLocation(
                    "cegui/xml_schemas", "FileSystem", "xml_schemas");

    ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    ResourceGroupManager::getSingleton().loadResourceGroup("General");

}

Camera *Main::createCamera(SceneNode *shipSceneNode) {

    Camera *camera = sceneMgr->createCamera("mainCam");

    shipSceneNode->attachObject(camera);

    camera->setPosition(Vector3(0,0,0));
    camera->lookAt(Vector3(0,0,-1));
    //camera->setFOVy(Radian(2.0943951));
    camera->setNearClipDistance(0.1);
    camera->setFarClipDistance(2500);

    // Lighting
    //sceneMgr->setShadowColour(ColourValue(0.5,0.5,0.5));

    // Add some sexy fog
    ColourValue fadeColour(0.1,0.1,0.1);
    sceneMgr->setFog(FOG_LINEAR, fadeColour, 0.01,50,450);

    /*Light *sp = sceneMgr->createLight("ShipLight");
    sp->setType(Light::LT_POINT);
    sp->setDiffuseColour(1.0,1.0,1.0);
    sp->setSpecularColour(1.0,1.0,1.0);
    sp->setDirection(Vector3(0,0,1));
    sp->setAttenuation( 600, 1.0, 0.007, 0.0002);*/

    // Josh if you are looking for the spot light,
    // they have been moved into the class "spotLight.h"

    //shipSceneNode->attachObject(sp);

    return camera;
}

//lol                ,
//lol          (`.  : \               __..----..__
//lol           `.`.| |:          _,-':::''' '  `:`-._
//lol             `.:\||       _,':::::'         `::::`-.
//lol               \\`|    _,':::::::'     `:.     `':::`.
//lol                ;` `-''  `::::::.                  `::\
//lol             ,-'      .::'  `:::::.         `::..    `:\
//lol           ,' /_) -.            `::.           `:.     |
//lol         ,'.:     `    `:.        `:.     .::.          \
//lol    __,-'   ___,..-''-.  `:.        `.   /::::.         |
//lol   |):'_,--'           `.    `::..       |::::::.      ::\
//lol    `-'                 |`--.:_::::|_____\::::::::.__  ::|
//lol                        |   _/|::::|      \::::::|::/\  :|
//lol                        /:./  |:::/        \__:::):/  \  :\
//lol                      ,'::'  /:::|        ,'::::/_/    `. ``-.__
//lol        will         ''''   (//|/\      ,';':,-'         `-.__  `'--..__
//lol                                                              `''---::::'


void Main::createViewPort() {

    Viewport *vp = window->addViewport(camera);
    vp->setBackgroundColour(ColourValue(0,0,0));

    camera->setAspectRatio(
        Real(vp->getActualWidth()) / Real(vp->getActualHeight()*1.17));
    //camera->setAspectRatio(1.17);

    vp->update();
}

int main(int argc,char *argv[])
{
    bool useKey = true;
    bool useMouse = true;
    bool enemies = true;
    bool collisions = true;
    bool rebuildCollisionMeshes = false;
    cout << "argc=" << argc << endl;
    cout << argv[0] << endl;
    for (int i=1;i<argc;i++)
    {
        if (string(argv[i]) == "-nk") {
            useKey = false;
            cout << "Keys are not bound." <<endl;
        } else if (string(argv[i]) == "-ne") {
            enemies = false; //not done yet
            cout << "Flag for no enemies entered." <<endl;
        } else if (string(argv[i]) == "-nc") {
            collisions = false;
            cout << "Disabling ship-wall collisions." <<endl;
        } else if (string(argv[i]) == "-nm") {
            useMouse = false;
            cout << "Mouse is not bound" <<endl;
        }
         else if (string(argv[i]) == "-rc") {
            rebuildCollisionMeshes = true;
            cout << "Rebuiling collision meshes" <<endl;
        }
    }
    Main *main = new Main(useKey, useMouse, enemies, collisions, rebuildCollisionMeshes);
    delete main;
}

Main::~Main()
{
    delete cons;
    delete inputState;
    delete soundMgr;

    OGRE_DELETE root;
}

void Main::exit()
{
    soundMgr->stopEngine();
    //soundMgr->changeMusic(4); // Change back to theme music
    gameLoop->running = false;
}

