#include "bulletManager.h"
#include "const.h"


// TODO: Does this contain numbers which should be constants in const.h?

void BulletManager::fire(Vector3 origin, Vector3 direction, ColourValue c) 
{
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
    bulletNode->setPosition(origin);

    BillboardSet *bbbs = sceneMgr->createBillboardSet(bname,1);
    bbbs->setMaterialName("PE/Streak");
    Billboard *bbb = bbbs->createBillboard(0,0,0,c);
    bbb->setDimensions(0.7,0.7);

    RibbonTrail *trail = sceneMgr->createRibbonTrail(rname);
    trail->setMaterialName("PE/LightRibbonTrail");
    trail->setTrailLength(75);
    trail->setMaxChainElements(400);
    trail->setInitialColour(0,c);
    trail->setInitialWidth(0,0.7);
    trail->addNode(bulletNode);
    sceneMgr->getRootSceneNode()->attachObject(trail);

    Light *l = sceneMgr->createLight(lname);
    l->setType(Light::LT_POINT);
    l->setDiffuseColour(c);
    l->setSpecularColour(c);
    l->setAttenuation(100,0.5,0.0005,0);

    bulletNode->attachObject(bbbs);
    bulletNode->attachObject(l);

    Vector3 *pos = new Vector3(origin.x,origin.y,origin.z);

    double t = colMgr->getRCMapDist(pos,&direction);
    if(t<0) t=10000;

	bool isEnemy = false;
    Enemy *hurtEnemy = NULL;
    if(swarmMgr) {
	    vector<Enemy*> ents = swarmMgr->getAllEnemies();
	    Enemy *e;
	    for(vector<Enemy*>::const_iterator it=ents.begin();it!=ents.end();++it) {
	        e = *it;
	        double temp = colMgr->rayCollideWithTransform(pos,&direction,e->getEntity());
	        if(temp<t && temp > 0.0) {
	        	t = temp;
	        	isEnemy = true;
	        	hurtEnemy = e;
	        }
		}
    }

    //cout << t << endl;
    
    // FIRE THE BULLET!
    Bullet *b = new Bullet(bulletNode,sceneMgr,bullName,rname,direction,
    	Const::FRONT_BULLET_SPEED,t);
	b->enemy = hurtEnemy;
	activeBullets->push_back(b);
}

void BulletManager::updateBullets() {
    for(int i=0;i<activeBullets->size();i++) {
        Bullet *b = activeBullets->at(i);
        b->updateLocation();
        if(b->distanceTravelled>b->distanceToTravel) {
        
        	// Hurt Enemy
			if(b->enemy != NULL) {
				b->enemy->health -= 1;
			}
        
            delete b;
            activeBullets->erase(activeBullets->begin()+(i));
        }
    }
}

BulletManager::BulletManager(SceneNode *shipSceneNode, SceneManager *sceneMgr, 
                GunState *pilotGunState, GunState *engineerGunState, 
                GunState *navigatorGunState, CollisionManager *colMgr, 
        	SwarmManager *swarmMgr)
    : shipSceneNode(shipSceneNode)
    , sceneMgr(sceneMgr)
    , pilotGunState(pilotGunState)
    , engineerGunState(engineerGunState)
    , navigatorGunState(navigatorGunState)
    , colMgr(colMgr)
    , swarmMgr(swarmMgr)
    , bnum(0)
{
    activeBullets = new vector<Bullet*>();
    colMgr->addMesh( sceneMgr->getEntity("ourship") );
}

BulletManager::~BulletManager() {
    cout << "...activeBullets" << endl;
    delete activeBullets;
}

void BulletManager::tick()
{
    // Firing the pilots gun
    if((pilotGunState && pilotGunState->fire()) || (engineerGunState && engineerGunState->fire()) || (navigatorGunState && navigatorGunState->fire())) {
        
        Vector3 position = shipSceneNode->getPosition();
        position.y -= 3.0;
        Quaternion orient = shipSceneNode->getOrientation();
    	Vector3 direction = orient.zAxis();
    	
    	fire(position,direction,ColourValue(0.7f,0.4f,0.0f));
        
    }
    
    // Check if any enemies are shooting
    
    // Loop for all enemies
    if(swarmMgr) {
	    vector<Enemy*> ents = swarmMgr->getAllEnemies();
	    Enemy *e;
	    for(vector<Enemy*>::const_iterator it=ents.begin();it!=ents.end();++it) {
	        e = *it;
	        
	        if(e->fire) {
	        	fire(e->getLocation(),e->getDirection(),ColourValue(0.7f,0.0f,0.0f));
	        }
	    }
    }
    
    
    
    updateBullets();
}
