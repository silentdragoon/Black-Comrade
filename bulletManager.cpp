#include "bulletManager.h"

void BulletManager::fire(SceneNode *bulletNode, Vector3 direction) {
    
    Bullet *b = new Bullet(bulletNode,direction,15,false,false);
    
    activeBullets.push_back(b);
}

void BulletManager::updateBullets() {
    for(int i=0;i<activeBullets.size();i++) {
        Bullet *b = activeBullets.at(i);
        b->updateLocation();
        // TODO: Test for a collision/Destruction here
        if(b->aliveTicks>2000) {
            // TODO: This doesnt work and is retarded completely
            delete b;
        }
    }
}

BulletManager::BulletManager(SceneNode *shipSceneNode,SceneManager *sceneMgr, FrontGunState *gunState) 
    : shipSceneNode(shipSceneNode)
    , sceneMgr(sceneMgr)
    , gunState(gunState)
    , bnum(0)
{}

void BulletManager::tick()
{
    if(gunState->fire()) {

        // Making a new bullet
        string bname = "Bullet";
        string lname = "Light";
        stringstream out;
        out << bnum++;
        bname += out.str();
        lname += out.str();

        SceneNode *bulletNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
        bulletNode->setPosition(shipSceneNode->getPosition());

        // TODO: Get the direction the ship is pointing in first then pass it to the fire method
        Quaternion orient = shipSceneNode->getOrientation();
        Vector3 direction = orient.zAxis();


        BillboardSet *bbbs = sceneMgr->createBillboardSet(bname,1);
        bbbs->setMaterialName("PE/Streak");
        Billboard *bbb = bbbs->createBillboard(0,0,0,ColourValue(0.0,0.7,0.0f));
        bbb->setDimensions(3.0,3.0);

        Light *l = sceneMgr->createLight(lname);
        l->setType(Light::LT_POINT);
        l->setDiffuseColour(ColourValue(0.0f,0.1f,0.0f));
        l->setSpecularColour(ColourValue(0.0f,0.1f,0.0f));
        l->setAttenuation(100,0.5,0.0005,0);

        bulletNode->attachObject(bbbs);
        bulletNode->attachObject(l);
        
        // FIRE THE BULLET!
        fire(bulletNode,direction);
    }
    updateBullets();
}
