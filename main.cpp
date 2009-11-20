
#include "main.h"

#include "stateUpdate.h"

#include "networkingManager.h"

using namespace RakNet;

Main::Main() {

    networkingManager = new NetworkingManager();

    shipState = new ShipState();

    startNetworking();
   
    root = new Root();
    
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
    
    createFrameListener();
    
    //EngineState *es = new EngineState(window);
    //MotionState *ms = new MotionState(es);
    //shipState = new ShipState(shipSceneNode, ms);
    shipState = (ShipState*) networkingManager->replicaManager.GetReplicaAtIndex(0);
    shipState->shipSceneNode = shipSceneNode;
    
    stateUpdate = new StateUpdate();
    stateUpdate->addTickable(networkingManager);
    //stateUpdate->addTickable(es);
    //stateUpdate->addTickable(ms);
    stateUpdate->addTickable(shipState);
    
    root->addFrameListener(stateUpdate);
    
    // Start Rendering Loop
    root->startRendering();
}

void Main::startNetworking() {
    char ch;
    printf("Start as (c)lient, (s)erver?\n");
    ch=getch();

    if (ch=='c' || ch=='C')
    {
        networkingManager->startNetworking(false);
    }
    else if (ch=='s' || ch=='S')
    {
        networkingManager->startNetworking(true);
    }
    while(networkingManager->replicaManager.GetReplicaCount() == 0)
    {
        networkingManager->tick();
    }
    printf("Hello\n");
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
    
    robotNode->yaw(Ogre::Radian(4.712));
}

void Main::createFrameListener(void)
    {
        frameListener= new ExampleFrameListener(window, camera);
        frameListener->showDebugOverlay(true);
        root->addFrameListener(frameListener);
    }

int main() {
    
    Main *main = new Main();
}
