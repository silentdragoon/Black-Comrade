#ifndef _CONSOLE_H 
#define _CONSOLE_H

#include <OGRE/Ogre.h>
#include "ITickable.h"
#include <string>

using namespace Ogre;

class Console : public ITickable 
{
    private:
        SceneManager *sceneMgr;
        Rectangle2D *rect;
        SceneNode *node;
        OverlayElement *textbox;
        Overlay *overlay;
        
        float height;
        int rollTick;
        bool isVisible;


    public:
        Console(SceneManager *sceneMgr);
        ~Console();

        void append(std::string c);
        void tick();
        void setVisible(bool visible);
};

#endif

