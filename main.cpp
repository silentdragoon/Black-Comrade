
#include "main.h"
#include <iostream>
#include "stateUpdate.h"

#include "networkRole.h"
#include "collaborationInfo.h"
#include "networkingManager.h"
#include "constManager.h"

using namespace RakNet;

Main::Main(  bool useKey, bool useMouse, bool enemies, bool collisions  ) {
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

    guiMgr = new GuiManager(sceneMgr);

    // Explosion creator
    particleSystemEffectManager = new ParticleSystemEffectManager(sceneMgr);

    // Game Loop
    gameLoop = new StateUpdate();

    // User Input
    inputState = new InputState(window,true,this,useKey,useMouse);
    gameLoop->addTickable(inputState,"inputState");

    // Networking
    networkingManager = new NetworkingManager(this);

    // Pre-game environment
    PreGame *preGame = new PreGame(sceneMgr,window,inputState,guiMgr,networkingManager);

    collabInfo = preGame->run();

    //TODO Release keyboard and mouse here if in nk/nm mode

    // Other players' state
    networkingManager->replicate(collabInfo);
    pilotInfo = (CollaborationInfo*) networkingManager->getReplica("PilotInfo",true);

    if (collabInfo->getNetworkRole() != DEVELOPMENTSERVER) {
        engineerInfo = (CollaborationInfo*) networkingManager->getReplica("EngineerInfo",true);
        navigatorInfo = (CollaborationInfo*) networkingManager->getReplica("NavigatorInfo",true);
    } else {
        engineerInfo = new CollaborationInfo("Engineer",CLIENT,ENGINEER);
        navigatorInfo = new CollaborationInfo("Navigator",CLIENT,NAVIGATOR);
    }

    std::cout << "Your pilot is " << pilotInfo->getNick() << std::endl;
    std::cout << "Your engineer is " << engineerInfo->getNick() << std::endl;
    std::cout << "Your navigator is " << navigatorInfo->getNick() << std::endl;

    // Damage State
    if (collabInfo->getGameRole() == PILOT) {
        damageState = new DamageState(pilotInfo,engineerInfo,navigatorInfo);
        networkingManager->replicate(damageState);
    } else {
        damageState =
                (DamageState*) networkingManager->getReplica("DamageState",true);
    }

    // SceneNode Manager
    sceneNodeMgr = new SceneNodeManager(sceneMgr);
    gameLoop->addTickable(sceneNodeMgr,"sceneNodeMgr");

    // Create Map
    mapMgr = new MapManager("examplemap_new.txt",sceneMgr);

    // Ship State
    if(collabInfo->getGameRole() == PILOT) {
        shipState = new ShipState(shipSceneNode);
        networkingManager->replicate(shipState);
    } else {
        shipState = 
            (ShipState*) networkingManager->getReplica("ShipState",true);
    }
    shipState->setX(mapMgr->startx);
    shipState->setY(0);
    shipState->setZ(mapMgr->starty);
    gameLoop->addTickable(shipState, "shipState");

    // Ship Node
    shipSceneNode = sceneNodeMgr->createNode(shipState);
    Entity *shipEntity = sceneNodeMgr->getEntity(shipState);
    if (collabInfo->getGameRole() == PILOT) {
        shipEntity->setVisible(false);
    }
    
    // Camera
    camera = createCamera(shipSceneNode);
    if(collabInfo->getGameRole() == PILOT) {
        camera->setPosition(Vector3(0,0,-5));
    } else if(collabInfo->getGameRole() == NAVIGATOR) {
        camera->setPosition(Vector3(3.5,0,0));
    } else if(collabInfo->getGameRole() == ENGINEER) {
        camera->setPosition(Vector3(-3.5,0,0));
    }
    createViewPort();

    // Collision Manager (takes 99% of our loading time)
    collisionMgr = new CollisionManager(sceneMgr,mapMgr);

    // Engineer Controls
    if(collabInfo->getGameRole() == ENGINEER) {
        engineerControls = new EngineerControls(inputState,camera);
        gameLoop->addTickable(engineerControls,"engineerControls");

        systemManager = new SystemManager(engineerControls, damageState);
        
        networkingManager->replicate(systemManager);
    } else {
        if (collabInfo->getNetworkRole() == DEVELOPMENTSERVER) {
            systemManager = new SystemManager();
        } else {
            systemManager = (SystemManager*) networkingManager->
                getReplica("SystemManager",true);
        }
    }

    // Pilot Controls
    if(collabInfo->getGameRole() == PILOT) {
        collisionMgr->addMesh(shipEntity);
        pilotControls = new PilotControls(inputState,camera);
        //last 3 terms of flying are the starting position x y z. Note mapMgr->starty = z
        flying = new Flying( pilotControls, shipState, damageState, collisionMgr, systemManager, collisions, mapMgr->startx, 0.0, mapMgr->starty  );
        gameLoop->addTickable(pilotControls,"pilotControls");
        gameLoop->addTickable(flying,"flying");
    }
    
    // Navigator Controls
    if(collabInfo->getGameRole() == NAVIGATOR) {
        navigatorControls = new NavigatorControls(inputState,camera);
        gameLoop->addTickable(navigatorControls,"navigatorControls");
    }

    // GameState
    if(collabInfo->getGameRole() == PILOT) {
        gameStateMachine = new GameStateMachine(mapMgr,shipState,damageState);
        networkingManager->replicate(gameStateMachine);    
    } else {
        gameStateMachine = 
            (GameStateMachine*) networkingManager->
                getReplica("GameStateMachine",true);
    }
    gameLoop->addTickable(gameStateMachine,"gameStateMachine");
    gameParameterMap = new GameParameterMap(gameStateMachine);

    // Print Game State changes
    printState = new PrintState(gameStateMachine);
    gameLoop->addTickable(printState,"printState");

    // Notifications
    if (collabInfo->getGameRole() == NAVIGATOR || collabInfo->getNetworkRole() == DEVELOPMENTSERVER) {
        notificationMgr = new NotificationManager(collabInfo, gameStateMachine, mapMgr, shipState, damageState);
        networkingManager->replicate(notificationMgr);
    } else {
        notificationMgr = (NotificationManager*) networkingManager->
            getReplica("NotificationManager",true);
        notificationMgr->setCollaborationInfo(collabInfo);
    }
    gameLoop->addTickable(notificationMgr,"notifications");

    // Pilot Gun State
    if(collabInfo->getGameRole() == PILOT) {
        pilotGunState = new GunState(pilotControls,damageState,systemManager,collabInfo->getGameRole());
        networkingManager->replicate(pilotGunState);
    } else if(collabInfo->getGameRole() == ENGINEER) {
        pilotGunState = (GunState*) networkingManager->
            getReplica("PilotGunState",true);
        pilotGunState->setSystemManager(systemManager);
        std::cout << "Got nav gun from net" << std::endl;
    } else {
        pilotGunState = (GunState*) networkingManager->
        getReplica("PilotGunState",true);
    }
    gameLoop->addTickable(pilotGunState,"pilotGunState");
    
    // Navigator Gun State
    if(collabInfo->getGameRole() == NAVIGATOR) {
        navigatorGunState = new GunState(navigatorControls,damageState,systemManager,collabInfo->getGameRole());
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
        engineerGunState = new GunState(engineerControls,damageState,systemManager,collabInfo->getGameRole());
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

    // TODO: start the enemies pointing towards the ship?
    // Swarm Manager
    if (collabInfo->getGameRole() == PILOT) {
        swarmMgr = new SwarmManager(sceneMgr, sceneNodeMgr, gameParameterMap, mapMgr,
            shipState,collisionMgr,networkingManager,lines,gameStateMachine,particleSystemEffectManager);
    } else {
        swarmMgr = new SwarmManager(sceneMgr, sceneNodeMgr, gameParameterMap,
            networkingManager);
    }
    gameLoop->addTickable(swarmMgr, "swarmMgr");

    // Console
    cons = new Console(sceneMgr);
    gameLoop->addTickable(cons,"console");

    // Minigame manager
    IPlayerControls *myControls;
    if (collabInfo->getGameRole() == PILOT) {
        myControls = pilotControls;
    } else if (collabInfo->getGameRole() == NAVIGATOR) {
        myControls = navigatorControls;
    } else if (collabInfo->getGameRole() == ENGINEER) {
        myControls = engineerControls;   
    }
    miniGameMgr = new MiniGameManager(cons,inputState,myControls,sceneMgr,collabInfo);
    gameLoop->addTickable(miniGameMgr,"miniGameManager");

    // Networking
    gameLoop->addTickable(networkingManager,"networkingManager");

    gameLoop->addTickable(damageState, "damageState");

    gameLoop->addTickable(particleSystemEffectManager, "psem");

    // Bullet Manager
    bulletMgr = new BulletManager(shipState,sceneMgr,pilotGunState,
        engineerGunState,navigatorGunState,collisionMgr,swarmMgr,sceneNodeMgr,
        damageState);
    gameLoop->addTickable(bulletMgr,"bulletManager");
    
    gameLoop->addTickable(systemManager,"systemManager");

    // Audio
    soundMgr = new SoundManager();
    gameLoop->addTickable(soundMgr,"soundManager");
    audioState = new AudioState(pilotGunState,soundMgr,shipSceneNode,
                                notificationMgr,bulletMgr,miniGameMgr);
    gameLoop->addTickable(audioState,"audioState");
	
    // Last class to be added to the game loop

    // CEGUI Stuff
    hud = new HUD(guiMgr, shipState,collabInfo->getGameRole(),mapMgr);
    guiStatusUpdater = new GuiStatusUpdater(guiMgr,gameLoop,damageState,navigatorControls,
                                            collabInfo->getGameRole(),systemManager,hud,
                                            flying,notificationMgr);
    gameLoop->addTickable(guiStatusUpdater,"guiStatusUpdater");
	
    // Radar GUI
    if (collabInfo->getGameRole() == ENGINEER) {
    	radarGui = new RadarGui(guiMgr, shipState, swarmMgr, hud);
    	gameLoop->addTickable(radarGui,"Radar");
	}

    // Start Rendering Loop
    
    gameLoop->startLoop();
    //root->startRendering();
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
                    ".", "FileSystem", "General");

    // Set the paths to look for various resources
    ResourceGroupManager::getSingleton().addResourceLocation(
                    "models", "FileSystem", "General");

    ResourceGroupManager::getSingleton().addResourceLocation(
                    "sounds", "FileSystem", "General");

    ResourceGroupManager::getSingleton().addResourceLocation(
                    "materials/scripts", "FileSystem", "General");

    ResourceGroupManager::getSingleton().addResourceLocation(
                    "materials/programs", "FileSystem", "General");

    ResourceGroupManager::getSingleton().addResourceLocation(
                    "materials/textures", "FileSystem", "General");
                    
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
    camera->setNearClipDistance(1);
    camera->setFarClipDistance(2500);
    
    // Lighting
    //sceneMgr->setShadowColour(ColourValue(0.5,0.5,0.5));
    
    // Add some sexy fog
    ColourValue fadeColour(0.2,0.2,0.2);
    sceneMgr->setFog(FOG_EXP, fadeColour, 0.01);
    
    Light *sp = sceneMgr->createLight("ShipLight");
    sp->setType(Light::LT_POINT);
    sp->setDiffuseColour(0.6,0.6,1.0);
    sp->setSpecularColour(0.6,0.6,1.0);
    sp->setDirection(Vector3(0,0,1));
    sp->setAttenuation( 600, 1.0, 0.007, 0.0002);

    //Light *spot = sceneMgr->createLight("shipSpot");
    //spot->setType(Light::LT_SPOTLIGHT);
    //spot->setDiffuseColour(1.0,1.0,1.0);
    //spot->setSpecularColour(1.0,1.0,1.0);
    //spot->setDirection(Vector3(0,0,1));

    //shipSceneNode->attachObject(spot);
    shipSceneNode->attachObject(sp);
    
    return camera;
}

//                ,
//          (`.  : \               __..----..__
//           `.`.| |:          _,-':::''' '  `:`-._
//             `.:\||       _,':::::'         `::::`-.
//               \\`|    _,':::::::'     `:.     `':::`.
//                ;` `-''  `::::::.                  `::\
//             ,-'      .::'  `:::::.         `::..    `:\
//           ,' /_) -.            `::.           `:.     |
//         ,'.:     `    `:.        `:.     .::.          \
//    __,-'   ___,..-''-.  `:.        `.   /::::.         |
//   |):'_,--'           `.    `::..       |::::::.      ::\
//    `-'                 |`--.:_::::|_____\::::::::.__  ::|
//                        |   _/|::::|      \::::::|::/\  :|
//                        /:./  |:::/        \__:::):/  \  :\
//                      ,'::'  /:::|        ,'::::/_/    `. ``-.__
//        will         ''''   (//|/\      ,';':,-'         `-.__  `'--..__
//                                                              `''---::::'


void Main::createViewPort() {

    Viewport *vp = window->addViewport(camera);
    vp->setBackgroundColour(ColourValue(1,0,0));
    
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
    }

    Main *main = new Main(useKey, useMouse, enemies, collisions);

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
    gameLoop->running = false;
}

