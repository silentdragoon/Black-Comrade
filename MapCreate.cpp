#include "MapCreate.h"

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

bool MapCreate::outputMap()
{
    for(int xpos=0;xpos<MAPSIZE;xpos++) {
        for(int ypos=0;ypos<MAPSIZE;ypos++) {
            if(geo[xpos][ypos] == 's') {
                vector<int> connections = vector<int>();
                connections = getConnections(xpos,ypos);
                if(connections.size()==1) {
                    vector<string> files = vector<string>();
                    string dir = MAPROOT;
                    dir += "ends/";
                    stringstream out;
                    out << connections.at(0);
                    dir += out.str();
                    dir += "/";
                    getMeshList(dir,files);
                    // TODO: Select random file from list
                    random_shuffle(files.begin(),files.end());
                    //cout << files.at(0) << endl;
                    // TODO: Replace above line with code to add to ogre
                } else {
                    cerr << "Start has more than one connection." << endl;
                    return false;
                }
            } else if(geo[xpos][ypos] == '+') {
                vector<int> connections = vector<int>();
                connections = getConnections(xpos,ypos);
                if(connections.size()!=1) {
                    vector<string> files = vector<string>();
                    string dir = MAPROOT;
                    dir += "junct/";
                    stringstream out;
                    for(vector<int>::const_iterator it=connections.begin();it!=connections.end(); ++it) {
                        out << *it << "-";
                    }
                    string tmp = out.str();
                    dir += tmp.substr(0,tmp.length()-1);
                    dir += "/";
                    getMeshList(dir,files);
                    // TODO: Select random file from list
                    random_shuffle(files.begin(),files.end());
                    //cout << files.at(0) << endl;
                    // TODO: Replace above line with code to add to ogre
                } else {
                    cerr << "Junction at " << xpos << " " << ypos << " has 1 or less connections." << endl;
                    return false;
                }
            } else if(geo[xpos][ypos] == 'x') {
                vector<int> connections = vector<int>();
                connections = getConnections(xpos,ypos);
                vector<string> files = vector<string>();
                string dir = MAPROOT;
                if(connections.size()==2) {
                    dir += "strai/";
                } else if(connections.size()==1) {
                    dir += "ends/";
                } else if(connections.size()>2) {
                    cerr << "Component at " << xpos << " " << ypos << " is bad, consider junction." << endl;
                    return false;
                }
                stringstream out;
                for(vector<int>::const_iterator it=connections.begin();it!=connections.end(); ++it) {
                    out << *it << "-";
                }
                string tmp = out.str();
                dir += tmp.substr(0,tmp.length()-1);
                dir += "/";
                getMeshList(dir,files);
                // TODO: Select random file from list
                random_shuffle(files.begin(),files.end());
                //cout << files.at(0) << endl;
                // TODO: Replace above line with code to add to ogre

            } else if(geo[xpos][ypos] == 'e') {
                vector<int> connections = vector<int>();
                connections = getConnections(xpos,ypos);
                if(connections.size()==1) {
                    vector<string> files = vector<string>();
                    string dir = MAPROOT;
                    dir += "ends/";
                    stringstream out;
                    out << connections.at(0);
                    dir += out.str();
                    dir += "/";
                    getMeshList(dir,files);
                    // TODO: Select random file from list
                    random_shuffle(files.begin(),files.end());
                    //cout << files.at(0) << endl;
                    // TODO: Replace above line with code to add to ogre
                } else {
                    cerr << "End has more than one connection." << endl;
                    return false;
                }

            } else if(geo[xpos][ypos] == 'c') {

            } else if(geo[xpos][ypos] == 'o') {

            }
        }
    }

    return true;
}

int MapCreate::getMeshList(string dir, vector<string>& files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp = opendir(dir.c_str()))==NULL) {
        cerr << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        if((string(dirp->d_name).compare(".")!=0)&&(string(dirp->d_name).compare("..")!=0)) {
            files.push_back(string(dirp->d_name));
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

MapCreate::MapCreate(char* file)
{
    if(buildMap(file)) {
        outputMap();
    } else {
        cerr << "Something to do with reading the map went horribly wrong." << endl;
    }
}

int main(int argc, char* argv[])
{
    MapCreate *me = new MapCreate(argv[1]);
    return 0;
}