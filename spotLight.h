#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H

// Includes all Ogre classes
#include <OGRE/Ogre.h>

#include "ITickable.h"
#include "gunState.h"

using namespace Ogre;

class SpotLight : public ITickable {
private:
    Light *light;
    GunState *gunState;
    
    SceneNode *sceneNode;
    SceneNode *shipSceneNode;
    
public:
   
    void tick();

    SpotLight(SceneManager *sceneMgr, SceneNode *shipSceneNode, 
        GunState *gunState);
};

#endif
