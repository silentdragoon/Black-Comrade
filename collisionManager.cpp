#include "collisionManager.h"

CollisionManager::CollisionManager( SceneManager* sceneMgr, MapCreate* mp ):
    sceneMgr(sceneMgr),
    mp(mp)
{
    cd = new CollisionDetection();
    vector<Entity*> pc = mp->getMapPieces();
    for( vector<Entity*>::iterator it = pc.begin(); it!=pc.end(); ++it)
    {
        cd->addTreeCollisionMesh(*it);
    }
    cd->createShipMesh( sceneMgr->getEntity("ourship") );
    //cd->addTreeCollisionMesh( sceneMgr->getEntity("ourship") );
}

bool CollisionManager::isCollided(Vector3 *shipPos)
{
    bool isCollided = false;
    Entity* e = mp->getEntity( shipPos );
    if( e != NULL) isCollided = cd->isCollision( sceneMgr->getEntity("ourship"), e );
    return isCollided;
}
