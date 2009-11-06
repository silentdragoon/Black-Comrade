
#ifndef MAIN_H
#define MAIN_H

// Includes all Ogre classes
#include <OGRE/Ogre.h>

#include "ExampleFrameListener.h"

using namespace Ogre;

class Main {
private:
    Root *root;
    Camera *camera;
    SceneManager *sceneMgr;
    RenderWindow *window;
    ExampleFrameListener *frameListener;
    
    SceneNode *robotNode;
    
    void createCamera();
    void createScene();
    void createViewPort();
    void createFrameListener(void);
    
public:
    Main();
};

#endif
