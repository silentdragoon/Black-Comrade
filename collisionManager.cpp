#include "collisionManager.h"

CollisionManager::CollisionManager( SceneManager* sceneMgr, MapManager* mp,
                                    LoadingScreen* loadingScreen, bool rebuildCollisionMeshes ):
    sceneMgr(sceneMgr),
    mp(mp),
    loadingScreen(loadingScreen),
    obj(true)
{
    movableObj = std::vector<Entity*>();
    if(rebuildCollisionMeshes) deleteAllColMeshes();
    cd = new CollisionDetection(rebuildCollisionMeshes);
    //building the map
    std::vector<Entity*> pc = mp->getMapEntitiesForCollision();
    double percInc = 100.0/pc.size();
    double percDone = 0;
    for( std::vector<Entity*>::iterator it = pc.begin(); it!=pc.end(); ++it)
    {
        cout << "Map pieces loaded: "<< int(percDone) <<"%"<<endl;
        loadingScreen->updateProgress(int(percDone));
        percDone += percInc;
        cd->addStaticTreeCollisionMesh(*it);
    }
    //adding the objective collision mesh
    Vector3 obj = mp->getObjectivePosition();
    Real x = obj.x;
    Real y = obj.y;
    Real z = obj.z;
    addObjMesh(x,y,z,30.0);

    cout << "Map pieces loaded: 100%"<<endl;
}

void CollisionManager::deleteAllColMeshes()
{
    DIR *directory;

    string dirString = ConstManager::getString("cmesh_file_path");
    if((directory=opendir(dirString.c_str()))==NULL) cout << "Couldnt delete  cmeshes"<<endl;
    else
    {
        struct dirent *theFile;
        while ((theFile=readdir(directory)))
        {
            //exlude pilepaths of . and ..
            string path = dirString;
            if( !(string(theFile->d_name).compare(".") == 0 || string(theFile->d_name).compare("..") == 0))
            {
                path.append(theFile->d_name);
                //cout<<"P: "<< path <<endl;
                if(remove( path.c_str() ) == -1 ) cout << "Error deleting file"<< endl;
            }
        }
        closedir(directory);
    }
}

void CollisionManager::addColidableMovableObject( Entity *e )
{
    cd->createConvexHull(e);
    movableObj.push_back(e);
}

dFloat CollisionManager::getRCMapDist( Vector3 *pos, Vector3 *direction )
{
    Entity* e[5];
    mp->getEntitiesForCollisionFromAPosition( pos, e );
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

dFloat CollisionManager::getRCMapAndMovObjsDist( Vector3 *pos, Vector3 *direction )
{
    dFloat mapDist = getRCMapDist(pos, direction);
    dFloat moveObjDist = getRCMovableObjectsDist( pos, direction );
    if( mapDist < moveObjDist ) return mapDist;
    else return moveObjDist;
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
    cd->createConvexHull(e);
}

void CollisionManager::addShipMesh( Entity *e)
{
    cd->createConvexHull(e);
}

void CollisionManager::addObjMesh( Real x, Real y, Real z, Real radius)
{
    cd->createObjPrimitive(x,y,z,radius);
}

dFloat CollisionManager::getRCObjDist( Vector3 *start, Vector3 *direction)
{
    if( obj )
    {
        dFloat dist = 2000;
        double x = start->x + direction->x * dist;
        double y = start->y + direction->y * dist;
        double z = start->z + direction->z * dist;
        Vector3 end = Vector3(x,y,z);
        return cd->objRayCollision( start, &end )*dist;
    }
    else return false;
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

bool CollisionManager::collideEntityWithObj(Entity *e)
{
    if(obj) return cd->collideEntityWithObj(e);
    else return false;
}

Collision CollisionManager::collideWithMapPieceAndMovableObjects( Entity *e )
{
    //dFloat contacts[16] = {0.0f};
    //dFloat normals[16] = {0.0f};
    //dFloat penetration[16] = {0.0f};
    Collision col;// = Collision(false,normals,contacts,penetration);

    Vector3 pos = e->getParentSceneNode()->_getFullTransform().getTrans();

    Entity* ents[5];
    mp->getEntitiesForCollisionFromAPosition( &pos, ents );

    for( int i = 0; i < 5; i++ )
    {
        if( ents[i] != NULL)
        {
            col = cd->staicAndDynamicCollision( e, ents[i], false );
            if( col.isCollided ) return col;
        }
    }

    for(std::vector<Entity*>::const_iterator it=movableObj.begin();it!=movableObj.end(); ++it)
    {
        col = cd->staicAndDynamicCollision( e, *it, true);
        if( col.isCollided ) return col;
    }
    return col;
}

dFloat CollisionManager::getRCMovableObjectsDist( Vector3 *start, Vector3 *direction )
{
    dFloat dist = 2000.0;
    double x = start->x + direction->x * dist;
    double y = start->y + direction->y * dist;
    double z = start->z + direction->z * dist;

    dFloat minDist = 2000.0;
    dFloat tmpDist;

    for(std::vector<Entity*>::const_iterator it=movableObj.begin();it!=movableObj.end(); ++it)
    {
        Vector3 end = Vector3(x,y,z);
        tmpDist = cd->rayCollideWithTransform( start, &end, *it) * dist;
        if( tmpDist < minDist ) minDist = tmpDist;
    }
    return minDist;
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


/* Collision CollisionManager::shipMapCollision(Vector3 *shipPos)
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
} */

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






