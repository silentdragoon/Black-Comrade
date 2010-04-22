
#include "spotLight.h"

SpotLight::SpotLight(SceneManager *sceneMgr, SceneNode *shipSceneNode, 
                                                    GunState *gunState)
    : gunState(gunState)
    , shipSceneNode(shipSceneNode)
{
    light = sceneMgr->createLight();
    light->setType(Light::LT_SPOTLIGHT);
    light->setDiffuseColour(1.0,1.0,1.0);
    light->setSpecularColour(1.0,1.0,1.0);
    light->setDirection(Vector3(0,0,-1));
    light->setSpotlightInnerAngle(Radian(Degree(5)));
    light->setSpotlightOuterAngle(Radian(Degree(100)));
    light->setSpotlightFalloff(40.0);
    
    sceneNode = shipSceneNode->createChildSceneNode();
    
    sceneNode->attachObject(light);
}

void SpotLight::tick()
{
    cout << gunState->getBulletDirection() << endl;

    sceneNode->lookAt(gunState->getBulletDirection() 
        + shipSceneNode->getPosition(), 
        Ogre::Node::TS_WORLD);
}

