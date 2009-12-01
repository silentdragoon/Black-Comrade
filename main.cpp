
#include "main.h"
#include <iostream>
#include "stateUpdate.h"

#include "networkingManager.h"

using namespace RakNet;

Main::Main() {

    networkingManager = new NetworkingManager(this);

    // networking
    isServer = startNetworking();

    root = new Root();
    
    if (!root->restoreConfig())
        root->showConfigDialog();
    
    window = root->initialise(true, "Test Window");
    
    sceneMgr = root->createSceneManager(ST_GENERIC);
  
    // Set the paths to look for varius resources
    ResourceGroupManager::getSingleton().addResourceLocation(
                    "models", "FileSystem", "General");

    ResourceGroupManager::getSingleton().addResourceLocation(
                    ".", "FileSystem", "General");
  
    ResourceGroupManager::getSingleton().addResourceLocation(
                    "materials/scripts", "FileSystem", "General");
                    
    ResourceGroupManager::getSingleton().addResourceLocation(
                    "materials/programs", "FileSystem", "General");
                    
    ResourceGroupManager::getSingleton().addResourceLocation(
                    "materials/textures", "FileSystem", "General");
    
    // Magic Resource line
    ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    
    //createFrameListener();
    
    mc = new MapCreate("examplemap.txt",sceneMgr);
    
    createCamera();
    createViewPort();
    createScene();
    

    //createFrameListener();
    
    ks = new KeyState(window, false, this);
    
    stateUpdate = new StateUpdate();

    stateUpdate->addTickable(networkingManager);
    stateUpdate->addTickable(ks);

    if (isServer) {
        serverStartup();
    }
    else {
        clientStartup();
    }

    audioState = new AudioState(frontGunState);

    stateUpdate->addTickable(frontGunState);
    stateUpdate->addTickable(audioState);
    stateUpdate->addTickable(shipState);

    root->addFrameListener(stateUpdate);
    
    // Start Rendering Loop
    root->startRendering();

    networkingManager->stopNetworking();
}

void Main::clientStartup() {
    camera->setPosition(0,0,-40);
    shipState = (ShipState*) networkingManager->getReplica("ShipState",true);
    frontGunState = (FrontGunState *) networkingManager->getReplica("FrontGunState",true);
    shipState->shipSceneNode = shipSceneNode;
}

void Main::serverStartup() {
    camera->setPosition(Vector3(0,0,0));
    sc = new ShipControls(ks);
    as = new AccelerationState(sc);
    ms = new MotionState(as);
    frontGunState = new FrontGunState(sc);
    shipState = new ShipState(shipSceneNode, ms);

    networkingManager->replicate(shipState);
    networkingManager->replicate(frontGunState);

    stateUpdate->addTickable(sc);
    stateUpdate->addTickable(as);
    stateUpdate->addTickable(ms);

    shipState->position = new Vector3(mc->startx,0,mc->starty);
}

bool Main::startNetworking() {
    char ch;
    printf("Start as (c)lient, (s)erver?\n");
    ch=getch();
    bool isActuallyServer;

    if (ch=='c' || ch=='C')
    {
        isActuallyServer = networkingManager->startNetworking(false);
    }
    else
    {
        isActuallyServer = networkingManager->startNetworking(true);
    }

    return isActuallyServer;
}

void Main::createCamera() {

    shipSceneNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
    
    camera = sceneMgr->createCamera("mainCam");
    
    shipSceneNode->attachObject(camera);
    
    //camera->setPosition(Vector3(0,0,-50));
    camera->lookAt(Vector3(0,0,1));
    camera->setNearClipDistance(5);
    camera->setFarClipDistance(10000);
}

void Main::createViewPort() {

    Viewport *vp = window->addViewport(camera);
    vp->setBackgroundColour(ColourValue(100,0,0));
    
    camera->setAspectRatio(
        Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
}

void Main::createScene() {

    sceneMgr->setAmbientLight(ColourValue(0.1,0.1,0.1));
    
    Light *l = sceneMgr->createLight("MainLight");
    
    l->setPosition(20,80,50);
    
    //Entity *e = sceneMgr->createEntity("object","testmap.mesh");
    
    //e->setMaterialName("Examples/EnvMappedRustySteel");
    
    mapNode = sceneMgr->getRootSceneNode()->createChildSceneNode();

    mc->outputMap(mapNode);
    
    SceneNode *modelNode = shipSceneNode->createChildSceneNode();
    
    Entity *e = sceneMgr->createEntity("robot", "ourship.mesh");
    modelNode->attachObject(e);
    //modelNode->setScale(0.15,0.15,0.15);
    modelNode->setPosition(0,-7,-5);
    //modelNode->yaw(Degree(270));
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
    if (isServer) delete shipState;
    
    delete stateUpdate;
    delete networkingManager;

    OGRE_DELETE root;
}

void Main::exit()
{
    stateUpdate->running = false;
}

