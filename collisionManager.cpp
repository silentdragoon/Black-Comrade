#include "collisionManager.h"

CollisionManager::CollisionManager( SceneManager* sceneMgr, MapManager* mp ):
    sceneMgr(sceneMgr),
    mp(mp)
{
    cd = new CollisionDetection();
    vector<Entity*> pc = mp->getMapPieces();
    for( vector<Entity*>::iterator it = pc.begin(); it!=pc.end(); ++it)
    {
        cd->addStaticTreeCollisionMesh(*it);
    }
    cd->createShipMesh( sceneMgr->getEntity("ourship") );
    //cd->addTreeCollisionMesh( sceneMgr->getEntity("ourship") );
}

Collision CollisionManager::shipMapCollision(Vector3 *shipPos)
{
    //bool isCollided = false;
    dFloat contacts[16] = {0.0f};
    dFloat normals[16] = {0.0f};
    dFloat penetration[16] = {0.0f};
    Collision col = Collision(false,normals,contacts,penetration);
    //gets the map the ship is in
    Entity* e = mp->getEntity( shipPos );
    //checks if the two entities have collided
    if( e != NULL) col = cd->isCollision( sceneMgr->getEntity("ourship"), e );
    return col;
}

dFloat CollisionManager::getRCDistBetweenPoints( Vector3 *start, Vector3 *end, Entity *collideAgainst )
{
    return cd->rayCollideDist( start, end, collideAgainst);
}

dFloat CollisionManager::getRCDistAlongRay( Vector3 *start, Real pitch, Real yaw, dFloat maxDist, Entity* collideAgainst )
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

Vector3* CollisionManager::getRCVector( Vector3 *start, Real pitch, Real yaw, Entity* collideAgainst )
{
    Vector3 end;
    dFloat dist = 2000.0;
    end.x = start->x + sin(yaw)*dist;
    end.y = start->y + sin(pitch)*dist;
    end.z = start->z + cos(yaw)*dist;
    dFloat t = cd->rayCollideDist( start, &end, collideAgainst);
    
    return new Vector3( start->x + sin(yaw)*t*dist, start->y + sin(pitch)*t*dist, start->z + cos(yaw)*t*dist);
}

//doenst work
dFloat CollisionManager::getRCAgainstShip ( Vector3  *start, Real pitch, Real yaw )
{
    return getRCDistAlongRay( start, pitch, yaw, 2000.0, sceneMgr->getEntity("ourship") );
}

void CollisionManager::addEnemy( Entity e)
{
    ;//cd->createEnemyMesh(e);
}

dFloat CollisionManager::rayCollideWithEnemy( Vector3 *start, Vector3 *direction, Entity* collideAgainst)
{
    Vector3 end;
    dFloat dist = 2000;
    end.x = start->x + direction->x * dist;
    end.y = start->y + direction->y * dist;
    end.z = start->z + direction->z * dist;

    return rayCollideWithEnemy( start, &end, collideAgainst) *dist;
}
    



