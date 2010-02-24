#ifndef CONSOLE_MINIGAME_H
#define CONSOLE_MINIGAME_H

#include <iostream>
#include <string>
#include "IMiniGame.h"
#include "console.h"
#include "inputState.h"
#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>
#include <boost/algorithm/string/trim.hpp>

class ConsoleMiniGame : public IMiniGame, public OIS::KeyListener
{
    private:
        InputState *inputState;
        Console *console;

        std::string command;

        bool isEnd;
        int score;

        void processCommand();
        
    public:       
        void tick();
        bool end();
        int getScore();
    
        Overlay *getOverlay();

        bool keyPressed (const OIS::KeyEvent &arg);
        bool keyReleased (const OIS::KeyEvent &arg);
    
        ConsoleMiniGame(Console *console, InputState *inputState);
        ~ConsoleMiniGame();
};

#endif
