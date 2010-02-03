
#include "main.h"
#include <iostream>
#include "stateUpdate.h"

#include "networkRole.h"
#include "collaborationInfo.h"
#include "networkingManager.h"

using namespace RakNet;

Main::Main() {

    // networking
    networkingManager = new NetworkingManager(this);
    collabInfo = runLoby(networkingManager);

	// Start Ogre
    root = configRoot();
    sceneMgr = root->createSceneManager(ST_GENERIC);
    window = root->initialise(true, "I removed what was once here");
    
    configResources();
    
    // Ship Node
    shipSceneNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
    Entity *shipEntity = sceneMgr->createEntity("ourship", "ourship.mesh");
    shipSceneNode->attachObject(shipEntity);
    
    // Camera
    camera = createCamera(shipSceneNode);
    if(collabInfo->getGameRole() == PILOT) {
    	camera->setPosition(Vector3(0,7,5));
    }
    createViewPort();
    
    // Create Map
    mapMgr = new MapManager("examplemap.txt",sceneMgr);
    mapMgr->outputMap(sceneMgr->getRootSceneNode());
    
    // Collision Manager
	collisionMgr = new CollisionManager(sceneMgr,mapMgr);
    
    // Game Loop
    gameLoop = new StateUpdate();
    root->addFrameListener(gameLoop);
    
    // User Input
    inputState = new InputState(window, false, this,true,false);
    gameLoop->addTickable(inputState);
    
    // Navigator Controls
    if(collabInfo->getGameRole() == NAVIGATOR) {
	    navControls = new NavigatorControls(inputState,camera);
	    gameLoop->addTickable(navControls);
    }
    
    // Pilot --- Flying 1.0 ---
    if(collabInfo->getGameRole() == PILOT) {
	    pilotControls = new PilotControls(inputState);
	    accelerationState = new AccelerationState(pilotControls);
	    motionState = new MotionState(accelerationState);
	    gameLoop->addTickable(pilotControls);
	    gameLoop->addTickable(accelerationState);
	    gameLoop->addTickable(motionState);
    }
    
    // Ship State
    if(collabInfo->getGameRole() == PILOT) {
	    shipState = new ShipState(shipSceneNode,motionState);
	    networkingManager->replicate(shipState);
    } else {
    	shipState = 
    		(ShipState*) networkingManager->getReplica("ShipState",true);
    	shipState->shipSceneNode = shipSceneNode;
    }
    shipState->position = new Vector3(mapMgr->startx,0,mapMgr->starty);
    gameLoop->addTickable(shipState);

	// Front Gun State
	if(collabInfo->getGameRole() == PILOT) {
	    frontGunState = new FrontGunState(pilotControls);
	    networkingManager->replicate(frontGunState);
    } else {
    	frontGunState = 
    		(FrontGunState*) networkingManager->getReplica("FrontGunState",true);
    }
    gameLoop->addTickable(frontGunState);

	// Bullet Manager
	if(collabInfo->getGameRole() == PILOT) {
	    bulletMgr = new BulletManager(shipSceneNode,sceneMgr,frontGunState,
	    	collisionMgr, NULL);
	    //networkingManager->replicate(bulletMgr);
	    gameLoop->addTickable(bulletMgr);
    } else {
    	//bulletMgr = 
    	//	(BulletManager*) networkingManager->getReplica("BulletManager",true);
    }
    

	// Last class to be added to the game loop
    gameLoop->addTickable(networkingManager);
    
    // Start Rendering Loop
    root->startRendering();
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
                    ".", "FileSystem", "General");

    // Set the paths to look for various resources
    ResourceGroupManager::getSingleton().addResourceLocation(
                    "models", "FileSystem", "General");

    ResourceGroupManager::getSingleton().addResourceLocation(
                    "sounds", "FileSystem", "Genescenemral");

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
}

CollaborationInfo *Main::runLoby(NetworkingManager *networkingManager) {
    
    CollaborationInfo *collabInfo;
    
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
    
    return collabInfo;
}

Camera *Main::createCamera(SceneNode *shipSceneNode) {

    Camera *camera = sceneMgr->createCamera("mainCam");

    shipSceneNode->attachObject(camera);

    camera->setPosition(Vector3(0,0,0));
    camera->lookAt(Vector3(0,0,1));
    //camera->setFOVy(Radian(2.0943951));
    camera->setNearClipDistance(1);
    camera->setFarClipDistance(1500);
    
    // Lighting
    //sceneMgr->setShadowColour(ColourValue(0.5,0.5,0.5));

    //sceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_MODULATIVE);
    
    // Add some sexy fog
    //ColourValue fadeColour(0.1,0.1,0.1);
    //sceneMgr->setFog(FOG_LINEAR, fadeColour, 0.0, 0, 300);
    
    Light *sp = sceneMgr->createLight("ShipLight");
    sp->setType(Light::LT_POINT);
    sp->setDiffuseColour(1.0,1.0,1.0);
    sp->setSpecularColour(0.2,0.2,0.7);
    sp->setDirection(Vector3(0,0,1));
    //sp->setAttenuation(10000, 0.7, 0.000025, 0.0000045);

    shipSceneNode->attachObject(sp);
    
    return camera;
}

void Main::createViewPort() {

    Viewport *vp = window->addViewport(camera);
    vp->setBackgroundColour(ColourValue(1,0,0));
    
    camera->setAspectRatio(
        Real(vp->getActualWidth()) / Real(vp->getActualHeight()*1.17));
    //camera->setAspectRatio(1.17);
}

int main()
{
    Main *main = new Main();

    delete main;
}

Main::~Main()
{
}

void Main::exit()
{
    gameLoop->running = false;
}

