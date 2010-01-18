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

Collision CollisionManager::isCollided(Vector3 *shipPos)
{
    //bool isCollided = false;
    dFloat contacts[16] = {0.0f};
    dFloat normals[16] = {0.0f};
    dFloat penetration[16] = {0.0f};
    Collision col = Collision(false,normals,contacts,penetration);
    Entity* e = mp->getEntity( shipPos );
    if( e != NULL) col = cd->isCollision( sceneMgr->getEntity("ourship"), e );
    return col;
}
