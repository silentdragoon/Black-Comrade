
#ifndef LINES_H
#define LINES_H

// Includes all Ogre classes
#include <OGRE/Ogre.h>

#include "DynamicLines.h"

using namespace Ogre;
using namespace std;

class Lines {
private:
    SceneManager *mSceneMgr;
    
    SceneNode *linesNode;
    DynamicLines *lines;
    
public:
    void addLine( Vector3 *a, Vector3 *b );
    void addPoly(  Vector3 *a, Vector3 *b, Vector3 *c );
    void addCross( Vector3 *a );
    void addLine( double ax, double ay, double az, double bx, double by, double bz);

    void clear();
    
    Lines( SceneManager *sceneMgr );
    void clearWithoutUpdate();
};

#endif
