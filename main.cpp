
#include "main.h"
#include <iostream>
#include "stateUpdate.h"

Main::Main() {
    
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
    sc = new ShipControls(ks);
    frontGunState = new FrontGunState(sc);
    as = new AccelerationState(sc);
    ms = new MotionState(as);
    shipState = new ShipState(shipSceneNode, ms);
    
    enemyState = new EnemyState( enemySceneNode, sceneMgr );
    
    audioState = new AudioState(frontGunState);
    
    stateUpdate = new StateUpdate();
    
    stateUpdate->addTickable(enemyState);
    stateUpdate->addTickable(ks);
    stateUpdate->addTickable(sc);
    stateUpdate->addTickable(frontGunState);
    stateUpdate->addTickable(as);
    stateUpdate->addTickable(ms);
    stateUpdate->addTickable(shipState);
    stateUpdate->addTickable(audioState);
    
    shipState->position = new Vector3(mc->startx,0,mc->starty);
    enemyState->position = new Vector3(mc->startx,0,mc->starty+500);
    //enemyState->yaw = Degree(90);
    enemyState->updateOgre();
    root->addFrameListener(stateUpdate);
    
    // Start Rendering Loop
    root->startRendering();
}

void Main::createCamera() {

    shipSceneNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
    
    camera = sceneMgr->createCamera("mainCam");
    
    shipSceneNode->attachObject(camera);
    
    camera->setPosition(Vector3(0,0,-50));
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
    
    Entity *en = sceneMgr->createEntity("enemy","smallenemy.mesh");
    
    enemySceneNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
    enemySceneNode->showBoundingBox(true);
    enemySceneNode->attachObject(en);

    mapNode = sceneMgr->getRootSceneNode()->createChildSceneNode();

    mc->outputMap(mapNode);
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
    delete shipState;
    
    delete stateUpdate;
    
    OGRE_DELETE root;
}

void Main::exit()
{
    stateUpdate->running = false;
}

