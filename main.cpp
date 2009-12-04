
#include "main.h"
#include <iostream>
#include "stateUpdate.h"

Main::Main() {

    root = new Root();

    if (!root->restoreConfig())
        root->showConfigDialog();

    window = root->initialise(true, "Test Window");

    sceneMgr = root->createSceneManager(ST_GENERIC);

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
    as = new AccelerationState(sc);
    ms = new MotionState(as);
    shipState = new ShipState(shipSceneNode, ms);
    frontGunState = new FrontGunState(sc);
    bullet = new Bullet(shipSceneNode,sceneMgr,frontGunState);
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
    stateUpdate->addTickable(bullet);

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

    sceneMgr->setAmbientLight(ColourValue(0.5,0.5,0.5));

    Light *l = sceneMgr->createLight("MainLight");

    l->setPosition(20,80,50);

    Entity *en = sceneMgr->createEntity("enemy","smallenemy.mesh");
    
    enemySceneNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
    enemySceneNode->showBoundingBox(true);
    enemySceneNode->attachObject(en);

    //Entity *e = sceneMgr->createEntity("object","testmap.mesh");

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
    delete bullet;

    delete stateUpdate;

    OGRE_DELETE root;
}

void Main::exit()
{
    stateUpdate->running = false;
}

