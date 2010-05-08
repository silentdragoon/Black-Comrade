#ifndef COLLISION_LIGHTMGR_H
#define COLLISION_LIGHTMGR_H

// Includes all Ogre classes
#include <OGRE/Ogre.h>
#include <vector>
#include <math.h>
#include "ITickable.h"
#include "fan.h"



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
    void attachFanBlade( Vector3 pos, int direction, SceneNode *node );


  public:
    LightAndObjectsManager(SceneManager *sceneManager);
    void addConnPieceObjsAndSPLight( Vector3 pos, int d, SceneNode *node );
    virtual void tick();
    ~LightAndObjectsManager(){}
};

#endif
