
#ifndef MAIN_H
#define MAIN_H

// Includes all Ogre classes
#include <OGRE/Ogre.h>

#include "networkingManager.h"
#include "stateUpdate.h"
#include "shipState.h"
#include "keyState.h"
#include "accelerationState.h"
#include "motionState.h"
#include "IExit.h"

//include networking stuff
#include <string.h>
#include <string>
#include <cstdio>

using namespace Ogre;
using namespace std;
using namespace RakNet;

class Main : public IExit {
private:
    Root *root;
    Camera *camera;
    SceneManager *sceneMgr;
    RenderWindow *window;

    NetworkingManager *networkingManager;
    bool isServer;
    
    KeyState *ks;
    AccelerationState *as;
    MotionState *ms;
    
    SceneNode *robotNode;
    
    StateUpdate *stateUpdate;
    SceneNode *shipSceneNode;
    ShipState *shipState;
    
    void createCamera();
    void createScene();
    void createViewPort();
    bool startNetworking();
    
public:
    Main();
    ~Main();
    
    void exit();
};

#endif
