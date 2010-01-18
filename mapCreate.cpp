#include "mapCreate.h"

bool MapCreate::buildMap(char* file)
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
            for(int xpos=0;xpos<MAPSIZE;xpos++) {
                if(line[xpos]=='.') geo[xpos][ypos] = '.';
                if(line[xpos]=='s') geo[xpos][ypos] = 's'; start = true; startx = xpos; starty = ypos;
                if(line[xpos]=='+') geo[xpos][ypos] = '+';
                if(line[xpos]=='x') geo[xpos][ypos] = 'x';
                if(line[xpos]=='e') geo[xpos][ypos] = 'e'; end = true; endx = xpos; endy = ypos;
                if(line[xpos]=='c') geo[xpos][ypos] = 'c';
                if(line[xpos]=='o') geo[xpos][ypos] = 'o'; objec = true;
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

void MapCreate::addEnemies()
{

}

bool MapCreate::outputMap(SceneNode *sceneNode)
{
    for(int xpos=0;xpos<MAPSIZE;xpos++) {
        for(int ypos=0;ypos<MAPSIZE;ypos++) {
            if(geo[xpos][ypos] == 's') {
                startx = (xpos * TILE_SIZE)+(TILE_SIZE/2);
                starty = (ypos * TILE_SIZE)+(TILE_SIZE/2);

                cout << "Startx: " << startx << " Start y: " << starty << endl;

                vector<int> connections = vector<int>();
                connections = getConnections(xpos,ypos);
                if(connections.size()==1) {
                    fetchTile("ends/",connections,xpos,ypos,sceneNode);
                } else {
                    cerr << "Start has more than one connection." << endl;
                    return false;
                }
            } else if(geo[xpos][ypos] == '+') {
                vector<int> connections = vector<int>();
                connections = getConnections(xpos,ypos);
                if(connections.size()!=1) {
                    fetchTile("junct/",connections,xpos,ypos,sceneNode);
                } else {
                    cerr << "Junction at " << xpos << " " << ypos << " has 1 or less connections." << endl;
                    return false;
                }
            } else if(geo[xpos][ypos] == 'x') {
                vector<int> connections = vector<int>();
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
                vector<int> connections = vector<int>();
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
                    vector<string> files = vector<string>();
                    string dir = MAPROOT;
                    dir += "caves/1-2-3-4/";
                    getMeshList(dir,files,xpos,ypos);
                    // TODO: Select random file from list
                    random_shuffle(files.begin(),files.end());
                    
                    attachTile(sceneNode, &files.at(0), xpos, ypos);
                } else if(cavtest==7) {
                    vector<string> files = vector<string>();
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
                    vector<string> files = vector<string>();
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

    return true;
}

void MapCreate::fetchTile(string adir, vector<int> connections, int x, int y, SceneNode *sceneNode)
{
    vector<string> files = vector<string>();
    string dir = MAPROOT;
    dir += adir;
    stringstream out;
    for(vector<int>::const_iterator it=connections.begin();it!=connections.end(); ++it) {
        out << *it << "-";
    }
    string tmp = out.str();
    dir += tmp.substr(0,tmp.length()-1);
    dir += "/";
    
    getMeshList(dir,files,x,x);

    // TODO:  This is the bit where we need to randomise the vector, or somehow randomis the file returned.
    random_shuffle(files.begin(),files.end());

    attachTile(sceneNode, &files.at(0), x, y);
}

void MapCreate::attachTile(SceneNode *sceneNode, string *file, int x, int y)
{
	SceneNode *node = sceneNode->createChildSceneNode();

    string name = "mapTile";
    stringstream out;
    out << "-" << x << "-" << y;
    name += out.str();
	Entity *e = sceneManager->createEntity(name, *file);
    mapEntities.push_back( e );
	
	node->attachObject(e);

    cout << file << ": " << (x * TILE_SIZE) << " " << (y * TILE_SIZE) << endl;
	
	Vector3 pos(x * TILE_SIZE,0 , y * TILE_SIZE);
	
	node->setPosition(pos);
}

vector<Entity*> MapCreate::getMapPieces()
{
    return mapEntities;
}

Entity* MapCreate::getEntity(Vector3 *locn) 
{
    int x = floor(locn->x/(double)TILE_SIZE);
    int y = floor(locn->z/(double)TILE_SIZE);
    string name = "mapTile";
    stringstream out;
    out << "-" << x << "-" << y;
    name += out.str();
    Entity* e;
    try
    {
        e = sceneManager->getEntity(name);
    }
    catch(...)
    {
        return NULL;
    }
    return e;
}


int MapCreate::getMeshList(string dir, vector<string>& files, int x, int y)
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

vector<int> MapCreate::getConnections(int x, int y)
{
    vector<int> connections = vector<int>();
    if(x!=MAPSIZE) {
        if(geo[x+1][y]!='.') connections.push_back(2);
    }
    if(y!=MAPSIZE) {
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

int MapCreate::cavernChecker(int x, int y, char type)
{
    int cavCount = 0;
    bool up,down,left,right=false;

    if(x!=MAPSIZE) {
        if(geo[x+1][y]==type) cavCount++; right = true;
    }
    if(y!=MAPSIZE) {
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

MapCreate::MapCreate(char* file, SceneManager *sceneManager)
	: sceneManager(sceneManager)
{
    if(buildMap(file)) {
    } else {
        cerr << "Something to do with reading the map went horribly wrong." << endl;
    }
}

//int main(int argc, char* argv[])
//{
//    MapCreate *me = new MapCreate(argv[1], NULL);
//    return 0;
//}
