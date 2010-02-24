#ifndef _CONSOLE_H 
#define _CONSOLE_H

#include <OGRE/Ogre.h>
#include "ITickable.h"
#include <string>
#include <list>

#define CONSOLE_LENGTH 60
#define CONSOLE_HEIGHT 15

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
        std::list<std::string> *lines;
        std::string prompt;

        void displayText();
    public:
        Console(SceneManager *sceneMgr);
        ~Console();

        void append(std::string c);
        void tick();
        void setVisible(bool visible);
        void typeShit(std::string c);
        void enterCommand();
};

#endif

