#ifndef CONSOLE_MINIGAME_H
#define CONSOLE_MINIGAME_H

#include <iostream>
#include "IMiniGame.h"
#include "console.h"
#include "inputState.h"

class ConsoleMiniGame : public IMiniGame
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
    
        ConsoleMiniGame(Console *console, InputState *inputState);
        ~ConsoleMiniGame();
};

#endif
