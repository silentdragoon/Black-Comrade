#include "lightManager.h"

LightManager::LightManager(SceneManager *sceneManager) :
    sceneManager(sceneManager)
{
    connPieceMainSPLight = std::vector<Light*>();
    angle = 0.0;
}

void LightManager::tick()
{
    /*
    double x = sin(angle);
    double z = cos(angle);
    for(std::vector<Light*>::const_iterator it=connPieceMainSPLight.begin();it!=connPieceMainSPLight.end(); ++it)
    {
        Light *light = *it;
        light->setDirection( x, -0.2, z );
    }
    angle += 0.03;
     */
}


void LightManager::addConnPieceSPLight( Vector3 pos )
{
    SceneNode *node = sceneManager->getRootSceneNode()->createChildSceneNode();

    std::stringstream out;
    out << "-" << pos.x << "-" << pos.z;

    string lightS = "light";
    lightS += out.str();

    Light* light = sceneManager->createLight(lightS);
    light->setType(Light::LT_SPOTLIGHT);
    light->setDiffuseColour(1.0,1.0,1.0);
    light->setSpecularColour(1.0,1.0,1.0);
    //light->setAttenuation( 100, 1.0, 0.045, 0.0075);
    light->setSpotlightInnerAngle(Radian(Degree(5)));
    light->setSpotlightOuterAngle(Radian(Degree(100)));
    light->setSpotlightFalloff(40.0);
    light->setDirection(Vector3(0.0, -1.0, 0.0));
    //light->setSpotlightRange(Ogre::Degree(20), Ogre::Degree(60), 1.2);
    //light->setDirection(Vector3::NEGATIVE_UNIT_Y);
    node->attachObject(light);
    pos.y = 23;
    node->setPosition(pos);
    /*
    lightS += "2";
    Light *light2 = sceneManager->createLight(lightS);
    light2->setType(Light::LT_SPOTLIGHT);
    light2->setDiffuseColour(ColourValue(25.25f,0.0,0.0));
    light2->setSpecularColour(ColourValue(25.25f,0.0,0.0));
    //light->setAttenuation( 100, 1.0, 0.045, 0.0075);
    light2->setSpotlightInnerAngle(Radian(Degree(4)));
    light2->setSpotlightOuterAngle(Radian(Degree(60)));
    light2->setSpotlightFalloff(20.0);
    light2->setDirection(Vector3(0.0, -1.0, 0.0));
    node->attachObject(light2);
    connPieceMainSPLight.push_back(light2);
     */
}
