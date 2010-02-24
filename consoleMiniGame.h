#ifndef CONSOLE_MINIGAME_H
#define CONSOLE_MINIGAME_H

#include <iostream>
#include "IMiniGame.h"
#include "console.h"
#include "inputState.h"
#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>

class ConsoleMiniGame : public IMiniGame, public OIS::KeyListener
{
    private:
        InputState *inputState;
        Console *console;

        bool isEnd;
        int score;
        
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
