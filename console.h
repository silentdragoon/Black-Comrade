#ifndef _CONSOLE_H 
#define _CONSOLE_H

#include <OGRE/Ogre.h>
#include "ITickable.h"
#include <string>
#include <list>

#define CONSOLE_LENGTH 60
#define CONSOLE_HEIGHT 23 

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

        void tick();

        bool getVisible();
        void setVisible(bool visible);

        void appendLine(std::string s);

        void returnKeyPrompt();
        void backSpacePrompt();
        void appendToPrompt(std::string s);
        void appendToPrompt(char c);
        void clearPrompt();
};

#endif

