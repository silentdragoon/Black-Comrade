
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

    soundMgr = new SoundManager();
    
    mc = new MapCreate("examplemap.txt",sceneMgr);

    createCamera();
    createViewPort();
    createScene();
    
    ks = new KeyState(window, false, this);
    
    stateUpdate = new StateUpdate();
    
    stateUpdate->addTickable(ks);
    
    collisionMgr = new CollisionManager( sceneMgr, mc );

    GameRole myRole = collabInfo->getGameRole();
    if (myRole == PILOT) {
        serverStartup();
    }
    else {
        clientStartup();
    }

    bulletMgr = new BulletManager(shipSceneNode,sceneMgr,frontGunState);
    audioState = new AudioState(frontGunState,soundMgr,shipSceneNode);
    miniGameMgr = new MiniGameManager(ks,sc,sceneMgr);

    stateUpdate->addTickable(frontGunState);
    stateUpdate->addTickable(audioState);
    stateUpdate->addTickable(shipState);
    stateUpdate->addTickable(enemyState);
    stateUpdate->addTickable(networkingManager);
    stateUpdate->addTickable(bulletMgr);
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
    shipState = new ShipState(shipSceneNode, ms, collisionMgr);
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
    camera->setFOVy(Radian(2.0943951));
    camera->setNearClipDistance(1);
    camera->setFarClipDistance(1500);
}

void Main::createViewPort() {

    Viewport *vp = window->addViewport(camera);
    vp->setBackgroundColour(ColourValue(0,0,0));
    
    camera->setAspectRatio(
        Real(vp->getActualWidth()) / Real(vp->getActualHeight()*1.17));
    //camera->setAspectRatio(1.17);
}

void Main::createScene() {

    sceneMgr->setShadowColour(ColourValue(0.5,0.5,0.5));

    sceneMgr->setAmbientLight(ColourValue(1,1,1));
    //sceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_MODULATIVE);
    
    // Add some sexy fog
    ColourValue fadeColour(0.1,0.1,0.1);
    sceneMgr->setFog(FOG_LINEAR, fadeColour, 0.0, 0, 300);
    
    // Creating the light that is attached to the ship
    Light *sp = sceneMgr->createLight("ShipLight");
    sp->setType(Light::LT_POINT);
    sp->setDiffuseColour(0.2,0.2,0.4);
    sp->setSpecularColour(0.2,0.2,0.7);
    sp->setDirection(Vector3(0,0,1));
    sp->setAttenuation(10000, 0.7, 0.000025, 0.0000045);

    shipSceneNode->attachObject(sp);
    
    Entity *en = sceneMgr->createEntity("enemy","smallenemy.mesh");
    
    enemySceneNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
    enemySceneNode->showBoundingBox(true);
    enemySceneNode->attachObject(en);

    mapNode = sceneMgr->getRootSceneNode()->createChildSceneNode();

    mc->outputMap(mapNode);
    
    SceneNode *modelNode = shipSceneNode->createChildSceneNode();
    
    Entity *e = sceneMgr->createEntity("ourship", "ourship.mesh");
    modelNode->attachObject(e);
    modelNode->setPosition(0,-7,-5);
}

int main()
{
    Main *main = new Main();

    delete main;
}

Main::~Main()
{
    cout << "deleting ks" << endl;
    delete ks;
    cout << "deleting sc" << endl;
    delete sc;
    cout << "deleting as" << endl;
    delete as;
    cout << "deleting ms" << endl;
    delete ms;
    cout << "deleteing bulletMgr" << endl;
    delete bulletMgr;
    if (collabInfo->getNetworkRole() == SERVER || collabInfo->getNetworkRole() == INVISIBLESERVER) {
        cout << "deleting shipstate" << endl;
        delete shipState;
        cout << "deleting frontGunState" << endl;
        delete frontGunState;
    }
    cout << "deleting networkingManger" << endl;
    delete networkingManager;

    cout << "deleting stateUpdate" << endl;
    delete stateUpdate;

    delete soundMgr;

    OGRE_DELETE root;

    //cout << "PLEASE STOP HERE!" << endl;
}

void Main::exit()
{
    stateUpdate->running = false;
}

