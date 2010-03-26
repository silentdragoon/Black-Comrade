#include "mapManager2.h"

MapManager::MapManager(char*file, SceneManager *sceneManager) : 
    sceneManager(sceneManager)
{
    MAPROOT = ConstManager::getString("map_file_path");

    cout << "Opening map: " << file << endl;

    objective = false;
    
    ifstream map (file);
    if(map.is_open()) {
        int y = 0;
        while(!map.eof()) {
            string line;
            getline(map, line);
            if(y<ConstManager::getInt("map_size")) {
                // Read in map tile information
                for(int x=0;x<ConstManager::getInt("map_size");x++) {
                    std::vector<int> connections = std::vector<int>();
                    if(line[x]=='.') {
                        mts[x][y] = new MapTile();
                    } else if(line[x]=='|') {
                        connections.push_back(1);
                        connections.push_back(3);
                        sort(connections.begin(),connections.end()); // Sort to make sure nto the wrong way around
                        createTile("strai/",connections,x,y);
                    } else if(line[x]=='-') {
                        connections.push_back(2);
                        connections.push_back(4);
                        createTile("strai/",connections,x,y);
                    } else if(line[x]=='l') {
                        connections.push_back(1);
                        connections.push_back(2);
                        createTile("junct/",connections,x,y);
                    } else if(line[x]=='r') {
                        connections.push_back(2);
                        connections.push_back(3);
                        createTile("junct/",connections,x,y);
                    } else if(line[x]=='<') {
                        connections.push_back(3);
                        connections.push_back(4);
                        createTile("junct/",connections,x,y);
                    } else if(line[x]=='j') {
                        connections.push_back(1);
                        connections.push_back(4);
                        createTile("junct/",connections,x,y);
                    } else if(line[x]=='t') {
                        connections.push_back(1);
                        connections.push_back(2);
                        connections.push_back(3);
                        createTile("junct/",connections,x,y);
                    } else if(line[x]=='=') {
                        connections.push_back(1);
                        connections.push_back(3);
                        connections.push_back(4);
                        createTile("junct/",connections,x,y);
                    } else if(line[x]=='+') {
                        connections.push_back(1);
                        connections.push_back(2);
                        connections.push_back(3);
                        connections.push_back(4);
                        createTile("junct/",connections,x,y);
                    } else if(line[x]=='/') {
                        connections.push_back(1);
                        connections.push_back(2);
                        connections.push_back(4);
                        createTile("junct/",connections,x,y);
                    } else if(line[x]=='\\') {
                        connections.push_back(2);
                        connections.push_back(3);
                        connections.push_back(4);
                        createTile("junct/",connections,x,y);
                    } else if(line[x]=='1') {
                        connections.push_back(1);
                        createTile("ends/",connections,x,y);
                    } else if(line[x]=='2') {
                        connections.push_back(2);
                        createTile("ends/",connections,x,y);
                    } else if(line[x]=='3') {
                        connections.push_back(3);
                        createTile("ends/",connections,x,y);
                    } else if(line[x]=='4') {
                        connections.push_back(4);
                        createTile("ends/",connections,x,y);
                    } else if(line[x]=='0') {
                        objx=x;
                        objy=y;
                        connections.push_back(1);
                        connections.push_back(2);
                        connections.push_back(3);
                        connections.push_back(4);
                        createTile("objec/",connections,x,y);
                    }
                }

            } else {
                // Read in waypoint information
                if((line[0]=='w')&&(line[1]=='p')) {
                    istringstream liness(line);
                    int x;
                    int y;
                    string temp;
                    string rubbish;

                    liness >> rubbish;
                    liness >> x;
                    liness >> y;
                    liness >> temp;

                    string *name = new string(temp);
                    cout << x << " " << y << " " << name << endl;

                    Waypoint *w = new Waypoint(name,x,y);
                    waypoints.push_back(w);
                    mts[x][y]->assignWaypoint(w);

                    if(*w->getName()=="wp_start") {
                        startx = (x * ConstManager::getInt("map_tile_size"))+(ConstManager::getInt("map_tile_size")/2);
                        starty = (y * ConstManager::getInt("map_tile_size"))+(ConstManager::getInt("map_tile_size")/2);
                    }
                }
            }
            y++;
        }
        map.close();

        setSpawnPoints();
        makeConPieces();

        cout << "Map " << file << " loaded..." << endl;
    } else {
        cerr << "Unable to open map: " << file << endl;
    }
}

void MapManager::createTile(string adir, std::vector<int> connections, int x, int y) {
    std::vector<string> files = std::vector<string>();
    string dir = MAPROOT + adir;
    std::stringstream out;
    for(std::vector<int>::const_iterator it=connections.begin();it!=connections.end(); ++it) {
        out << *it << "-";
    }
    dir+=out.str().substr(0,out.str().length()-1);
    dir+="/";

    DIR *dp;
    struct dirent *dirp;
    if((dp = opendir(dir.c_str()))==NULL) {
        cerr << "Error(" << errno << ") opening " << dir << " at " << x << " " << y << endl;
    }

    while((dirp = readdir(dp))!=NULL) {
        if((string(dirp->d_name).compare(".")!=0)&&(string(dirp->d_name).compare("..")!=0)) {
            string path = dir;
            path += string(dirp->d_name);
            files.push_back(path);
        }
    }
    closedir(dp);

    SceneNode *node = sceneManager->getRootSceneNode()->createChildSceneNode();
    string name = "mapTile";
    std::stringstream out2;
    out2 << "-" << x << "-" << y;
    name += out2.str();
    
    MeshPtr pMesh = MeshManager::getSingleton().load(files.at(0),
          ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,    
          HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY,
          HardwareBuffer::HBU_STATIC_WRITE_ONLY,
          true, true);

    unsigned short src, dest;
    if (!pMesh->suggestTangentVectorBuildParams(VES_TANGENT, src, dest))
    {
       pMesh->buildTangentVectors(VES_TANGENT, src, dest);
    }
    
    Entity *e = sceneManager->createEntity(name, pMesh->getName() );

    mapEntities.push_back(e);
    node->attachObject(e);
    
    cout << files.at(0) << ": " << (x * ConstManager::getInt("map_tile_size")) << " " << (y * ConstManager::getInt("map_tile_size")) << endl;
	Vector3 pos(x * ConstManager::getInt("map_tile_size"),0 , y * ConstManager::getInt("map_tile_size"));
    node->setPosition(pos);

    MapTile *m = new MapTile(node,e,x,y);
    m->setConnections(connections);
    mts[x][y] = m;
}

std::vector<Entity*> MapManager::getMapPieces() {
    return mapEntities;
}

Entity* MapManager::getEntity(Vector3 *locn) {
    int x =(int) floor(locn->x/(double)ConstManager::getInt("map_tile_size"));
    int y =(int) floor(locn->z/(double)ConstManager::getInt("map_tile_size"));

    if(0 <= x && x < ConstManager::getInt("map_size")) {
        if(0 <= y && y < ConstManager::getInt("map_size")) {
            return mts[x][y]->getEntity();
        }
    }
    
    return NULL;
}

void MapManager::getMapEntities(Vector3 *locn, Entity** mps) {
    int x =(int) floor(locn->x/(double)ConstManager::getInt("map_tile_size"));
    int y =(int) floor(locn->z/(double)ConstManager::getInt("map_tile_size"));

    /* if(mts[x][y]->isObj()) {
        mps[0] = mts[x][y]->getEntity();
        //cout << "0: " << mts[x][y]->getEntity()->getName() << endl;
        std::vector<int> adj = mts[x][y]->getConnections();

        mps[1] = NULL;
        mps[2] = NULL;
        mps[3] = NULL;
        mps[4] = NULL;

        for(std::vector<int>::const_iterator it=adj.begin();it!=adj.end(); ++it) {
            int c = *it;
            if(c==1) {
                if(!mts[x][y]->getAdjacent(c)->isObj()) {
                    //cout << "1: " <<mts[x][y]->getAdjacent(c)->getEntity()->getName() <<  endl;
                    mps[1] = mts[x][y]->getAdjacent(c)->getEntity();
                }
            } else if(c==2) {
                if(!mts[x][y]->getAdjacent(c)->isObj()) {
                    //cout << "2: " <<mts[x][y]->getAdjacent(c)->getEntity()->getName() <<  endl;
                    mps[2] = mts[x][y]->getAdjacent(c)->getEntity();
                }
            } else if(c==3) {
                if(!mts[x][y]->getAdjacent(c)->isObj()) {
                    //cout << "3: " <<mts[x][y]->getAdjacent(c)->getEntity()->getName() <<  endl;
                    mps[3] = mts[x][y]->getAdjacent(c)->getEntity();
                }
            } else if(c==4) {
                if(!mts[x][y]->getAdjacent(c)->isObj()) {
                    //cout << "4: " <<mts[x][y]->getAdjacent(c)->getEntity()->getName() <<  endl;
                    mps[4] = mts[x][y]->getAdjacent(c)->getEntity();
                }
            } else {
            }
        }
    } else { */
        
    mps[0] = mts[x][y]->getEntity();
    //cout << "0: " << mts[x][y]->getEntity()->getName() << endl;
    std::vector<int> adj = mts[x][y]->getConnections();

    mps[1] = NULL;
    mps[2] = NULL;
    mps[3] = NULL;
    mps[4] = NULL;

    for(std::vector<int>::const_iterator it=adj.begin();it!=adj.end(); ++it) {
        int c = *it;
        if(c==1) {
            //cout << "1: " <<mts[x][y]->getAdjacent(c)->getEntity()->getName() <<  endl;
            mps[1] = mts[x][y]->getAdjacent(c)->getEntity();
        } else if(c==2) {
            //cout << "2: " <<mts[x][y]->getAdjacent(c)->getEntity()->getName() <<  endl;
            mps[2] = mts[x][y]->getAdjacent(c)->getEntity();
        } else if(c==3) {
            //cout << "3: " <<mts[x][y]->getAdjacent(c)->getEntity()->getName() <<  endl;
            mps[3] = mts[x][y]->getAdjacent(c)->getEntity();
        } else if(c==4) {
            //cout << "4: " <<mts[x][y]->getAdjacent(c)->getEntity()->getName() <<  endl;
            mps[4] = mts[x][y]->getAdjacent(c)->getEntity();
        } else {
        }
    }
}

MapTile* MapManager::getMapTile(Vector3 *locn) {
    int x =(int) floor(locn->x/(double)ConstManager::getInt("map_tile_size"));
    int y =(int) floor(locn->z/(double)ConstManager::getInt("map_tile_size"));
    
    if(0 <= x && x < ConstManager::getInt("map_size")) {
        if(0 <= y && y < ConstManager::getInt("map_size")) {
            return mts[x][y];
        }
    }
    
    return NULL;   
}

Vector3 MapManager::getActualPosition(MapTile *mapTile) {
    int half = (int) floor((double)ConstManager::getInt("map_tile_size") / 2.0);
    int x = (int)(mapTile->getX() * ConstManager::getInt("map_tile_size")) + half;
    int y = (int)(mapTile->getY() * ConstManager::getInt("map_tile_size")) + half;
    return Vector3(x,0,y);
}

std::vector<string*> MapManager::getWaypoints(Vector3 *locn) {
    int x =(int) floor(locn->x/(double)ConstManager::getInt("map_tile_size"));
    int y =(int) floor(locn->z/(double)ConstManager::getInt("map_tile_size"));
    std::vector<string*> names;
    if(mts[x][y]->hasWaypoint()) {
        std::vector<Waypoint*> wps = mts[x][y]->getWaypoints();
        for(std::vector<Waypoint*>::const_iterator it=wps.begin();it!=wps.end(); ++it) {
            Waypoint *w = *it;
            names.push_back(w->getName());
        }
    }
    return names;
}

std::vector<Vector3*> MapManager::getSpawnPoints(Vector3 *locn) {
    int x = (int) floor(locn->x/(double)ConstManager::getInt("map_tile_size"));
    int y = (int) floor(locn->z/(double)ConstManager::getInt("map_tile_size"));
    return mts[x][y]->getSpawnPoints(); 
}

std::vector<Vector3*> MapManager::getInitialSpawnPoints() {
    Waypoint *w;
    std::vector<Vector3*> places = std::vector<Vector3*>();

    for(std::vector<Waypoint*>::const_iterator it=waypoints.begin();it!=waypoints.end(); ++it) {
        w = *it;
        if(*(w->getName())=="en_spawn") {
            int x = w->getX();
            int y = w->getY();

            double xx = (x * (ConstManager::getInt("map_tile_size"))) + (0.5*ConstManager::getInt("map_tile_size"));
            double yy = (y * (ConstManager::getInt("map_tile_size"))) + (0.5*ConstManager::getInt("map_tile_size"));

            Vector3 *v = new Vector3(xx,0.0,yy);
            places.push_back(v);
        }
    }
    return places;
}

void MapManager::makeConPieces() {
    for(int y=0;y<Const::MAPSIZE;y++) {
        for(int x=0;x<Const::MAPSIZE;x++) {
            if(!(mts[x][y]->isEmpty())) {
                if(mts[x][y]->eastConnected()) {
                    SceneNode *node = sceneManager->getRootSceneNode()->createChildSceneNode();
                    string name = "eConPiece";
                    std::stringstream out;
                    out << "-" << x << "-" << y << "-2";
                    name += out.str();
                    Entity *e = sceneManager->createEntity(name, "polySurfaceShape7.mesh");
                    node->attachObject(e);
                    node->yaw( Radian(PI/2.0) );
                    //needs Tuning
                    Vector3 pos( x * ConstManager::getInt("map_tile_size") + ConstManager::getInt("map_tile_size"),0 , y * ConstManager::getInt("map_tile_size") + (ConstManager::getInt("map_tile_size")/2.0));
                    node->setPosition(pos);
                    attachLight( pos.x, pos.z);
                }
                if(mts[x][y]->southConnected()) {
                    SceneNode *node = sceneManager->getRootSceneNode()->createChildSceneNode();
                    string name = "sConPiece";
                    std::stringstream out;
                    out << "-" << x << "-" << y << "-3";
                    name += out.str();
                    Entity *e = sceneManager->createEntity(name,  "polySurfaceShape7.mesh");
                    node->attachObject(e);
                    //needs Tuning
                    Vector3 pos(x * ConstManager::getInt("map_tile_size") + (ConstManager::getInt("map_tile_size")/2.0) ,0 , y * ConstManager::getInt("map_tile_size") +(ConstManager::getInt("map_tile_size")));
                    node->setPosition(pos);
                    attachLight( pos.x, pos.z);
                }
            }
        }
    }
}

void MapManager::attachLight( Real x, Real z )
{
    SceneNode *node = sceneManager->getRootSceneNode()->createChildSceneNode();
    
    std::stringstream out;
    out << "-" << x << "-" << z;
    
    string lightS = "light";
    lightS += out.str();
    
    Light* light = sceneManager->createLight(lightS);
    light->setType(Light::LT_POINT);
    light->setDiffuseColour(ColourValue(25.25f,25.25f,25.0f));
    light->setSpecularColour(ColourValue(25.25f,25.25f,25.0f));
    light->setAttenuation( 100, 1.0, 0.045, 0.0075);
    //light->setSpotlightRange(Ogre::Degree(20), Ogre::Degree(60), 1.2);
    //light->setDirection(Vector3::NEGATIVE_UNIT_Y);
    node->attachObject(light);
    Vector3 pos(x, 23 , z);
    node->setPosition(pos);
}


Vector3 MapManager::getDynamicSpawnPoint(Vector3 *locn) {
    int x = (int) floor(locn->x/(double)ConstManager::getInt("map_tile_size"));
    int y = (int) floor(locn->z/(double)ConstManager::getInt("map_tile_size"));

    std::vector<int> conns = mts[x][y]->getConnections();
    int c = conns.at(0);

    std::vector<int> conns2 = mts[x][y]->getAdjacent(c)->getConnections();
    int c2 = conns2.at(0);

    if((((c+1)%4)+1)==c2) {
        if(conns2.size()!=1) {
            c2 = conns2.at(1);
        }
    }

    std::vector<int> conns3 = mts[x][y]->getAdjacent(c)->getAdjacent(c2)->getConnections();
    int c3 = conns3.at(0);
    if((((c2+1)%4)+1)==c3) {
        if(conns3.size()!=1) {
            c3 = conns3.at(1);
        }
    }

    if((mts[x][y]->getAdjacent(c)->getAdjacent(c2)->getX()==x)&&
            (mts[x][y]->getAdjacent(c)->getAdjacent(c2)->getY()==y)) {
        return getDynamicSpawnPoint(locn);
    }

    Vector3* sp =  mts[x][y]->getAdjacent(c)->getAdjacent(c2)->getSpawn(c3);
    return Vector3(sp->x,sp->y,sp->z);
}

void MapManager::setSpawnPoints() {
    for(int y=0;y<ConstManager::getInt("map_size");y++) {
        for(int x=0;x<ConstManager::getInt("map_size");x++) {
            if(!mts[x][y]->isEmpty()) {
                std::vector<int> conns = mts[x][y]->getConnections(); 

                double xx = x * ConstManager::getInt("map_tile_size");
                double yy = y * ConstManager::getInt("map_tile_size"); // Actually z in ogre coords
                double zz = 0.0;

                std::vector<Vector3*> places = std::vector<Vector3*>();

                for(std::vector<int>::const_iterator it=conns.begin();it!=conns.end(); ++it) {
                    int c = *it;
                    if(c==1) {
                        double xxx;
                        double yyy = yy;
                        xxx = xx + (0.5*ConstManager::getInt("map_tile_size"));
                        Vector3 *v = new Vector3(xxx,zz,yyy);
                        places.push_back(v);
                        mts[x][y]->setSpawn(c,v);
                        mts[x][y]->setAdjacent(c,mts[x][y-1]);
                    }
                    if(c==2) {
                        double xxx;
                        double yyy;
                        xxx = xx + (ConstManager::getInt("map_tile_size"));
                        yyy = yy + (0.5*ConstManager::getInt("map_tile_size"));
                        Vector3 *v = new Vector3(xxx,zz,yyy);
                        places.push_back(v);
                        mts[x][y]->setSpawn(c,v);
                        mts[x][y]->setAdjacent(c,mts[x+1][y]);
                    }
                    if(c==3) {
                        double xxx;
                        double yyy;
                        xxx = xx + (0.5*ConstManager::getInt("map_tile_size"));
                        yyy = yy + (ConstManager::getInt("map_tile_size"));
                        Vector3 *v = new Vector3(xxx,zz,yyy);
                        places.push_back(v);
                        mts[x][y]->setSpawn(c,v);
                        mts[x][y]->setAdjacent(c,mts[x][y+1]);
                    }
                    if(c==4) {
                        double xxx = xx;
                        double yyy;
                        yyy = yy + (0.5*ConstManager::getInt("map_tile_size"));
                        Vector3 *v = new Vector3(xxx,zz,yyy);
                        places.push_back(v);
                        mts[x][y]->setSpawn(c,v);
                        mts[x][y]->setAdjacent(c,mts[x-1][y]);
                    }
                }
                mts[x][y]->assignSpawnPoints(places);
            }
        }
    }
}

Vector3 MapManager::getObjectivePosition() {
    double x = (objx*ConstManager::getInt("map_tile_size"))+(ConstManager::getInt("map_tile_size")/2.0);
    double y = 0.0;
    double z = (objy*ConstManager::getInt("map_tile_size"))+(ConstManager::getInt("map_tile_size")/2.0);
    return Vector3(x,y,z);
}
