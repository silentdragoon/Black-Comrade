
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
    
    audioState = new AudioState(frontGunState,soundMgr,shipSceneNode);
    
    stateUpdate = new StateUpdate();
    
    stateUpdate->addTickable(enemyState);
    stateUpdate->addTickable(ks);
    stateUpdate->addTickable(sc);
    stateUpdate->addTickable(frontGunState);
    stateUpdate->addTickable(as);
    stateUpdate->addTickable(ms);
    stateUpdate->addTickable(shipState);
    stateUpdate->addTickable(audioState);
    stateUpdate->addTickable(soundMgr);
    
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

    sceneMgr->setAmbientLight(ColourValue(0,0,0));
    //sceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);
    
    Light *l = sceneMgr->createLight("MainLight");
    l->setType(Light::LT_POINT);
    l->setDiffuseColour(0.1,0.1,0.1);
    l->setSpecularColour(0.1,0.1,0.1);

    Light *sp = sceneMgr->createLight("FrontSpot");
    sp->setType(Light::LT_SPOTLIGHT);
    sp->setDiffuseColour(0.7,0.7,0.7);
    sp->setSpecularColour(0.7,0.7,0.7);
    sp->setSpotlightRange(Degree(50), Degree(50));
    sp->setDirection(Vector3(0,0,1));

    //l->setPosition(20,80,50);
    shipSceneNode->attachObject(l);
    shipSceneNode->attachObject(sp);
    
    Entity *en = sceneMgr->createEntity("enemy","smallenemy.mesh");
    
    enemySceneNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
    enemySceneNode->showBoundingBox(true);
    enemySceneNode->attachObject(en);

    mapNode = sceneMgr->getRootSceneNode()->createChildSceneNode();

    mc->outputMap(mapNode);
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
    delete shipState;
    
    delete stateUpdate;
    
    // TODO: Fix destructing soundManager
    
    OGRE_DELETE root;
}

void Main::exit()
{
    stateUpdate->running = false;
}

