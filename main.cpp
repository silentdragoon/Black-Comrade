
#include "main.h"
#include <iostream>
#include "stateUpdate.h"

#include "networkingManager.h"

using namespace RakNet;

Main::Main() {

    networkingManager = new NetworkingManager(this);

    shipState = new ShipState();

    isServer = startNetworking();
   
    root = new Root();
    
    if (!root->restoreConfig())
        root->showConfigDialog();

    // networking
    
    window = root->initialise(true, "Test Window");
    
    sceneMgr = root->createSceneManager(ST_GENERIC);
  
    // Set the paths to look for varius resources
    ResourceGroupManager::getSingleton().addResourceLocation(
                    "models", "FileSystem", "General");
  
    ResourceGroupManager::getSingleton().addResourceLocation(
                    "materials/scripts", "FileSystem", "General");
                    
    ResourceGroupManager::getSingleton().addResourceLocation(
                    "materials/programs", "FileSystem", "General");
                    
    ResourceGroupManager::getSingleton().addResourceLocation(
                    "materials/textures", "FileSystem", "General");
    
    // Magic Resource line
    ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    
    createCamera();
    createViewPort();
    createScene();
    
    if(!isServer) camera->setPosition(0,0,-40);
    
    //createFrameListener();
    
    ks = new KeyState(window, false, this);    
    
    if (isServer)
    {
        sc = new ShipControls(ks);
        as = new AccelerationState(sc);
        ms = new MotionState(as);
        frontGunState = new FrontGunState(sc);
        audioState = new AudioState(frontGunState);
        shipState = new ShipState(shipSceneNode, ms);
        networkingManager->replicate(shipState);
    }
    else
    {
        shipState = (ShipState*) networkingManager->getReplica(0,true);
        shipState->shipSceneNode = shipSceneNode;
    }

    stateUpdate = new StateUpdate();

    stateUpdate->addTickable(networkingManager);
    stateUpdate->addTickable(ks);

    if (isServer)
    {
        stateUpdate->addTickable(sc);
        stateUpdate->addTickable(as);
        stateUpdate->addTickable(ms);
        stateUpdate->addTickable(audioState);
        stateUpdate->addTickable(frontGunState);
    }


    stateUpdate->addTickable(shipState);

    
    root->addFrameListener(stateUpdate);
    
    // Start Rendering Loop
    root->startRendering();

    networkingManager->stopNetworking();
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
    else if (ch=='s' || ch=='S')
    {
        isActuallyServer = networkingManager->startNetworking(true);
    }

    return isActuallyServer;
}

void Main::createCamera() {

    shipSceneNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
    
    camera = sceneMgr->createCamera("mainCam");
    
    shipSceneNode->attachObject(camera);
    
    //camera->setPosition(Vector3(0,0,50));
    camera->lookAt(Vector3(0,0,1));
    camera->setNearClipDistance(5);
    camera->setFarClipDistance(1000);
}

void Main::createViewPort() {

    Viewport *vp = window->addViewport(camera);
    vp->setBackgroundColour(ColourValue(100,0,0));
    
    camera->setAspectRatio(
        Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
}

void Main::createScene() {

    sceneMgr->setAmbientLight(ColourValue(0.5,0.5,0.5));
    
    Light *l = sceneMgr->createLight("MainLight");
    
    l->setPosition(20,80,50);
    
    Entity *e = sceneMgr->createEntity("object","testmap.mesh");
    
    //e->setMaterialName("Examples/EnvMappedRustySteel");
    
    robotNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
    robotNode->attachObject(e);
    
    SceneNode *modelNode = shipSceneNode->createChildSceneNode();
    
    e = sceneMgr->createEntity("robot", "ourship.mesh");
    modelNode->attachObject(e);
    //modelNode->setScale(0.15,0.15,0.15);
    modelNode->setPosition(0,-7,-5);
    //modelNode->yaw(Degree(270));
    
    
    robotNode->yaw(Ogre::Radian(4.712));
}

int main() {
    
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

