#include "collisionManager.h"

CollisionManager::CollisionManager( SceneManager* sceneMgr, MapManager* mp ):
    sceneMgr(sceneMgr),
    mp(mp)
{
    cd = new CollisionDetection();
    std::vector<Entity*> pc = mp->getMapPieces();
    double percInc = 100.0/pc.size();
    double percDone = 0;
    for( std::vector<Entity*>::iterator it = pc.begin(); it!=pc.end(); ++it)
    {
        cout << "Map pieces loaded: "<< int(percDone) <<"%"<<endl;
        percDone += percInc;
        cd->addStaticTreeCollisionMesh(*it);
    }
    cout << "Map pieces loaded: 100%"<<endl;
}


dFloat CollisionManager::getRCMapDist( Vector3 *pos, Vector3 *direction )
{
    Entity* e[5];
    mp->getMapEntities( pos, e );
    dFloat closestDist = 2500.0;
    for(int i = 0; i < 5; i++)
    {
        if( e[i] != NULL )
        {
            dFloat tmp = getRCDirDist(pos,direction, 2000.0, e[i]);
            if( tmp < closestDist) closestDist = tmp;
        }
    }
    return  closestDist;
}

dFloat CollisionManager::getRCDirDist(Vector3 *pos, Vector3 *direction, dFloat dist, Entity* e)
{
    Vector3 end;
    
    end.x = pos->x + direction->x * dist;
    end.y = pos->y + direction->y * dist;
    end.z = pos->z + direction->z * dist;
    
    return (cd->rayCollideDist( pos, &end, e)*dist);
}

void CollisionManager::addMesh( Entity *e)
{
    cd->createConvexHull(e);//cd->createEnemyMesh(e);
}

dFloat CollisionManager::rayCollideWithTransform( Vector3 *start, Vector3 *direction, Entity* collideAgainst)
{ 
    dFloat dist = 2000;
    double x = start->x + direction->x * dist;
    double y = start->y + direction->y * dist;
    double z = start->z + direction->z * dist;
    Vector3 end = Vector3(x,y,z);
    return (cd->rayCollideWithTransform( start, &end, collideAgainst) * dist);
    //return -1;
}

Collision CollisionManager::collideWithMapPiece( Entity *e )
{
    dFloat contacts[16] = {0.0f};
    dFloat normals[16] = {0.0f};
    
    dFloat penetration[16] = {0.0f};
    Collision col = Collision(false,normals,contacts,penetration);
    Vector3 pos = e->getParentSceneNode()->_getFullTransform().getTrans();
    Entity* mapEn = mp->getEntity( &pos );
    //checks if the two entities have collided
    if( e != NULL) col = cd->mapCollision( e, mapEn );
    return col;
}
    

/* dFloat CollisionManager::getRCDistBetweenPoints( Vector3 *start, Vector3 *end, Entity *collideAgainst )
{
    return cd->rayCollideDist( start, end, collideAgainst);
} */

/* dFloat CollisionManager::getRCDistAlongRay( Vector3 *start, Real pitch, Real yaw, dFloat maxDist, Entity* collideAgainst )
{
    Vector3 end;

    end.x = start->x + sin(yaw)*maxDist;
    end.y = start->y + sin(pitch)*maxDist;
    end.z = start->z + cos(yaw)*maxDist;
    dFloat tmp = cd->rayCollideDist( start, &end, collideAgainst);
    return tmp * maxDist;
}

dFloat CollisionManager::getRCMapDist( Vector3 *pos, Real pitch, Real yaw  )
{
    Entity* e = mp->getEntity( pos );
    return getRCDistAlongRay( pos, pitch, yaw, 2000.0, e );
} */


Collision CollisionManager::shipMapCollision(Vector3 *shipPos)
{
    //bool isCollided = false;
    dFloat contacts[16] = {0.0f};
    dFloat normals[16] = {0.0f};
    dFloat penetration[16] = {0.0f};
    Collision col = Collision(false,normals,contacts,penetration);
    //gets the map the ship is in
    Entity* e = mp->getEntity( shipPos );
    //NOTE: uses asumed name for ship
    //checks if the two entities have collided
    if( e != NULL) col = cd->mapCollision( sceneMgr->getEntity("ourship"), e );
    return col;
}

/* Vector3* CollisionManager::getRCVector( Vector3 *start, Real pitch, Real yaw, Entity* collideAgainst )
{
    Vector3 end;
    dFloat dist = 2000.0;
    end.x = start->x + sin(yaw)*dist;
    end.y = start->y + sin(pitch)*dist;
    end.z = start->z + cos(yaw)*dist;
    dFloat t = cd->rayCollideDist( start, &end, collideAgainst);
    
    return new Vector3( start->x + sin(yaw)*t*dist, start->y + sin(pitch)*t*dist, start->z + cos(yaw)*t*dist);
} */


    



