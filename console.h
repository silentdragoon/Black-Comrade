#ifndef _CONSOLE_H 
#define _CONSOLE_H

#include <OGRE/Ogre.h>
#include "ITickable.h"
#include <string>
#include <list>
#include <iterator>

#define CONSOLE_LENGTH 60
#define CONSOLE_HEIGHT 24
#define CONSOLE_WIDTH 115

using namespace Ogre;

class Console : public ITickable 
{
    private:
        SceneManager *sceneMgr;
        OverlayContainer *panel;
        OverlayElement *textbox;
        Overlay *overlay;

        int slideTicks;
        float openHeight;
        float top;
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

        void forceHide();

        int getWidth();
        int getHeight();

        void setChar(char c, int x, int y);
        void setString(std::string str, int xStart, int yStart);

        void appendLine(std::string s);
        void appendBlankLine();
        void appendBlankLines(int number);
        void makeBlank();

        void returnKeyPrompt();
        void backSpacePrompt();
        void appendToPrompt(std::string s);
        void appendToPrompt(char c);
        void clearPrompt();
};

#endif

