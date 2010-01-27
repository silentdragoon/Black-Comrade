#include "bulletManager.h"

// TODO: Does this contain numbers which should be constants in const.h?

void BulletManager::fire(SceneNode *bulletNode, Vector3 direction,string bullName, string rname, double distance) {

    Bullet *b = new Bullet(bulletNode,sceneMgr,bullName,rname,direction,50,distance);
    
    activeBullets->push_back(b);
}

void BulletManager::updateBullets() {
    for(int i=0;i<activeBullets->size();i++) {
        Bullet *b = activeBullets->at(i);
        b->updateLocation();
        if(b->distanceTravelled>b->distanceToTravel) {
            delete b;
            activeBullets->erase(activeBullets->begin()+(i));
        }
    }
}

BulletManager::BulletManager(SceneNode *shipSceneNode,SceneManager *sceneMgr, FrontGunState *gunState, CollisionManager *colMgr) 
    : shipSceneNode(shipSceneNode)
    , sceneMgr(sceneMgr)
    , gunState(gunState)
    , colMgr(colMgr)
    , bnum(0)
{
    activeBullets = new vector<Bullet*>();
}

BulletManager::~BulletManager() {
    cout << "...activeBullets" << endl;
    delete activeBullets;
}

void BulletManager::tick()
{
    // Firing the pilots gun
    if(gunState->fire()) {
        string bullName = "Bullet";
        string bname = "Bill";
        string lname = "Light";
        string rname = "Ribbon";
        stringstream out;
        out << bnum++;
        bname += out.str();
        lname += out.str();
        bullName += out.str();
        rname += out.str();

        SceneNode *bulletNode = sceneMgr->getRootSceneNode()->createChildSceneNode(bullName);
        Vector3 sp = shipSceneNode->getPosition();
        sp.y = sp.y - 3.0;
        bulletNode->setPosition(sp);

        Quaternion orient = shipSceneNode->getOrientation();
        Vector3 direction = orient.zAxis();

        BillboardSet *bbbs = sceneMgr->createBillboardSet(bname,1);
        bbbs->setMaterialName("PE/Streak");
        Billboard *bbb = bbbs->createBillboard(0,0,0,ColourValue(1.0,0.7,0.0));
        bbb->setDimensions(0.7,0.7);

        RibbonTrail *trail = sceneMgr->createRibbonTrail(rname);
        trail->setMaterialName("PE/LightRibbonTrail");
        trail->setTrailLength(75);
        trail->setMaxChainElements(400);
        trail->setInitialColour(0,1.0,0.7,0.0);
        trail->setInitialWidth(0,0.7);
        trail->addNode(bulletNode);
        sceneMgr->getRootSceneNode()->attachObject(trail);

        Light *l = sceneMgr->createLight(lname);
        l->setType(Light::LT_POINT);
        l->setDiffuseColour(ColourValue(0.7f,0.4f,0.0f));
        l->setSpecularColour(ColourValue(0.7f,0.4f,0.0f));
        l->setAttenuation(100,0.5,0.0005,0);

        bulletNode->attachObject(bbbs);
        bulletNode->attachObject(l);

        Vector3 *pos = new Vector3(sp.x,sp.y,sp.z);

        double t = colMgr->getRCMapDist(pos,&direction);
        
        // FIRE THE BULLET!
        fire(bulletNode,direction,bullName,rname,t);
    }
    // TODO: Add stuff like the thing above here for the other guns or enemies
    updateBullets();
}
