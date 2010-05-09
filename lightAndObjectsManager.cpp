#include "lightAndObjectsManager.h"

LightAndObjectsManager::LightAndObjectsManager(SceneManager *sceneManager, SceneNodeManager *sceneNodeMgr, StateUpdate *gameLoop) :
    gameLoop(gameLoop)
    , sceneNodeMgr(sceneNodeMgr)
    , sceneManager(sceneManager)
{
    connPieceMainSPLight = std::vector<Light*>();
    fanBladesDir1 = std::vector<SceneNode*>();
    fanBladesDir2 = std::vector<SceneNode*>();
    angle = 0.0;
    fanAngle = 0.0;
    fanCounter = 0;
}

void LightAndObjectsManager::tick()
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


void LightAndObjectsManager::addConnPieceObjsAndSPLight( Vector3 pos, int direction, IDrawable *parent  )
{
    attachFanBlades( pos , direction, parent );
    SceneNode *lnode = sceneManager->getRootSceneNode()->createChildSceneNode();

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
    lnode->attachObject(light);
    pos.y = 23;
    lnode->setPosition(pos);
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

void LightAndObjectsManager::attachFanBlades( Vector3 pos, int direction, IDrawable *parent )
{
    Fan *fan1 = new Fan( Vector3( 10,20,-42), 0.0, parent );
    sceneNodeMgr->createNode(fan1);
    gameLoop->addTickable( fan1, "" );
    fan1 = new Fan( Vector3( -10.0, 20.0, -42.0), 0.0, parent );
    sceneNodeMgr->createNode(fan1);
    gameLoop->addTickable( fan1, "" );
    fan1 = new Fan( Vector3( -10.0, 20.0, 42.0), myPI, parent );
    sceneNodeMgr->createNode(fan1);
    gameLoop->addTickable( fan1, "" );
    fan1 = new Fan( Vector3( 10.0, 20.0, 42.0), myPI, parent );
    sceneNodeMgr->createNode(fan1);
    gameLoop->addTickable( fan1, "" );

    /* SceneNode *fnode1 = parent->createChildSceneNode();
    SceneNode *fnode2 = parent->createChildSceneNode();
    SceneNode *fnode3 = parent->createChildSceneNode();
    SceneNode *fnode4 = parent->createChildSceneNode();

    std::stringstream name;

    string fanS = "fan";
    name << fanCounter;
    fanS += name.str();
    fanCounter++;

    Entity *e = sceneManager->createEntity(fanS, "fanBlades.mesh");

    fnode1->attachObject(e);
    fnode2->attachObject(e);
    fnode3->attachObject(e);
    fnode4->attachObject(e);
    fnode1->setPosition( -10.0, 20.0, -42 );
    fnode2->setPosition( 10.0, 20.0, -42 );
    fnode3->setPosition( -10.0, 20.0, 42 );
    fnode3->yaw( Radian(myPI) );
    fnode4->setPosition( 10.0, 20.0, 42 );
    fnode3->yaw( Radian(myPI) );
    fanBladesDir1.push_back(fnode1);
    fanBladesDir1.push_back(fnode2);
    fanBladesDir1.push_back(fnode3);
    fanBladesDir1.push_back(fnode4); */
}


