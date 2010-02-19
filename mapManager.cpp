#include "mapManager.h"

using namespace std;

bool MapManager::buildMap(char* file)
{
    bool start = false;
    bool end = false;
    bool objec = false;

    cout << "Opening map template: " << file << endl;
    ifstream map (file);
    if(map.is_open()) {
        int ypos = 0;
        while(!map.eof()) {
            string line;
            getline(map, line);
            if(ypos<Const::MAPSIZE) {
                for(int xpos=0;xpos<Const::MAPSIZE;xpos++) {
                    mts[xpos][ypos]= new MapTile();
                    if(line[xpos]=='.') geo[xpos][ypos] = '.';
                    if(line[xpos]=='s'){ geo[xpos][ypos] = 's'; start = true; startx = xpos; starty = ypos;}
                    if(line[xpos]=='+') geo[xpos][ypos] = '+';
                    if(line[xpos]=='x') geo[xpos][ypos] = 'x';
                    if(line[xpos]=='e'){ geo[xpos][ypos] = 'e'; end = true; endx = xpos; endy = ypos;}
                    if(line[xpos]=='c') geo[xpos][ypos] = 'c';
                    if(line[xpos]=='o'){ geo[xpos][ypos] = 'o'; objec = true;}
                }
            } else {
                // READ IN WAYPOINTS
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
                }
            }
            ypos++;
        }
        map.close();
        cout << "Template loading complete." << endl;
        return true;
    } else {
        cerr << "Unable to open map file." << endl;
    }
    return false;
}

bool MapManager::outputMap(SceneNode *sceneNode)
{
    for(int xpos=0;xpos<Const::MAPSIZE;xpos++) {
        for(int ypos=0;ypos<Const::MAPSIZE;ypos++) {
            if(geo[xpos][ypos] == 's') {
                startx = (xpos * Const::TILE_SIZE)+(Const::TILE_SIZE/2);
                starty = (ypos * Const::TILE_SIZE)+(Const::TILE_SIZE/2);

                cout << "Startx: " << startx << " Start y: " << starty << endl;

                std::vector<int> connections = std::vector<int>();
                connections = getConnections(xpos,ypos);
                if(connections.size()==1) {
                    fetchTile("ends/",connections,xpos,ypos,sceneNode);
                } else {
                    cerr << "Start has more than one connection." << endl;
                    return false;
                }
            } else if(geo[xpos][ypos] == '+') {
                std::vector<int> connections = std::vector<int>();
                connections = getConnections(xpos,ypos);
                if(connections.size()!=1) {
                    fetchTile("junct/",connections,xpos,ypos,sceneNode);
                } else {
                    cerr << "Junction at " << xpos << " " << ypos << " has 1 or less connections." << endl;
                    return false;
                }
            } else if(geo[xpos][ypos] == 'x') {
                std::vector<int> connections = std::vector<int>();
                connections = getConnections(xpos,ypos);
                string dir;
                if(connections.size()==2) {
                    dir = "strai/";
                } else if(connections.size()==1) {
                    dir = "ends/";
                } else if(connections.size()>2) {
                    cerr << "Component at " << xpos << " " << ypos << " is bad, consider junction." << endl;
                    return false;
                }
                fetchTile(dir,connections,xpos,ypos,sceneNode);
            } else if(geo[xpos][ypos] == 'e') {
                std::vector<int> connections = std::vector<int>();
                connections = getConnections(xpos,ypos);
                if(connections.size()==1) {
                    fetchTile("ends/",connections,xpos,ypos,sceneNode);
                } else {
                    cerr << "End has more than one connection." << endl;
                    return false;
                }
            // 'c' and 'o' are special cases so we cant use fetchTile()
            } else if(geo[xpos][ypos] == 'c') {
                int cavtest;
                cavtest = cavernChecker(xpos,ypos,'c');
                if(cavtest==0) {
                    std::vector<string> files = std::vector<string>();
                    string dir = MAPROOT;
                    dir += "caves/1-2-3-4/";
                    getMeshList(dir,files,xpos,ypos);
                    // TODO: Select random file from list
                    random_shuffle(files.begin(),files.end());
                    
                    attachTile(sceneNode, &files.at(0), xpos, ypos);
                } else if(cavtest==7) {
                    std::vector<string> files = std::vector<string>();
                    string dir = MAPROOT;
                    dir += "cavel/1-2-3-4/";
                    getMeshList(dir,files,xpos,ypos);
                    // TODO: Select random file from list
                    random_shuffle(files.begin(),files.end());
                    
                    attachTile(sceneNode, &files.at(0), xpos, ypos);
                } else if(cavtest==-1) {
                    cerr << "A badly sized cavern appeared at " << xpos << " " << ypos << "." << endl;
                    return false;
                }
            } else if(geo[xpos][ypos] == 'o') {
                int cavtest;
                cavtest = cavernChecker(xpos,ypos,'o');
                if(cavtest==0) {
                    cerr << "The objective is too small, must be 3x3." << endl;
                    return false;
                } else if(cavtest==7) {
                    std::vector<string> files = std::vector<string>();
                    string dir = MAPROOT;
                    dir += "cavel/1-2-3-4/";
                    getMeshList(dir,files,xpos,ypos);
                    // TODO: Select random file from list
                    random_shuffle(files.begin(),files.end());
                    
                    attachTile(sceneNode, &files.at(0), xpos, ypos);
                } else if(cavtest==-1) {
                    cerr << "A badly sized objective appeared at " << xpos << " " << ypos << "." << endl;
                    return false;
                }
            }
        }
    }

    setSpawnPoints();
    //makeConPieces();

    return true;
}

void MapManager::fetchTile(string adir, std::vector<int> connections, int x, int y, SceneNode *sceneNode)
{
    std::vector<string> files = std::vector<string>();
    string dir = MAPROOT;
    dir += adir;
    std::stringstream out;
    for(std::vector<int>::const_iterator it=connections.begin();it!=connections.end(); ++it) {
        out << *it << "-";
    }
    string tmp = out.str();
    dir += tmp.substr(0,tmp.length()-1);
    dir += "/";
    
    getMeshList(dir,files,x,x);

    // TODO:  This is the bit where we need to randomise the std::vector, or somehow randomis the file returned.
    random_shuffle(files.begin(),files.end());

    attachTile(sceneNode, &files.at(0), x, y);
}

void MapManager::attachTile(SceneNode *sceneNode, string *file, int x, int y)
{
	SceneNode *node = sceneNode->createChildSceneNode();

    string name = "mapTile";
    std::stringstream out;
    out << "-" << x << "-" << y;
    name += out.str();
	Entity *e = sceneManager->createEntity(name, *file);
    mapEntities.push_back( e );
	
	node->attachObject(e);

    cout << file << ": " << (x * Const::TILE_SIZE) << " " << (y * Const::TILE_SIZE) << endl;
	
	Vector3 pos(x * Const::TILE_SIZE,0 , y * Const::TILE_SIZE);
	
	node->setPosition(pos);

    MapTile *m = new MapTile(node,e,x,y);

    Waypoint *w;

    for(std::vector<Waypoint*>::const_iterator it=waypoints.begin();it!=waypoints.end(); ++it) {
        w = *it;
        if((w->getX()==x)&&(w->getY()==y)) m->assignWaypoint(w);
    }

    mts[x][y] = m;
}



std::vector<Entity*> MapManager::getMapPieces()
{
    return mapEntities;
}

Entity* MapManager::getEntity(Vector3 *locn) 
{
    int x =(int) floor(locn->x/(double)Const::TILE_SIZE);
    int y =(int) floor(locn->z/(double)Const::TILE_SIZE);

    return mts[x][y]->getEntity();
}

void MapManager::getMapEntities(Vector3 *locn, Entity** mps ) {

    int x =(int) floor(locn->x/(double)Const::TILE_SIZE);
    int y =(int) floor(locn->z/(double)Const::TILE_SIZE);

    mps[0] = mts[x][y]->getEntity();
    std::vector<int> adj = mts[x][y]->getConnections();

    mps[1] = NULL;
    mps[2] = NULL;
    mps[3] = NULL;
    mps[4] = NULL;

    for(std::vector<int>::const_iterator it=adj.begin();it!=adj.end(); ++it) {
        int c = *it;
        if(c==1) {
            mps[1] = mts[x][y]->getAdjacent(c)->getEntity();
        } else if(c==2) {
            //mps[2] = mts[x+1][y]->getEntity();
            mps[2] = mts[x][y]->getAdjacent(c)->getEntity();
        } else if(c==3) {
            //mps[3] = mts[x][y+1]->getEntity();
            mps[3] = mts[x][y]->getAdjacent(c)->getEntity();
        } else if(c==4) {
            //mps[4] = mts[x-1][y]->getEntity();
            mps[4] = mts[x][y]->getAdjacent(c)->getEntity();
        } else {
        }
    }
}    

MapTile* MapManager::getMapTile(Vector3 *locn) {
    int x =(int) floor(locn->x/(double)Const::TILE_SIZE);
    int y =(int) floor(locn->z/(double)Const::TILE_SIZE);
    return mts[x][y];    
}

Vector3 MapManager::getActualPosition(MapTile* mapTile) {
    int half = (int) floor((double)Const::TILE_SIZE / 2.0);
    int x = (int)(mapTile->getX() * Const::TILE_SIZE) + half;
    int y = (int)(mapTile->getY() * Const::TILE_SIZE) + half;
    return Vector3(x,0,y);
}

string* MapManager::getWaypoint(Vector3 *locn) 
{
    int x =(int) floor(locn->x/(double)Const::TILE_SIZE);
    int y =(int) floor(locn->z/(double)Const::TILE_SIZE);
    
    if(mts[x][y]->hasWaypoint()) {
        return mts[x][y]->getWaypoint()->getName();
    }

    return NULL;
}

std::vector<Vector3*> MapManager::getSpawnPoints(Vector3 *locn)
{
    int x = (int) floor(locn->x/(double)Const::TILE_SIZE);
    int y = (int) floor(locn->z/(double)Const::TILE_SIZE);

    return mts[x][y]->getSpawnPoints(); 
}

void MapManager::setSpawnPoints()
{
    for(int y=0;y<Const::MAPSIZE;y++) {
        for(int x=0;x<Const::MAPSIZE;x++) {
            std::vector<int> conns = getConnections(x,y);

            mts[x][y]->setConnections(conns);

            double xx = x * Const::TILE_SIZE;
            double yy = y * Const::TILE_SIZE; // Actually z in ogre coords
            double zz = 0.0;

            std::vector<Vector3*> places = std::vector<Vector3*>();

            for(std::vector<int>::const_iterator it=conns.begin();it!=conns.end(); ++it) {
                int c = *it;
                if(c==1) {
                    double xxx;
                    double yyy = yy;
                    xxx = xx + (0.5*Const::TILE_SIZE);
                    Vector3 *v = new Vector3(xxx,zz,yyy);
                    places.push_back(v);
                    mts[x][y]->setSpawn(c,v);
                    mts[x][y]->setAdjacent(c,mts[x][y-1]);
                }
                if(c==2) {
                    double xxx;
                    double yyy;
                    xxx = xx + (Const::TILE_SIZE);
                    yyy = yy + (0.5*Const::TILE_SIZE);
                    Vector3 *v = new Vector3(xxx,zz,yyy);
                    places.push_back(v);
                    mts[x][y]->setSpawn(c,v);
                    mts[x][y]->setAdjacent(c,mts[x+1][y]);
                }
                if(c==3) {
                    double xxx;
                    double yyy;
                    xxx = xx + (0.5*Const::TILE_SIZE);
                    yyy = yy + (Const::TILE_SIZE);
                    Vector3 *v = new Vector3(xxx,zz,yyy);
                    places.push_back(v);
                    mts[x][y]->setSpawn(c,v);
                    mts[x][y]->setAdjacent(c,mts[x][y+1]);
                }
                if(c==4) {
                    double xxx = xx;
                    double yyy;
                    yyy = yy + (0.5*Const::TILE_SIZE);
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


void MapManager::makeConPieces()
{
    for(int y=0;y<Const::MAPSIZE;y++) 
    {
        for(int x=0;x<Const::MAPSIZE;x++) 
        {
            if(!(mts[x][y]->isEmpty()))
            {
                if(mts[x][y]->eastConnected())
                {
                    SceneNode *node = sceneManager->getRootSceneNode()->createChildSceneNode();
                    string name = "eConPiece";
                    std::stringstream out;
                    out << "-" << x << "-" << y << "-2";
                    name += out.str();
                    Entity *e = sceneManager->createEntity(name, "polySurfaceShape7.mesh");
                    node->attachObject(e);
                    node->yaw( Radian(PI/2.0) );
                    //needs Tuning
                    Vector3 pos( x * Const::TILE_SIZE + Const::TILE_SIZE,0 , y * Const::TILE_SIZE + (Const::TILE_SIZE/2.0));
                    node->setPosition(pos);
                    //attachLight( pos.x, pos.z);
                }
                if(mts[x][y]->southConnected())
                {
                    SceneNode *node = sceneManager->getRootSceneNode()->createChildSceneNode();
                    string name = "sConPiece";
                    std::stringstream out;
                    out << "-" << x << "-" << y << "-3";
                    name += out.str();
                    Entity *e = sceneManager->createEntity(name,  "polySurfaceShape7.mesh");
                    node->attachObject(e);
                    //needs Tuning
                    Vector3 pos(x * Const::TILE_SIZE + (Const::TILE_SIZE/2.0) ,0 , y * Const::TILE_SIZE +(Const::TILE_SIZE));
                    node->setPosition(pos);
                    //attachLight( pos.x, pos.z);
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
    light->setType(Light::LT_SPOTLIGHT);
    light->setDiffuseColour(ColourValue(25.25f,25.25f,25.0f));
    light->setSpecularColour(ColourValue(25.25f,25.25f,25.0f));
    light->setAttenuation( 100, 1.0, 0.045, 0.0075);
    light->setSpotlightRange(Ogre::Degree(20), Ogre::Degree(60), 1.2);
    light->setDirection(Vector3::NEGATIVE_UNIT_Y);
    node->attachObject(light);
    Vector3 pos(x, 23 , z);
    node->setPosition(pos);
}

std::vector<Vector3*> MapManager::getInitialSpawnPoints()
{
    Waypoint *w;

    std::vector<Vector3*> places = std::vector<Vector3*>();

    for(std::vector<Waypoint*>::const_iterator it=waypoints.begin();it!=waypoints.end(); ++it) {
        w = *it;
        if(*(w->getName())=="en_spawn") {
            int x = w->getX();
            int y = w->getY();

            double xx = (x * (Const::TILE_SIZE)) + (0.5*Const::TILE_SIZE);
            double yy = (y * (Const::TILE_SIZE)) + (0.5*Const::TILE_SIZE);

            Vector3 *v = new Vector3(xx,0.0,yy);

            places.push_back(v);
        }
    }

    return places;
}

Vector3 MapManager::getDynamicSpawnPoint(Vector3 *locn) {
    int x = (int) floor(locn->x/(double)Const::TILE_SIZE);
    int y = (int) floor(locn->z/(double)Const::TILE_SIZE);

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

    Vector3* sp =  mts[x][y]->getAdjacent(c)->getAdjacent(c2)->getSpawn(c3);
    return Vector3(sp->x,sp->y,sp->z);
}



int MapManager::getMeshList(string dir, std::vector<string>& files, int x, int y)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp = opendir(dir.c_str()))==NULL) {
        cerr << "Error(" << errno << ") opening " << dir << " at " << x << " " << y << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        if((string(dirp->d_name).compare(".")!=0)&&(string(dirp->d_name).compare("..")!=0)) {
            string path = dir;
            path += string(dirp->d_name);
            files.push_back(path);
        }
    }
    closedir(dp);

    return 0;
}

std::vector<int> MapManager::getConnections(int x, int y)
{
    std::vector<int> connections = std::vector<int>();
    if(x!=Const::MAPSIZE) {
        if(geo[x+1][y]!='.') connections.push_back(2);
    }
    if(y!=Const::MAPSIZE) {
        if(geo[x][y+1]!='.') connections.push_back(3);
    }
    if(x!=0) {
        if(geo[x-1][y]!='.') connections.push_back(4);
    }
    if(y!=0) {
        if(geo[x][y-1]!='.') connections.push_back(1);
    }

    // Sort the output
    sort(connections.begin(),connections.end());

    return connections;
}

int MapManager::cavernChecker(int x, int y, char type)
{
    int cavCount = 0;
    bool up,down,left,right=false;

    if(x!=Const::MAPSIZE) {
        if(geo[x+1][y]==type) cavCount++; right = true;
    }
    if(y!=Const::MAPSIZE) {
        if(geo[x][y+1]==type) cavCount++; up = true;
    }
    if(x!=0) {
        if(geo[x-1][y]==type) cavCount++; left = true;
    }
    if(y!=0) {
        if(geo[x][y-1]==type) cavCount++; down = true;
    }

    if(cavCount==0) {
        return 0; // Single cavenr piece
    } else {
        if(cavCount==4) {
            return 5;
        } else if (cavCount==3) {
            if((up==true)&&(right==true)&&(left==true)) return 8;
            if((up==true)&&(down==true)&&(left==true)) return 6;
            if((up==true)&&(down==true)&&(right==true)) return 4;
            if((down==true)&&(right==true)&&(left==true)) return 2;
        } else if (cavCount==2) {
            if((right==true)&&(down==true)) return 1;
            if((left==true)&&(down==true)) return 3;
            if((right==true)&&(up==true)) return 7;
            if((left==true)&&(up==true)) return 9;
        } else {
            return -1;
        }
    }
    return -1;
}

MapManager::MapManager(char* file, SceneManager *sceneManager)
	: sceneManager(sceneManager)
{
    MAPROOT = ConstManager::getString("map_file_path");
    if(buildMap(file)) {
    } else {
        cerr << "Something to do with reading the map went horribly wrong." << endl;
    }
}
