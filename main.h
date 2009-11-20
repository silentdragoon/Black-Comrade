
#ifndef MAIN_H
#define MAIN_H

// Includes all Ogre classes
#include <OGRE/Ogre.h>

#include "ExampleFrameListener.h"
#include "networkingManager.h"
#include "stateUpdate.h"
#include "shipState.h"
#include "engineState.h"
#include "motionState.h"
//#include "fixedMotionState.h"

//include networking stuff
#include <string.h>
#include <string>
#include <cstdio>



enum Topology
{
	CLIENT,
	SERVER,
} topology;


using namespace Ogre;
using namespace std;
using namespace RakNet;

class Main {
private:
    Root *root;
    Camera *camera;
    SceneManager *sceneMgr;
    RenderWindow *window;
    ExampleFrameListener *frameListener;
    NetworkingManager *networkingManager;
    
    SceneNode *robotNode;
    
    StateUpdate *stateUpdate;
    SceneNode *shipSceneNode;
    ShipState *shipState;
    
    void createCamera();
    void createScene();
    void createViewPort();
    void createFrameListener(void);
    bool startNetworking();
    
public:
    Main();
};

#endif
