#ifndef COLLISION_LIGHTMGR_H
#define COLLISION_LIGHTMGR_H

// Includes all Ogre classes
#include <OGRE/Ogre.h>
#include <iostream>
#include <vector>
#include <math.h>
#include "ITickable.h"
#include "fan.h"
#include "sceneNodeManager.h"
#include "stateUpdate.h"

using namespace Ogre;
using namespace std;

class LightAndObjectsManager : public ITickable
{
  private:
    int fanCounter;
    static const double myPI = 3.141592;
    std::vector<Light*> connPieceMainSPLight;
    double angle;
    SceneManager *sceneManager;
    std::vector<SceneNode*> fanBladesDir1;
    std::vector<SceneNode*> fanBladesDir2;
    double fanAngle;
    void attachFanBlades( Vector3 pos, int direction, IDrawable *parent );
    SceneNodeManager *sceneNodeMgr;
    StateUpdate *gameLoop;

	void attachConLight(Vector3 pos, 
		Vector3 direction, IDrawable *parent);

  public:
    LightAndObjectsManager(SceneManager *sceneManager, SceneNodeManager *scenceNodeMgr, StateUpdate *gameLoop);
    void addConnPieceObjsAndSPLight( Vector3 pos, int d, IDrawable *parent );
    virtual void tick();
    ~LightAndObjectsManager(){}
};

#endif
