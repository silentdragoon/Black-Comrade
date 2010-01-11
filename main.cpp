
#include "main.h"
#include <iostream>
#include "stateUpdate.h"

#include "networkRole.h"
#include "collaborationInfo.h"
#include "networkingManager.h"

using namespace RakNet;

Main::Main() {

    networkingManager = new NetworkingManager(this);

    // networking
    startNetworking();

    root = new Root();
    
    if (!root->restoreConfig())
        root->showConfigDialog();
    
    window = root->initialise(true, collabInfo->getNick() + " - " + collabInfo->getGameRoleString());
    
    sceneMgr = root->createSceneManager(ST_GENERIC);
  
    // Set the paths to look for varius resources
    ResourceGroupManager::getSingleton().addResourceLocation(
                    "models", "FileSystem", "General");

    ResourceGroupManager::getSingleton().addResourceLocation(
                    ".", "FileSystem", "General");
  
    ResourceGroupManager::getSingleton().addResourceLocation(
                    "sounds", "FileSystem", "General");

    ResourceGroupManager::getSingleton().addResourceLocation(
                    "materials/scripts", "FileSystem", "General");
                    
    ResourceGroupManager::getSingleton().addResourceLocation(
                    "materials/programs", "FileSystem", "General");
                    
    ResourceGroupManager::getSingleton().addResourceLocation(
                    "materials/textures", "FileSystem", "General");
    
    // Magic Resource line
    ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    createSoundManager();
    
    mc = new MapCreate("examplemap.txt",sceneMgr);

    createCamera();
    createViewPort();
    createScene();

    ks = new KeyState(window, false, this);
    
    stateUpdate = new StateUpdate();
    stateUpdate->addTickable(networkingManager);
    
    stateUpdate->addTickable(ks);

    GameRole myRole = collabInfo->getGameRole();
    if (myRole == PILOT) {
        serverStartup();
    }
    else {
        clientStartup();
    }

    audioState = new AudioState(frontGunState,soundMgr,shipSceneNode);
    miniGameMgr = new MiniGameManager(ks,sc,sceneMgr);

    stateUpdate->addTickable(frontGunState);
    stateUpdate->addTickable(audioState);
    stateUpdate->addTickable(shipState);
    stateUpdate->addTickable(enemyState);
    stateUpdate->addTickable(soundMgr);
    stateUpdate->addTickable(miniGameMgr);
    
    enemyState->updateOgre();

    root->addFrameListener(stateUpdate);

    // Start Rendering Loop
    root->startRendering();

    networkingManager->stopNetworking();
}

void Main::clientStartup() {
    camera->setPosition(0,0,-40);
    shipState = (ShipState*) networkingManager->getReplica("ShipState",true);
    frontGunState = (FrontGunState *) networkingManager->getReplica("FrontGunState",true);
    enemyState = (EnemyState *) networkingManager->getReplica("EnemyState",true);
    
    enemyState->eSceneNode =  enemySceneNode;
    shipState->shipSceneNode = shipSceneNode;
}

void Main::serverStartup() {
    camera->setPosition(Vector3(0,0,0));
    sc = new ShipControls(ks);
    as = new AccelerationState(sc);
    ms = new MotionState(as);
    frontGunState = new FrontGunState(sc);
    shipState = new ShipState(shipSceneNode, ms);
    enemyState = new EnemyState(enemySceneNode, sceneMgr);

    networkingManager->replicate(shipState);
    networkingManager->replicate(frontGunState);
    networkingManager->replicate(enemyState);

    stateUpdate->addTickable(sc);
    stateUpdate->addTickable(as);
    stateUpdate->addTickable(ms);

    shipState->position = new Vector3(mc->startx,0,mc->starty);
    enemyState->position = new Vector3(mc->startx,0,mc->starty+500);
}

void Main::startNetworking() {
    char ch;
    printf("Start as (c)lient, (s)erver or (d)evelopment server?\n");

    while(true) {
        ch=getch();
        if (ch=='c' || ch=='C')
        {
            collabInfo = networkingManager->startNetworking(CLIENT);
            break;
        }
        else if (ch=='d' || ch=='D')
        {
            collabInfo = networkingManager->startNetworking(DEVELOPMENTSERVER);
            printf("DEVELOPMENT SERVER\n");
            break;
        }
        else if (ch=='s' || ch=='S')
        {
            collabInfo = networkingManager->startNetworking(SERVER);
            break;
        }
    }
}

void Main::createCamera() {

    shipSceneNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
    
    camera = sceneMgr->createCamera("mainCam");
    
    shipSceneNode->attachObject(camera);
    
    camera->setPosition(Vector3(0,0,0));
    camera->lookAt(Vector3(0,0,1));
    camera->setNearClipDistance(1);
    camera->setFarClipDistance(1000);
}

void Main::createViewPort() {

    Viewport *vp = window->addViewport(camera);
    vp->setBackgroundColour(ColourValue(0,0,0));
    
    camera->setAspectRatio(
        Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
}

void Main::createScene() {

    sceneMgr->setShadowColour(ColourValue(0.5,0.5,0.5));

    sceneMgr->setAmbientLight(ColourValue(0,0,0));
    //sceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);
    
    Light *l = sceneMgr->createLight("MainLight");
    l->setType(Light::LT_POINT);
    l->setDiffuseColour(0.1,0.1,0.1);
    l->setSpecularColour(0.1,0.1,0.1);

    Light *sp = sceneMgr->createLight("FrontSpot");
    sp->setType(Light::LT_SPOTLIGHT);
    sp->setDiffuseColour(0.7,0.7,0.7);
    sp->setSpecularColour(0.7,0.7,0.3);
    sp->setSpotlightRange(Degree(50), Degree(30));
    sp->setDirection(Vector3(0,0,1));
    sp->setAttenuation(10000, 0.7, 0.000025, 0.0000045);
    sp->setCastShadows(true);

    //l->setPosition(20,80,50);
    shipSceneNode->attachObject(l);
    shipSceneNode->attachObject(sp);
    
    Entity *en = sceneMgr->createEntity("enemy","smallenemy.mesh");
    
    enemySceneNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
    enemySceneNode->showBoundingBox(true);
    enemySceneNode->attachObject(en);

    mapNode = sceneMgr->getRootSceneNode()->createChildSceneNode();

    mc->outputMap(mapNode);
    
    SceneNode *modelNode = shipSceneNode->createChildSceneNode();
    
    Entity *e = sceneMgr->createEntity("robot", "ourship.mesh");
    modelNode->attachObject(e);
    //modelNode->setScale(0.15,0.15,0.15);
    modelNode->setPosition(0,-7,-5);
    //modelNode->yaw(Degree(270));
}

void Main::createSoundManager()
{
    soundMgr = new SoundManager();
}

int main() 
{   
    Main *main = new Main();
    
    delete main;
}

Main::~Main()
{
    delete ks;
    delete sc;
    delete as;
    delete ms;
    if (collabInfo->getNetworkRole() == SERVER || collabInfo->getNetworkRole() == INVISIBLESERVER) delete shipState;
    
    delete stateUpdate;
    delete networkingManager;
    
    // TODO: Fix destructing soundManager
    
    OGRE_DELETE root;
}

void Main::exit()
{
    stateUpdate->running = false;
}

